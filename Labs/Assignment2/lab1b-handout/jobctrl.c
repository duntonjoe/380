#include <string.h>

#include "jobctrl.h"


/*
 * jobctrl.c -- a tiny job management library for a tiny shell (implementation)
 */

int nextjid = 1;            /* next job ID to allocate */
jobctrl_job_t jobctrl_jobs[JOBCTRL_MAXJOBS]; /* The job list */

/* maxjid - Returns largest allocated job ID */
int
maxjid()
{
    int max = 0;
    for (int i = 0; i < JOBCTRL_MAXJOBS; i++) {
        if (jobctrl_jobs[i].jid > max) {
            max = jobctrl_jobs[i].jid;
        }
    }
    return max;
}

void
jobctrl_clear (jobctrl_job_t* job) {
    job->pid = 0;
    job->jid = 0;
    job->state = UNDEF;
    job->cmdline[0] = '\0';
}

void
jobctrl_init () {
    for (int i = 0; i < JOBCTRL_MAXJOBS; i++) {
        jobctrl_clear (&jobctrl_jobs[i]);
    }
}

int
jobctrl_add (pid_t pid, int state, char* cmdline)
{
    if (pid < 1) {
        return 0;
    }
    for (int i = 0; i < JOBCTRL_MAXJOBS; i++) {
        if (jobctrl_jobs[i].pid == 0) {
            jobctrl_jobs[i].pid = pid;
            jobctrl_jobs[i].state = state;
            jobctrl_jobs[i].jid = nextjid++;
            if (nextjid > JOBCTRL_MAXJOBS) {
                nextjid = 1;
            }
            strcpy(jobctrl_jobs[i].cmdline, cmdline);
            return 1;
        }
    }
    fprintf(stdout, "Tried to create too many jobs\n");
    fflush(stdout);
    return 0;
}

int
jobctrl_delete (pid_t pid)
{
    if (pid < 1) {
        return 0;
    }
    for (int i = 0; i < JOBCTRL_MAXJOBS; i++) {
        if (jobctrl_jobs[i].pid == pid) {
            jobctrl_clear(&jobctrl_jobs[i]);
            nextjid = maxjid()+1;
            return 1;
        }
    }
    return 0;
}

pid_t
jobctrl_fgpid () {
    for (int i = 0; i < JOBCTRL_MAXJOBS; i++) {
        if (jobctrl_jobs[i].state == FG) {
            return jobctrl_jobs[i].pid;
        }
    }
    return 0;
}


jobctrl_job_t*
jobctrl_get_by_pid (pid_t pid) {
    if (pid < 1) {
        return NULL;
    }
    for (int i = 0; i < JOBCTRL_MAXJOBS; i++) {
        if (jobctrl_jobs[i].pid == pid) {
            return &jobctrl_jobs[i];
        }
    }
    return NULL;
}

/* getjobjid  - Find a job (by JID) on the job list */
jobctrl_job_t*
jobctrl_get_by_jid (int jid)
{
    if (jid < 1) {
        return NULL;
    }
    for (int i = 0; i < JOBCTRL_MAXJOBS; i++) {
        if (jobctrl_jobs[i].jid == jid) {
            return &jobctrl_jobs[i];
        }
    }
    return NULL;
}

/* pid2jid - Map process ID to job ID */
int
jobctrl_pid2jid (pid_t pid)
{
    if (pid < 1) {
        return 0;
    }
    for (int i = 0; i < JOBCTRL_MAXJOBS; i++) {
        if (jobctrl_jobs[i].pid == pid) {
            return jobctrl_jobs[i].jid;
        }
    }
    return 0;
}

/* listjobs - Print the job list */
void
jobctrl_list ()
{
    for (int i = 0; i < JOBCTRL_MAXJOBS; i++) {
        if (jobctrl_jobs[i].pid != 0) {
            fprintf (stdout, "[%d] (%d) ", jobctrl_jobs[i].jid, jobctrl_jobs[i].pid);
            switch (jobctrl_jobs[i].state) {
                case BG:
                    fprintf(stdout, "Running ");
                    break;
                case FG:
                    fprintf(stdout, "Foreground ");
                    break;
                case ST:
                    fprintf(stdout, "Stopped ");
                    break;
                default:
                    ;
            }
            fprintf (stdout, "%s", jobctrl_jobs[i].cmdline);
        }
    }
}
