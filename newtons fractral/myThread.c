/*
   myThread.c: my Thread library
   Shun Yan Cheung
   June 1997

   Solaris 2.5 version
*/


#include <stdio.h>
#include "myThread.h"

/* =======================================================================
	Thread management
   ======================================================================= */
/* -------------------------------------------------------------------
   Thread_Create(proc)

   input: any C function of the form:
		void *func(void *param)
   Effect: start a thread executing this function
   Return: thread id
   ------------------------------------------------------------------- */
ThreadID Thread_Create( void *my_proc(void *), void *param)
{
   ThreadID ID;
   pthread_attr_t attr;

   if (pthread_attr_init(&attr) != 0)
    { perror("Thread_Create: pthread_attr_init");
      exit(1);
    }
   if (pthread_create(&ID, &attr, my_proc, param) != 0)
    { perror("Thread_Create: pthread_create");
      exit(1);
    }
   return(ID);
}

/* -------------------------------------------------------------------
   Thread_Yield(): make thread yield its processor
   ------------------------------------------------------------------- */
void Thread_Yield()
{
   sched_yield();	/* pthread does not allow you to pass processor
			   to another another
			*/
}

/* -------------------------------------------------------------------
   Thread_Sleep(): make thread sleep for usec micro-seconds

   Note: we need this function to make an individual thread sleep
	 - we can't use "usleep()" because it's a *process* wide
	 - function, the wake up is sent to the process (handled by
	 - and thread), can't target a specific thread.
   ------------------------------------------------------------------- */
void Thread_Sleep(int sleeptime)
{
   static timed_bin_sema *s = NULL;
   struct timeval abstime;
 
   if (s == NULL)
      TimedBinSemaInit(&s, 0);

   /* ---------------------------------------------------------------
      Now wait by P-ing(semaphore)
      - This P-operation will never succeed, so thread wakes up
      - after usec delay
      --------------------------------------------------------------- */
   TimedBinSemaP(s, sleeptime);
}



/* =======================================================================
   Mutex functions: use these to synchronize threads accessing a common
		    variable
   ======================================================================= */
/* -------------------------------------------------------------------
   MutexInit(m): create a mutex variable and initialize it to UNLOCKED

   Usage:  mutex *m;
           MutexInit(&m);
   ------------------------------------------------------------------- */
void MutexInit(mutex **m)
{
   *m = (mutex *) malloc(sizeof(mutex));
   pthread_mutex_init(&((*m)->mutex), NULL);
}

/* -----------------------------------------------------------------------
   MutexLock(m): lock a mutex... will succeed immediately if mutex was
		 UNLOCKED, and will BLOCK thread if mutex is locked...
		 When LOCKED, MutexLock will succeed when some other
		 thread called MutexUnLock(m)
   ----------------------------------------------------------------------- */
void MutexLock(mutex *m)
{
   if (m == NULL)
    { fprintf(stderr, "User error: uninitialized mutex variable\n");
      exit(1);
    }
   pthread_mutex_lock(&(m->mutex));
}

/* -----------------------------------------------------------------------
   MutexUnLock(m): unlock a mutex... 
   ----------------------------------------------------------------------- */
void MutexUnLock(mutex *m)
{
   if (m == NULL)
    { fprintf(stderr, "User error: uninitialized mutex variable\n");
      exit(1);
    }
   pthread_mutex_unlock(&(m->mutex));
}




/* =======================================================================
   Binary Semaphore (value 0 or 1)

   PThreads do not have binary semaphores, only counting semaphores.
   Binary semaphores can be implemented using a lower level construct:

		conditional variables.

   Semaphores can't be implemented with Mutex because the OWNER of the lock
   should do unlock on a mutex !
   ======================================================================= */
/* -----------------------------------------------------------------------
   BinSemaInit(s, value): set semaphore s to value (value = 0 or 1)
   ----------------------------------------------------------------------- */
void BinSemaInit(bin_sema **s, int value)
{
   if (value)
      value = 1;		/* make sure it's 0 or 1 */

   *s = (bin_sema *) malloc(sizeof(bin_sema));
   pthread_mutex_init(&((*s)->mutex), NULL);
   pthread_cond_init(&((*s)->cv), NULL);
   (*s)->flag = value;
}


/* -----------------------------------------------------------------------
   BinSemaP(s): P operation
   ----------------------------------------------------------------------- */
void BinSemaP(bin_sema *s)
{
   if (s == NULL)
    { fprintf(stderr, "User error: uninitialized bin_sema variable\n");
      exit(1);
    }
   pthread_mutex_lock(&(s->mutex));	/* Gain exclusive access to s->value */
   while (s->flag == 0)			/* Wait until s->flag == 1 to exit */
      pthread_cond_wait(&(s->cv), &(s->mutex) );    /* flag = 0, wait on cv */
					/* Note: pthread_cond_wait will release
						 lock on s->mutex while waiting
						 on s->cv...
					*/

   /* -----------------------------------------------------------
      When you exit the while loop, s->flag == 1 and this thread
      has successfully pass the condition (s->flag == 0)
      ----------------------------------------------------------- */
   s->flag = 0;				/* Block other thread from entering */
   pthread_mutex_unlock(&(s->mutex));   /* Release exclusive access */
}


/* -----------------------------------------------------------------------
   BinSemaV(s): V operation
   ----------------------------------------------------------------------- */
void BinSemaV(bin_sema *s)
{
   if (s == NULL)
    { fprintf(stderr, "User error: uninitialized bin_sema variable\n");
      exit(1);
    }
   pthread_mutex_lock(&(s->mutex));	/* Gain exclusive access to s->value */
   if (s->flag == 0)			/* Check if some process is blocked */
      pthread_cond_signal(&(s->cv));    /* Signal process blocked on s->cv */
   s->flag = 1;
   pthread_mutex_unlock(&(s->mutex));	/* Release exclusive access */
}

 
/* -----------------------------------------------------------------------
   BinSemaDestroy(s): destry semaphore s

   Note: do NOT wake up any threads blocked on s...
   ----------------------------------------------------------------------- */
void BinSemaDestroy(bin_sema **s)
{
   if (s == NULL)
    { fprintf(stderr, "User error: uninitialized bin_sema variable\n");
      exit(1);
    }
   free(*s);
   *s = NULL;
}

/* -----------------------------------------------------------------------
   BinSemaForce(s, value): Force reinitialization of a semaphore s to value 
			   (value = 0 or 1)

   Side effect: can unblock processes that are waiting for semaphore...
   ----------------------------------------------------------------------- */
void BinSemaForce(bin_sema *s, int value)
{
   if (s == NULL)
    { fprintf(stderr, "User error: uninitialized bin_sema variable\n");
      exit(1);
    }

   if (value)
      value = 1;		/* make sure it's 0 or 1 */

   pthread_mutex_lock(&(s->mutex));	/* Gain exclusive access to s->value */
   s->flag = value;
   if (value > 0)			/* Check if some processes are blocked*/
      pthread_cond_signal(&(s->cv));    /* if so, signal blocked processes */
   pthread_mutex_unlock(&(s->mutex));	/* Release exclusive access */
}


 


/* =======================================================================
        TIMED Binary Semaphore: SemaP that can time out...
   ======================================================================= */
/* -----------------------------------------------------------------------
   TimedBinSemaInit(s, value): set TIMED semaphore s to value (value = 0 or 1)
   ----------------------------------------------------------------------- */
void TimedBinSemaInit(timed_bin_sema **s, int value)
{
   BinSemaInit( (bin_sema **) s, value);
}


/* ------------------------------------------------------------------------
   TimedBinSemaP(s, timeout): P-operation with given timeout limit.

   timeout = timeout value given in micro-sec
   If timeout == 0, TimedBinSemaP(s, NULL) works like BinSemaP(s)

   Note: to compute abstime for timeout:

	 0 define a struct timeval t
	 1. use gettimeofday(&t) to get current time
	 2. add timeout value to t
	 3. pass &t as abstime to TimedBinSemaP()

   Returns: 1 if OK (got semaphore before "time" expired)
	    0 if error (did not get semaphore after time expired
   ------------------------------------------------------------------------ */
int TimedBinSemaP(timed_bin_sema *s, int timeout)
{
   int result;
   struct timeval now;

   if (timeout == 0)
    { BinSemaP( (bin_sema *) s);
      return(1);
    }
   else
    {
      gettimeofday(&(now), NULL);		/* Get current time */
      now.tv_usec += timeout;

      /* ------
         Adjust
         ------ */
      now.tv_sec += now.tv_usec/1000000;
      now.tv_usec %= 1000000; 
      return( TimedBinSemaP2(s, &now) );
   }
}



/* ------------------------------------------------------------------------
   TimedBinSemaP2(s, abstime): P-operation with given timeout limit.

   abstime = timeout clock value (absolute time value required)
   If abstime == NULL, TimedBinSemaP(s, NULL) works like BinSemaP(s)

   Note: to compute abstime for timeout:

	 0 define a struct timeval t
	 1. use gettimeofday(&t) to get current time
	 2. add timeout value to t
	 3. pass &t as abstime to TimedBinSemaP()

   Returns: 1 if OK (got semaphore before "time" expired)
	    0 if error (did not get semaphore after time expired
   ------------------------------------------------------------------------ */
int TimedBinSemaP2(timed_bin_sema *s, struct timeval *abstime)
{
   int result;
   struct timespec thr_time;
 
   if (abstime != NULL)
    { thr_time.tv_sec = abstime->tv_sec;
      thr_time.tv_nsec = 1000*abstime->tv_usec;

      pthread_mutex_lock(&(s->mutex));
      result = 0;
      while ( (s->flag == 0) && (result == 0) )
         result = pthread_cond_timedwait(&(s->cv), &(s->mutex), &thr_time);

      if (s->flag != 0)
       { s->flag = 0;
       }

      pthread_mutex_unlock(&(s->mutex));
      return(result == 0);
    }
   else
    { BinSemaP( (bin_sema *) s);
      return(1);
    }
}

/* ------------------------------------------------------------------------
   TimedBinSemaV(s, time): V-opeartion

   Returns: 1 if OK (got semaphore before time expired)
	    0 if error (did not get semaphore after time expired
   ------------------------------------------------------------------------ */
void TimedBinSemaV(timed_bin_sema *s)
{
   BinSemaV( (bin_sema *) s);		/* V always succeeds.... */
}

/* -----------------------------------------------------------------------
   TimedBinSemaForce(s, value): Force reinitialization of a semaphore s 
				to value (value = 0 or 1)

   Side effect: can unblock processes that are waiting for semaphore...
   ----------------------------------------------------------------------- */
void TimedBinSemaForce(timed_bin_sema *s, int value)
{
   BinSemaForce((bin_sema *)s, value);
}


/* =========================================================================
	Counting Semaphore
   ========================================================================= */
/* Pthread has counting semaphores... See man semaphore */
/* I would like to implement my own that is smarter - some day...  */
 
/* =========================================================================
	TimedCounting Semaphore
   ========================================================================= */
/* Not implemented yet... */


/* =======================================================================
	Unary Semaphore

   Used to make one process to synchr with another process:

	P1: UnaSemaP(&una_sema); X
	P2: ..... UnaSemaV(&una_sema); X ....

    P1 and P2 are synchr. at point X.

    --- Why on earth did I implemented these for ??? Not used at all...
   ======================================================================= */
void UnaSemaInit(una_sema *s)
{
   pthread_mutex_init(&(s->mutex), NULL);
   pthread_cond_init(&(s->cv), NULL);
}
 
void UnaSemaDestroy(una_sema *s)
{
   pthread_mutex_destroy(&(s->mutex));
   pthread_cond_destroy(&(s->cv));
}
 
void UnaSemaP(una_sema *s)
{
   pthread_mutex_lock(&(s->mutex));
   pthread_cond_wait(&(s->cv), &(s->mutex) );
   pthread_mutex_unlock(&(s->mutex));
}
 
void UnaSemaV(una_sema *s)
{
   pthread_cond_signal(&(s->cv));            /* signal blocked process */
}


