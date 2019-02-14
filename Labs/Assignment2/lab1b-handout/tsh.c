/*
 * tsh - A tiny shell program
 *
 */

/*
 *******************************************************************************
 * INCLUDE DIRECTIVES
 *******************************************************************************
 */

#include <ctype.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "jobctrl.h"

/*
 *******************************************************************************
 * TYPE DEFINITIONS
 *******************************************************************************
 */

typedef void
handler_t (int);

/*
 *******************************************************************************
 * PREPROCESSOR DEFINITIONS
 *******************************************************************************
 */

#define MAXLINE 1024 /* max line size */
#define MAXARGS 128  /* max args on a command line */

/*
 *******************************************************************************
 * GLOBAL VARIABLES
 *******************************************************************************
 */

extern char** environ; /* defined in libc */

char const* const PROMPT = "tsh> "; /* command line prompt */

// JID, PID, signal
char const* const CHLD_STOP_MSG = "Job [%d] (%d) stopped by signal %d\n";
char const* const CHLD_TERM_MSG = "Job [%d] (%d) terminated by signal %d\n";

// JID, PID, command (when displaying after job running in bg)
char const* const BG_DISPLAY = "[%d] (%d) %s";

char const* const BAD_CMD = "%s: Command not found\n";
char const* const FG_BG_KILL_NOARG = "%s command requires PID or %%jobid argument\n";

char const* const FG_BG_KILL_NOPID = "(%d): No such process\n";
char const* const FG_BG_KILL_NOJID = "%s: No such job\n";
char const* const FG_BG_KILL_BADARG = "%s: argument must be a PID or %%jobid\n";
/*
 *******************************************************************************
 * FUNCTION PROTOTYPES
 *******************************************************************************
 */

int
parseline (const char* cmdline, char** argv);

void
unix_error (char* msg);

void
app_error (char* msg);

handler_t*
Signal (int signum, handler_t* handler);

void
sigquit_handler (int sig);

void
sigchld_handler (int sig);

void
sigint_handler (int sig);

void
sigtstp_handler (int sig);

void
eval (char *cmdline);

int 
builtin_command(char **argv);

void
waitfg (pid_t pid);

pid_t
Fork(void);

void
jobs();

/*
 *******************************************************************************
 * MAIN
 *******************************************************************************
 */

int
main (int argc, char** argv)
{
  /* Redirect stderr to stdout */
  dup2 (1, 2);

  /* Install signal handlers */
  Signal (SIGINT, sigint_handler);   /* ctrl-c */
  Signal (SIGTSTP, sigtstp_handler); /* ctrl-z */
  Signal (SIGCHLD, sigchld_handler); /* Terminated or stopped child */
  Signal (SIGQUIT, sigquit_handler); /* quit */

  /* TODO -- shell goes here */
  char cmdline[MAXLINE];

  jobctrl_init();
  while (1){
	printf("%s",PROMPT);
	fflush(stdout);
	fgets(cmdline, MAXLINE, stdin);
	if(feof(stdin))
		exit(0);
	eval(cmdline);
  }
  /* Quit */
  exit (0);
}

/*
 *******************************************************************************
 * HELPER ROUTINES
 *******************************************************************************
 */


/*
 * unix_error - unix-style error routine
 */
void
unix_error (char* msg)
{
  fprintf (stdout, "%s: %s\n", msg, strerror (errno));
  exit (1);
}

/*
*  app_error - application-style error routine
*/
void
app_error (char* msg)
{
  fprintf (stdout, "%s\n", msg);
  exit (1);
}

/*
*  Signal - wrapper for the sigaction function
*/
handler_t*
Signal (int signum, handler_t* handler)
{
  struct sigaction action, old_action;

  action.sa_handler = handler;
  sigemptyset (&action.sa_mask); /* block sigs of type being handled*/
  action.sa_flags = SA_RESTART;  /* restart syscalls if possible*/

  if (sigaction (signum, &action, &old_action) < 0)
    unix_error ("Signal error");
  return (old_action.sa_handler);
}

/*
*  sigquit_handler - The driver program can gracefully terminate the
*     child shell by sending it a SIGQUIT signal.
*/
void
sigquit_handler (int sig)
{
  printf ("Terminating after receipt of SIGQUIT signal\n");
  exit (1);
}

/*
*  parseline - Parse the command line and build the argv array.
*
*  Characters enclosed in single quotes are treated as a single
*  argument.
*
*  Returns true if the user has requested a BG job, false if
*  the user has requested a FG job.
*/
int
parseline (const char* cmdline, char** argv)
{
  static char array[MAXLINE]; /* holds local copy of command line*/
  char* buf = array;          /* ptr that traverses command line*/
  char* delim;                /* points to first space delimiter*/
  int argc;                   /* number of args*/
  int bg;                     /* background job?*/

  strcpy (buf, cmdline);
  buf[strlen (buf) - 1] = ' ';  /* replace trailing '\n' with space*/
  while (*buf && (*buf == ' ')) /* ignore leading spaces*/
    buf++;

  /* Build the argv list*/
  argc = 0;
  if (*buf == '\'')
  {
    buf++;
    delim = strchr (buf, '\'');
  }
  else
  {
    delim = strchr (buf, ' ');
  }

  while (delim)
  {
    argv[argc++] = buf;
    *delim = '\0';
    buf = delim + 1;
    while (*buf && (*buf == ' ')) /* ignore spaces*/
      buf++;

    if (*buf == '\'')
    {
      buf++;
      delim = strchr (buf, '\'');
    }
    else
    {
      delim = strchr (buf, ' ');
    }
  }
  argv[argc] = NULL;

  if (argc == 0) /* ignore blank line*/
  {
    return 1;
  }

  /* should the job run in the background?*/
  if ((bg = (*argv[argc - 1] == '&')) != 0)
  {
    argv[--argc] = NULL;
  }
  return bg;
}
/*
 *******************************************************************************
 * SIGNAL HANDLERS
 *******************************************************************************
 */

/*
*  sigchld_handler - The kernel sends a SIGCHLD to the shell whenever
*      a child job terminates (becomes a zombie), or stops because it
*      received a SIGSTOP or SIGTSTP signal. The handler reaps all
*      available zombie children, but doesn't wait for any other
*      currently running children to terminate.
*/
void
sigchld_handler (int sig)
{
	pid_t pid;
	int status;
	int fg_pid;
	while((pid = waitpid (-1, &status, WNOHANG|WUNTRACED)) > 0 )
	{
		if (WIFSTOPPED(status))
 			 {
				printf("Job (%d) stopped by signal %d\n", (fg_pid = jobctrl_fgpid()), WSTOPSIG(status));
				fflush(stdout);
 			 }
 		else if (WIFSIGNALED (status))
  			 {
				printf("Job (%d) terminated by signal %d\n", (fg_pid = jobctrl_fgpid()), WTERMSIG(status));
				fflush(stdout);
				jobctrl_delete(fg_pid);
 			 }
  		else if (WIFEXITED (status))
  		 	 {
				 fg_pid = jobctrl_fgpid();
				 jobctrl_delete(fg_pid);
     			 }
	}
}

/*
 *  sigint_handler - The kernel sends a SIGINT to the shell whenEver the
 *     user types ctrl-c at the keyboard.  Catch it and send it along
 *     to the foreground job.
 */
void
sigint_handler (int sig)
{
  pid_t fg_job;
  if ((fg_job = jobctrl_fgpid()) != 0)
  {
	kill(-fg_job, SIGINT);
	jobctrl_delete(fg_job);
  }
}

/*
 *  sigtstp_handler - The kernel sends a SIGTSTP to the shell whenever
 *      the user types ctrl-z at the keyboard. Catch it and suspend the
 *      foreground job by sending it a SIGTSTP.
 */
void
sigtstp_handler (int sig)
{
  pid_t fg_job;
  if ((fg_job = jobctrl_fgpid()) != 0)
  {
	kill(-fg_job, SIGTSTP);
	jobctrl_delete(fg_job);
  }
}

/*
 *******************************************************************************
 * HELPER ROUTINES
 *******************************************************************************
 */


/*
 * eval - evaluate entered shell commands
 *
 * takes *cmdline, the line of shell input to be evaluated.
 */
void
eval (char *cmdline){
	char *argv[MAXARGS];
	char buf[MAXLINE];
	int bg;
	pid_t pid;
	sigset_t mask;
	sigset_t prevMask;
	sigemptyset(&mask);
	sigaddset(&mask, SIGCHLD);
	sigaddset(&mask, SIGTSTP);
	sigaddset(&mask, SIGINT);
	sigprocmask(SIG_BLOCK, &mask, &prevMask);
	strcpy(buf, cmdline);
	bg = parseline(buf, argv);
	if (argv[0] == NULL)
		return;
	if(!builtin_command(argv)){
		if ((pid = Fork()) == 0){
			sigprocmask(SIG_SETMASK, &prevMask, NULL);
			setpgid(0, 0);
			if (execve(argv[0], argv, environ) < 0) {
				printf("%s: Command not found\n", argv[0]);
				fflush(stdout);
			}
		}
		sigprocmask(SIG_SETMASK, &prevMask, NULL);
		if (!bg) {
			waitfg(pid);
		}
		else
		{
			printf("(%d) %s", pid, cmdline);
			fflush(stdout);
		}
	}
}

/*
 * builtin_command - allows features like "quit" and "fg" to be immediatly evaluated.
 *
 * takes argv, a potential built-in command
*/
int builtin_command(char **argv)
{
	pid_t fg_pid;

	if (!strcmp(argv[0], "quit"))
	{
		exit(0);
	}
	if(!strcmp(argv[0], "fg"))
	{
		if((fg_pid = jobctrl_fgpid()) != 0)
		{
			kill(-fg_pid, SIGCONT);
			waitfg(fg_pid);
		}
		return 1;
	}
	if(!strcmp(argv[0], "jobs"))
	{
		jobs();
		return 1;
	}
	else if(!strcmp(argv[0], "&"))
	{
		return 1;
	}
	return 0;
}

/*
 * waitfg - waitfg makes the shell wait for a foreground process to finish, takes in no parameters.
*/
void
waitfg (pid_t pid)
{
	pid_t fg_pid;
	//while runnig & not interrupted, suspend. call sig suspend in a while loop. if running pid is 0, stop waiting.
	sigset_t mask;
	sigemptyset(&mask);
	while((fg_pid = jobctrl_fgpid()) == pid)
	{
		sigsuspend(&mask);
	}
}

/*
 *Fork - wrapper class for the fork() function
*/
pid_t
Fork(void)
{
	pid_t pid;

	if((pid=fork()) < 0)
		unix_error("Fork Error");
	return pid;
}

/*
 *Jobs - prints a list of all jobs.
*/
void
jobs()
{
	jobctrl_list();
}
