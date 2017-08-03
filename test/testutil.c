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

#include "core_general.h"
#include "core_debug.h"
#include "core_semaphore.h"
#include "s6a_lib.h"

#include "app.h"
#include "mme_context.h"
#include "abts.h"
#include "testutil.h"

static semaphore_id test_sem;
static void test_s6a_hook_handler(enum fd_hook_type type, struct msg * msg, 
    struct peer_hdr * peer, void * other, struct fd_hook_permsgdata *pmd, 
    void * regdata)
{
    if (type == HOOK_PEER_CONNECT_SUCCESS)
    {
        d_assert(semaphore_post(test_sem) == CORE_OK,,
                "semaphore_post() failed");
    }
}

void test_terminate(void)
{
    d_trace_global_on();

    app_terminate();
    core_terminate();
}

status_t test_initialize(void)
{
    status_t rv;

    s6a_hook_register(test_s6a_hook_handler);

    atexit(test_terminate);

    core_initialize();
    d_assert(semaphore_create(&test_sem, 0) == CORE_OK, 
            return CORE_ERROR, "semaphore_create() failed");

    rv = app_initialize(NULL, NULL);
    if (rv == CORE_OK)
    {
        d_assert(semaphore_wait(test_sem) == CORE_OK, return CORE_ERROR,
                "semaphore_wait() failed");
        d_assert(semaphore_wait(test_sem) == CORE_OK, return CORE_ERROR,
                "semaphore_wait() failed");
        d_assert(semaphore_delete(test_sem) == CORE_OK, return CORE_ERROR,
                "semaphore_delete() failed");
    }

    return rv;
}

