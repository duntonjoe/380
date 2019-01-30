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

/*
 *******************************************************************************
 * TYPE DEFINITIONS
 *******************************************************************************
 */

typedef void handler_t (int);

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

extern char** environ;   /* defined in libc */
char prompt[] = "tsh> "; /* command line prompt */

char sbuf[MAXLINE]; /* for composing sprintf messages */

/*
 *******************************************************************************
 * FUNCTION PROTOTYPES
 *******************************************************************************
 */

int
parseline(const char* cmdline, char* *argv);

void
sigquit_handler(int sig);

void
unix_error(char* msg);

void
app_error(char* msg);

handler_t*
Signal(int signum, handler_t* handler);

void
eval (char *cmdline);

int builtin_command(char **argv);

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
  //Signal (SIGINT, sigint_handler);   /* ctrl-c */
  //Signal (SIGTSTP, sigtstp_handler); /* ctrl-z */
  //Signal (SIGCHLD, sigchld_handler); /* Terminated or stopped child */
  //Signal (SIGQUIT, sigquit_handler); /* quit */

  /* TODO -- shell goes here*/

  char cmdline[MAXLINE];

  while (1){

	printf("~> ");
	fgets(cmdline, MAXLINE, stdin);
	if(feof(stdin))
		exit(0);
	//^Read
	
	eval(cmdline);

  }

  /* Quit */
  exit (0);
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
    return 1;

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
  return;
}

/*
 *  sigint_handler - The kernel sends a SIGINT to the shell whenEver the
 *     user types ctrl-c at the keyboard.  Catch it and send it along
 *     to the foreground job.
 */
void
sigint_handler (int sig)
{
  return;
}

/*
 *  sigtstp_handler - The kernel sends a SIGTSTP to the shell whenever
 *      the user types ctrl-z at the keyboard. Catch it and suspend the
 *      foreground job by sending it a SIGTSTP.
 */
void
sigtstp_handler (int sig)
{
  return;
}

/*
 *******************************************************************************
 * HELPER ROUTINES
 *******************************************************************************
 */


/*
 * eval - evaluate entered shell commands
 */
void
eval (char *cmdline){

	char *argv[MAXLINE];
	char buf[MAXLINE];
	int bg;
	pid_t pid;

	strcpy(buf, cmdline);
	bg = parseline(buf, argv);
	if (argv[0] == NULL)
		return;

	if(!builtin_command(argv)){
		if ((pid = fork()) == 0){
			if (execve(argv[0], argv, environ) < 0) {
				printf("%s; Command not found. \n", argv[0]);
				exit(0);
			}
		}
		if (!bg) {
			int status;
			if (waitpid(pid, status, 0) < 0)
				unix_error("waitfg: waitpid error");
		}
		else
			printf("%d %s", pid, cmdline);
	}
	return;
}

int builtin_command(char **argv)
{
	if (!strcmp(argv[0], "quit"))
		exit(0);
	if(!strcmp(argv[0], "&"))
		return 1;
	return 0;
}
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
