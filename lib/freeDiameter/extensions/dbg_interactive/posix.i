/*********************************************************************************************************
* Software License Agreement (BSD License)                                                               *
* Author: Sebastien Decugis <sdecugis@freediameter.net>							 *
*													 *
* Copyright (c) 2012, WIDE Project and NICT								 *
* All rights reserved.											 *
* 													 *
* Redistribution and use of this software in source and binary forms, with or without modification, are  *
* permitted provided that the following conditions are met:						 *
* 													 *
* * Redistributions of source code must retain the above 						 *
*   copyright notice, this list of conditions and the 							 *
*   following disclaimer.										 *
*    													 *
* * Redistributions in binary form must reproduce the above 						 *
*   copyright notice, this list of conditions and the 							 *
*   following disclaimer in the documentation and/or other						 *
*   materials provided with the distribution.								 *
* 													 *
* * Neither the name of the WIDE Project or NICT nor the 						 *
*   names of its contributors may be used to endorse or 						 *
*   promote products derived from this software without 						 *
*   specific prior written permission of WIDE Project and 						 *
*   NICT.												 *
* 													 *
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED *
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A *
* PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR *
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 	 *
* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 	 *
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR *
* TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF   *
* ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.								 *
*********************************************************************************************************/

/* Do not include this directly, use dbg_interactive.i instead */

/****** POSIX wrappers around useful functions for fD *********/

/**** MUTEX ****/
typedef struct {
} pthread_mutex_t;

%extend pthread_mutex_t {
	pthread_mutex_t() {
		int ret = 0;
		pthread_mutex_t * r = calloc(1, sizeof(pthread_mutex_t));
		if (!r) {
			DI_ERROR_MALLOC;
			return NULL;
		}
		ret = pthread_mutex_init(r, NULL);
		if (ret) {
			DI_ERROR(ret, NULL, NULL);
			free(r);
			return NULL;
		}
		return r;
	}
	~pthread_mutex_t() {
		int ret = 0;
		ret = pthread_mutex_destroy($self);
		if (ret) {
			DI_ERROR(ret, NULL, NULL);
			return;
		}
		free($self);
		return;
	}
	void lock() {
		int ret = pthread_mutex_lock($self);
		if (ret) {
			DI_ERROR(ret, NULL, NULL);
		}
	}
	void unlock() {
		int ret = pthread_mutex_unlock($self);
		if (ret) {
			DI_ERROR(ret, NULL, NULL);
		}
	}
}

/**** CONDVAR ****/
typedef struct {
} pthread_cond_t;

%extend pthread_cond_t {
	pthread_cond_t() {
		int ret = 0;
		pthread_cond_t * r = calloc(1, sizeof(pthread_cond_t));
		if (!r) {
			DI_ERROR_MALLOC;
			return NULL;
		}
		ret = pthread_cond_init(r, NULL);
		if (ret) {
			DI_ERROR(ret, NULL, NULL);
			free(r);
			return NULL;
		}
		return r;
	}
	~pthread_cond_t() {
		int ret = 0;
		ret = pthread_cond_destroy($self);
		if (ret) {
			DI_ERROR(ret, NULL, NULL);
			return;
		}
		free($self);
		return;
	}
	void signal() {
		int ret = pthread_cond_signal($self);
		if (ret) {
			DI_ERROR(ret, NULL, NULL);
		}
	}
	void broadcast() {
		int ret = pthread_cond_broadcast($self);
		if (ret) {
			DI_ERROR(ret, NULL, NULL);
		}
	}
	void wait(pthread_mutex_t * mutex) {
		int ret = pthread_cond_wait($self, mutex);
		if (ret) {
			DI_ERROR(ret, NULL, NULL);
		}
	}
	void timedwait(pthread_mutex_t * mutex, long seconds) {
		struct timespec ts;
		int ret;
		
		clock_gettime(CLOCK_REALTIME, &ts);
		ts.tv_sec += seconds;
		
		ret = pthread_cond_timedwait($self, mutex, &ts);
		if (ret && (ret != ETIMEDOUT)) {
			DI_ERROR(ret, NULL, NULL);
		}
	}
}

/**** RWLOCK ****/
typedef struct {
} pthread_rwlock_t;

%extend pthread_rwlock_t {
	pthread_rwlock_t() {
		int ret = 0;
		pthread_rwlock_t * r = calloc(1, sizeof(pthread_rwlock_t));
		if (!r) {
			DI_ERROR_MALLOC;
			return NULL;
		}
		ret = pthread_rwlock_init(r, NULL);
		if (ret) {
			DI_ERROR(ret, NULL, NULL);
			free(r);
			return NULL;
		}
		return r;
	}
	~pthread_rwlock_t() {
		int ret = 0;
		ret = pthread_rwlock_destroy($self);
		if (ret) {
			DI_ERROR(ret, NULL, NULL);
			return;
		}
		free($self);
		return;
	}
	void rdlock() {
		int ret = pthread_rwlock_rdlock($self);
		if (ret) {
			DI_ERROR(ret, NULL, NULL);
		}
	}
	void wrlock() {
		int ret = pthread_rwlock_wrlock($self);
		if (ret) {
			DI_ERROR(ret, NULL, NULL);
		}
	}
	void unlock() {
		int ret = pthread_rwlock_unlock($self);
		if (ret) {
			DI_ERROR(ret, NULL, NULL);
		}
	}
}
