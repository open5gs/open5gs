/* Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* This header file is where you should put ANY platform specific information.
 * This should be the only header file that programs need to include that 
 * actually has platform dependent code which refers to the .
 */
#ifndef __CORE_PORTABLE_H__
#define __CORE_PORTABLE_H__

#include "core_thread.h"
#include "core_network.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef int                   os_file_t;        /**< native file */
typedef pthread_t             os_thread_t;      /**< native thread */
typedef pid_t                 os_proc_t;        /**< native pid */
typedef int                   os_sock_t;        /**< native socket */

/**
 * convert the thread to os specific type from core type.
 * @param thethd The core thread to convert
 * @param thd The os specific thread we are converting to
 */
CORE_DECLARE(status_t) os_thread_get(os_thread_t **thethd, thread_id id);

/**
 * Get the thread ID
 */
CORE_DECLARE(os_thread_t) os_thread_current(void);


/**
 * Convert the socket from an core type to an OS specific socket
 * @param thesock The socket to convert.
 * @param sock The os specific equivalent of the core socket..
 */
CORE_DECLARE(status_t) os_sock_get(os_sock_t *thesock, sock_id id);

#ifdef __cplusplus
}
#endif

#endif  /* __CORE_PORTABLE_H__ */
