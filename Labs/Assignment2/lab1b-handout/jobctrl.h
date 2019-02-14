/*
 * jobctrl.h -- a tiny job management library for a tiny shell
 */

#ifndef JOBCTRL_H_
#define JOBCTRL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#define JOBCTRL_MAXJOBS        16   /* max jobs at any point in time */
#define JOBCTRL_MAXJID    (1<<16)   /* max job ID */

#define UNDEF 0 /* undefined */
#define FG 1    /* running in foreground */
#define BG 2    /* running in background */
#define ST 3    /* stopped */

struct jobctrl_job {        /* The job struct */
    pid_t pid;              /* job PID */
    int jid;                /* job ID [1, 2, ...] */
    int state;              /* UNDEF, BG, FG, or ST */
    char cmdline[1024];     /* command line */
};

typedef struct jobctrl_job jobctrl_job_t;

/* The job list */
extern jobctrl_job_t jobctrl_jobs[JOBCTRL_MAXJOBS];

/*
 * Initialize the job list
 */
void
jobctrl_init ();

/*
 * Add a job to the job list
 *
 * Returns true on success.
 */
int
jobctrl_add (pid_t pid, int state, char *cmdline);

/*
 * Delete a job whose PID=pid from the job list
 *
 * Returns true on success.
 */
int
jobctrl_delete (pid_t pid);

/*
 * Return PID of current foreground job, 0 if no such job
 */
pid_t
jobctrl_fgpid ();

/*
 * Clear the entries in a job struct
 */
void
jobctrl_clear (jobctrl_job_t* job);

/*
 * Find a job (by PID) on the job list
 */
jobctrl_job_t*
jobctrl_get_by_pid (pid_t pid);

/*
 * Find a job (by JID) on the job list
 */
jobctrl_job_t*
jobctrl_get_by_jid (int jid);

/*
 * Given a PID, find the corresponding JID
 */
int
jobctrl_pid2jid (pid_t pid);

/*
 * Print the job list to stdout
 */
void
jobctrl_list ();

#endif
