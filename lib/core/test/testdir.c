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

#include "core_file.h"
#include "testutil.h"

static void test_mkdir(abts_case *tc, void *data)
{
    status_t rv;
    file_info_t finfo;

    rv = dir_make("data/testdir", FILE_UREAD | FILE_UWRITE | FILE_UEXECUTE);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = file_stat(&finfo, "data/testdir", FILE_INFO_TYPE);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    ABTS_INT_EQUAL(tc, FILE_DIR, finfo.filetype);
}

static void test_mkdir_recurs(abts_case *tc, void *data)
{
    status_t rv;
    file_info_t finfo;

    rv = dir_make_recursive("data/one/two/three",
                                FILE_UREAD | FILE_UWRITE | FILE_UEXECUTE);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = file_stat(&finfo, "data/one", FILE_INFO_TYPE);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    ABTS_INT_EQUAL(tc, FILE_DIR, finfo.filetype);

    rv = file_stat(&finfo, "data/one/two", FILE_INFO_TYPE);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    ABTS_INT_EQUAL(tc, FILE_DIR, finfo.filetype);

    rv = file_stat(&finfo, "data/one/two/three", FILE_INFO_TYPE);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    ABTS_INT_EQUAL(tc, FILE_DIR, finfo.filetype);
}

static void test_remove(abts_case *tc, void *data)
{
    status_t rv;
    file_info_t finfo;

    rv = dir_remove("data/testdir");
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = file_stat(&finfo, "data/testdir", FILE_INFO_TYPE);
    ABTS_INT_EQUAL(tc, 1, STATUS_IS_ENOENT(rv));
}

static void test_removeall_fail(abts_case *tc, void *data)
{
    status_t rv;

    rv = dir_remove("data/one");
    ABTS_INT_EQUAL(tc, 1, STATUS_IS_ENOTEMPTY(rv));
}

static void test_removeall(abts_case *tc, void *data)
{
    status_t rv;

    rv = dir_remove("data/one/two/three");
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = dir_remove("data/one/two");
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = dir_remove("data/one");
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
}

static void test_remove_notthere(abts_case *tc, void *data)
{
    status_t rv;

    rv = dir_remove("data/notthere");
    ABTS_INT_EQUAL(tc, 1, STATUS_IS_ENOENT(rv));
}

static void test_mkdir_twice(abts_case *tc, void *data)
{
    status_t rv;

    rv = dir_make("data/testdir", FILE_UREAD | FILE_UWRITE | FILE_UEXECUTE);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = dir_make("data/testdir", FILE_UREAD | FILE_UWRITE | FILE_UEXECUTE);
    ABTS_INT_EQUAL(tc, 1, STATUS_IS_EEXIST(rv));

    rv = dir_remove("data/testdir");
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
}

abts_suite *testdir(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, test_mkdir, NULL);
    abts_run_test(suite, test_mkdir_recurs, NULL);
    abts_run_test(suite, test_remove, NULL);
    abts_run_test(suite, test_removeall_fail, NULL);
    abts_run_test(suite, test_removeall, NULL);
    abts_run_test(suite, test_remove_notthere, NULL);
    abts_run_test(suite, test_mkdir_twice, NULL);

    return suite;
}
