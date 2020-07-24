/*
   myThread.h
   Solaris 2.5 version
*/

#include <thread.h>
#include <pthread.h>
#include <sched.h>

/* ===================================================================
   Types that depend on native threading system....
   =================================================================== */
typedef pthread_t      ThreadID;

/* ----------
   Mutex
   ---------- */
typedef struct MUTEX
 { pthread_mutex_t mutex;
 } mutex;


/* ----------
   Binary semaphore
   ---------- */
typedef struct BIN_SEMA
 { pthread_mutex_t mutex;    /* CANNOT implement bin semaphore by mutex lock! */
   pthread_cond_t  cv;       /* Processes block on this cond. variable */
   int     flag;             /* 0 = locked, 1 = OK */
 } bin_sema;
 
/* ----------
   Timed Binary semaphore
   ---------- */
typedef struct TIMED_BIN_SEMA
 { pthread_mutex_t mutex;    /* Bin semaphore is same as a mutex lock */
   pthread_cond_t  cv;
   int		   flag;
 } timed_bin_sema;
 
 
/* ----------
   Unary semaphore (actually a conditional variable)
   ---------- */
typedef struct UNA_SEMA
 { pthread_mutex_t mutex;    /* Mutex variable controlling access of flag */
   pthread_cond_t  cv;       /* Processes block on this cond. variable */
 } una_sema;
 



/* --------------------------------------------- Threads management */
ThreadID Thread_Create( void *my_proc(void *), void *param);
void Thread_Yield();
void Thread_Sleep(int usec);

/* ---------------------------------------------- Mutex */
void MutexInit(mutex **m);
void MutexLock(mutex *m);
void MutexUnLock(mutex *m);

/* ---------------------------------------------- Binary semaphore */
void BinSemaInit(bin_sema **s, int value /* 0 or 1 */);
void BinSemaP(bin_sema *s);
void BinSemaV(bin_sema *s);
void BinSemaDestroy(bin_sema **s);
void BinSemaForce(bin_sema *s, int value);

void TimedBinSemaInit(timed_bin_sema **s, int value /* 0 or 1 */);
int  TimedBinSemaP(timed_bin_sema *s, int timeout);
int  TimedBinSemaP2(timed_bin_sema *s, struct timeval *abstime);
void TimedBinSemaV(timed_bin_sema *s);
void TimedBinSemaForce(timed_bin_sema *s, int value);
 
/* ---------------------------------------------- Unary semaphore */
void UnaSemaInit(una_sema *s);
void UnaSemaP(una_sema *s);
void UnaSemaV(una_sema *s);



