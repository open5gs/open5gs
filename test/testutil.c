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
#include "fd/fd_lib.h"

#include "app/app.h"
#include "app/context.h"
#include "testpacket.h"
#include "abts.h"
#include "testutil.h"

const struct testlist alltests[] = {
    {test_s1ap_message},
    {test_nas_message},
    {test_gtp_message},
    {test_security},
    {test_s1setup},
    {test_attach},
    {test_volte},
    {test_handover},
    {NULL},
};

static int connected_count = 0;
static void test_fd_logger_handler(enum fd_hook_type type, struct msg * msg, 
    struct peer_hdr * peer, void * other, struct fd_hook_permsgdata *pmd, 
    void * regdata)
{
    if (type == HOOK_PEER_CONNECT_SUCCESS)
    {
        connected_count++;
    }
}

void test_terminate(void)
{
    testpacket_final();
    app_terminate();

    d_trace_global_on();
    core_terminate();
}

status_t test_initialize(int argc, const char *const argv[], char *config_path)
{
    status_t rv;

    fd_logger_register(test_fd_logger_handler);

    atexit(test_terminate);

    core_initialize();
    rv = app_initialize(config_path, NULL);
    if (rv != CORE_OK)
    {
        d_error("app_initialize() failed");
        return CORE_ERROR;
    }
    rv = testpacket_init();
    if (rv != CORE_OK)
    {
        d_error("testpacket() failed");
        return CORE_ERROR;
    }

    while(1)
    {
        if (connected_count == 1) break;
        core_sleep(time_from_msec(50));
    }

    return rv;
}

