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
#include "abts.h"

#ifndef __APR_TEST_UTIL__
#define __APR_TEST_UTIL__

/* XXX: FIXME - these all should become much more utilitarian
 * and part of core, itself
 */

#ifdef WIN32
#ifdef BINPATH
#define TESTBINPATH APR_STRINGIFY(BINPATH) "/"
#else
#define TESTBINPATH ""
#endif
#else
#define TESTBINPATH "./"
#endif

#ifdef WIN32
#define EXTENSION ".exe"
#elif NETWARE
#define EXTENSION ".nlm"
#else
#define EXTENSION
#endif

#define STRING_MAX 8096

/* Some simple functions to make the test apps easier to write and
 * a bit more consistent...
 */

/* Assert that RV is an CORE_OK value; else fail giving strerror
 * for RV and CONTEXT message. */
void core_assert_ok(abts_case* tc, const char *context,
                        status_t rv, int lineno);
#define CORE_ASSERT_OK(tc, ctxt, rv) \
             core_assert_ok(tc, ctxt, rv, __LINE__)

void test_initialize(void);

abts_suite *testds(abts_suite *suite);
abts_suite *testfsm(abts_suite *suite);
abts_suite *testtlv(abts_suite *suite);
abts_suite *testaes(abts_suite *suite);
abts_suite *testsha2(abts_suite *suite);
abts_suite *testsock(abts_suite *suite);
abts_suite *testsctp(abts_suite *suite);
abts_suite *testtime(abts_suite *suite);
abts_suite *testtimer(abts_suite *suite);
abts_suite *testthread(abts_suite *suite);
abts_suite *testlock(abts_suite *suite);
abts_suite *testatomic(abts_suite *suite);
abts_suite *testfile(abts_suite *suite);
abts_suite *testfilecopy(abts_suite *suite);
abts_suite *testdir(abts_suite *suite);
abts_suite *testmsgq(abts_suite *suite);
abts_suite *testsleep(abts_suite *suite);
abts_suite *testpkbuf(abts_suite *suite);
abts_suite *testmisc(abts_suite *suite);
abts_suite *testhash(abts_suite *suite);
abts_suite *test3gpp(abts_suite *suite);

#endif /* CORE_TEST_INCLUDES */
