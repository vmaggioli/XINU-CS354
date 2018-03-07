/* resched.c - resched, resched_cntl */

#include <xinu.h>

// #define DEBUG 0

struct	defer	Defer;

/*------------------------------------------------------------------------
 *  resched  -  Reschedule processor to highest priority eligible process
 *------------------------------------------------------------------------
 */
void	resched(void)		/* Assumes interrupts are disabled	*/
{
	struct procent *ptold;	/* Ptr to table entry for old process	*/
	struct procent *ptnew;	/* Ptr to table entry for new process	*/
	
	/* If rescheduling is deferred, record attempt and return */

	if (Defer.ndefers > 0) {
		Defer.attempt = TRUE;
		return;
	}

	/* Point to process table entry for the current (old) process */

	ptold = &proctab[currpid];

	/* Vincent Maggioli 2/13 */	
	/* Set new priority - check if it's nulluser() */
	
	if (currpid == 0) {
		ptold->prprio = 0;		
	} else if ((ptold->prprio = MAXPRIO - getcputot(currpid)) < 1) {
		ptold->prprio = 1;
	}
	
	/* Print values on context switch if debugging */
	#ifdef DEBUG
		/* Only printing user processes */
		if (currpid > 2) {
			kprintf("Process %s\nCPU session time: %d\nCPU total time: %d\n\n", ptold->prname, clkmilli-ptold->prctxswbeg, getcputot(currpid));
		}
	#endif

	/* End changes */

	/* Sets beginning time in ms for new process */

	if (ptold->prstate == PR_CURR) {  /* Process remains eligible */
		if (ptold->prprio > firstkey(readylist)) {
			/* Vincent Maggioli 2/13 */
			/* Set new beginning for same process */
			ptold->prctxswbeg = clkmilli;
			/* End changes */
			return;
		}

		/* Old process will no longer remain current */

		ptold->prstate = PR_READY;
		insert(currpid, readylist, ptold->prprio);
	}

	/* Force context switch to highest priority ready process */

	currpid = dequeue(readylist);
	ptnew = &proctab[currpid];

	/* Vincent Maggioli 2/13 */
	/* Set new start time for new current process */

	ptnew->prctxswbeg = clkmilli;

	/* End changes */
	
	ptnew->prstate = PR_CURR;
	preempt = QUANTUM;		/* Reset time slice for process	*/
	ctxsw(&ptold->prstkptr, &ptnew->prstkptr);

	/* Old process returns here when resumed */

	return;
}

/*------------------------------------------------------------------------
 *  resched_cntl  -  Control whether rescheduling is deferred or allowed
 *------------------------------------------------------------------------
 */
status	resched_cntl(		/* Assumes interrupts are disabled	*/
	  int32	defer		/* Either DEFER_START or DEFER_STOP	*/
	)
{
	switch (defer) {

	    case DEFER_START:	/* Handle a deferral request */

		if (Defer.ndefers++ == 0) {
			Defer.attempt = FALSE;
		}
		return OK;

	    case DEFER_STOP:	/* Handle end of deferral */
		if (Defer.ndefers <= 0) {
			return SYSERR;
		}
		if ( (--Defer.ndefers == 0) && Defer.attempt ) {
			resched();
		}
		return OK;

	    default:
		return SYSERR;
	}
}
