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

#include "core_debug.h"

#include "cellwire.h"
#include "context.h"
#include "abts.h"
#include "testutil.h"

void core_assert_ok(abts_case* tc, const char* context, status_t rv,
                        int lineno)
{
    if (rv == CORE_ENOTIMPL)
    {
        abts_not_impl(tc, context, lineno);
    } else if (rv != CORE_OK)
    {
        char buf[STRING_MAX], ebuf[128];
        sprintf(buf, "%s (%d): %s\n", context, rv,
                core_strerror(rv, ebuf, sizeof ebuf));
        abts_fail(tc, buf, lineno);
    }
}

void test_terminate(void)
{
    d_log_set_level(D_MSG_TO_STDOUT, D_LOG_LEVEL_FULL);
    cellwire_terminate();
}

void test_initialize(void)
{
    cellwire_initialize(NULL, NULL);
    d_log_set_level(D_MSG_TO_STDOUT, D_LOG_LEVEL_ERROR);

    inet_pton(AF_INET, "127.0.0.1", &mme_self()->enb_local_addr);

    atexit(test_terminate);
}

