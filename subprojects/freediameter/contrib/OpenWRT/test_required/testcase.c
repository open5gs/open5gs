#include <pthread.h>
#include <stdio.h>
#include <strings.h>
#include <assert.h>

static pthread_barrier_t bar;
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cnd = PTHREAD_COND_INITIALIZER;
static int called = 0;

#ifndef ASSERT
#define ASSERT(x) assert(x)
#endif /* ASSERT */

static void cleanupmutex(void * arg)
{
	printf("cancelation cleanup handler called\n");
	if (arg) {
		ASSERT( pthread_mutex_unlock((pthread_mutex_t *)arg) == 0 );
		called++;
	}
	
}

static void * mythread(void * a)
{
	int ret;
	
	/* lock mutex */
	ASSERT( pthread_mutex_lock(&mtx) == 0 );
		
	/* Push cleanup */
	pthread_cleanup_push(cleanupmutex, &mtx);
	
	printf("thread synchronization (mutex acquired)\n");
	
	/* Wake the other thread */
	ret = pthread_barrier_wait(&bar);
	ASSERT( (ret == 0) || (ret == PTHREAD_BARRIER_SERIAL_THREAD) );
	
	/* Now wait for the condition, this unlocks the mutex */
	do {
		printf("thread waiting cond\n");
		ASSERT( pthread_cond_wait(&cnd, &mtx) == 0);
		printf("thread woken\n");
	} while (1);
	
	/* Cleanup, never reached */
	pthread_cleanup_pop(1);
	return NULL;
}

int main(int argc, char * argv[])
{
	int ret;
	pthread_t thr;
	void * dummy;
	
	/* initialize the barrier */
	ASSERT( pthread_barrier_init(&bar, NULL, 2) == 0 );
	
	printf("Creating thread\n");
	
	/* Create the thread */
	ASSERT( pthread_create(&thr, NULL, mythread, NULL) == 0 );
	
	printf("main synchronization\n");
	ret = pthread_barrier_wait(&bar);
	ASSERT( (ret == 0) || (ret == PTHREAD_BARRIER_SERIAL_THREAD) );
	
	ASSERT( pthread_mutex_lock(&mtx) == 0 );
	printf("main: thread is now waiting for condvar\n");
	
	/* Cancel the thread */
	ASSERT( pthread_cancel(thr) == 0 );
	
	/* Now, unlock, so that the thread can actually really exit */
	ASSERT( pthread_mutex_unlock(&mtx) == 0 );
	
	/* Release thread resources */
	ASSERT( pthread_join(thr, &dummy) == 0 );
	
	if (called == 1)
		printf("Test successful!\n");
	else
		printf("Test failed! Cleanup was not called (& lock not released)\n");
	
	return 0;
	
}
