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

/*
 * abts.c
 *
 * This code is taken from the APR library.
 * Modified by Sukchan Lee to fit Open5GS coding style.
 */
#include "abts.h"
#if 0 /* modified by acetcom */
#include "abts_tests.h"
#include "testutil.h"
#else
#include "ogs-core.h"
#endif

#define ABTS_STAT_SIZE 6
static char status[ABTS_STAT_SIZE] = {'|', '/', '-', '|', '\\', '-'};
static int curr_char;
#if 0 /* modified by acetcom */
static int verbose = 0;
#else
static int verbose = 1;
#endif
static int exclude = 0;
static int quiet = 0;
static int list_tests = 0;

const char **testlist = NULL;

static int find_test_name(const char *testname) {
    int i;
    for (i = 0; testlist[i] != NULL; i++) {
        if (!strcmp(testlist[i], testname)) {
            return 1;
        }
    }
    return 0;
}

/* Determine if the test should be run at all */
static int should_test_run(const char *testname) {
    int found = 0;
    if (list_tests == 1) {
        return 0;
    }
    if (testlist == NULL) {
        return 1;
    }
    found = find_test_name(testname);
    if ((found && !exclude) || (!found && exclude)) {
        return 1;
    }
    return 0;
}

static void reset_status(void)
{
    curr_char = 0;
}

static void update_status(void)
{
    if (!quiet) {
        curr_char = (curr_char + 1) % ABTS_STAT_SIZE;
        fprintf(stdout, "\b%c", status[curr_char]);
        fflush(stdout);
    }
}

static void end_suite(abts_suite *suite)
{
    if (suite != NULL) {
        sub_suite *last = suite->tail;
        if (!quiet) {
            fprintf(stdout, "\b");
            fflush(stdout);
        }
        if (last->failed == 0) {
            fprintf(stdout, "SUCCESS\n");
            fflush(stdout);
        }
        else {
            fprintf(stdout, "FAILED %d of %d\n", last->failed, last->num_test);
            fflush(stdout);
        }
    }
}

abts_suite *abts_add_suite(abts_suite *suite, const char *suite_name_full)
{
    sub_suite *subsuite;
    char *p;
    const char *suite_name;
    curr_char = 0;
    
    /* Only end the suite if we actually ran it */
    if (suite && suite->tail &&!suite->tail->not_run) {
        end_suite(suite);
    }

    subsuite = malloc(sizeof(*subsuite));
    subsuite->num_test = 0;
    subsuite->failed = 0;
    subsuite->next = NULL;
    /* suite_name_full may be an absolute path depending on __FILE__ 
     * expansion */
    suite_name = strrchr(suite_name_full, '/');
    if (!suite_name) {
        suite_name = strrchr(suite_name_full, '\\');
    }
    if (suite_name) {
        suite_name++;
    } else {
        suite_name = suite_name_full;
    }
    p = strrchr(suite_name, '.');
    if (p) {
        subsuite->name = memcpy(calloc(p - suite_name + 1, 1),
                                suite_name, p - suite_name);
    }
    else {
#if 0 /* modified by acetcom */
        subsuite->name = suite_name;
#else
        subsuite->name = memcpy(calloc(strlen(suite_name) + 1, 1),
                                suite_name, strlen(suite_name));
#endif
    }

    if (list_tests) {
        fprintf(stdout, "%s\n", subsuite->name);
    }
    
    subsuite->not_run = 0;

    if (suite == NULL) {
        suite = malloc(sizeof(*suite));
        suite->head = subsuite;
        suite->tail = subsuite;
    }
    else {
        suite->tail->next = subsuite;
        suite->tail = subsuite;
    }

    if (!should_test_run(subsuite->name)) {
        subsuite->not_run = 1;
        return suite;
    }

    reset_status();
    fprintf(stdout, "%-20s:  ", subsuite->name);
    update_status();
    fflush(stdout);

    return suite;
}

void abts_run_test(abts_suite *ts, test_func f, void *value)
{
    abts_case tc;
    sub_suite *ss;

    if (!should_test_run(ts->tail->name)) {
        return;
    }
    ss = ts->tail;

    tc.failed = 0;
    tc.suite = ss;
    
    ss->num_test++;
    update_status();

    f(&tc, value);
    
    if (tc.failed) {
        ss->failed++;
    }
}

static int report(abts_suite *suite)
{
    int count = 0;
    sub_suite *dptr;

    if (suite && suite->tail &&!suite->tail->not_run) {
        end_suite(suite);
    }

    for (dptr = suite->head; dptr; dptr = dptr->next) {
        count += dptr->failed;
    }

    if (list_tests) {
        return 0;
    }

    if (count == 0) {
        printf("All tests passed.\n");
        return 0;
    }

    dptr = suite->head;
    fprintf(stdout, "%-15s\t\tTotal\tFail\tFailed %%\n", "Failed Tests");
    fprintf(stdout, "===================================================\n");
    while (dptr != NULL) {
        if (dptr->failed != 0) {
            float percent = ((float)dptr->failed / (float)dptr->num_test);
            fprintf(stdout, "%-15s\t\t%5d\t%4d\t%6.2f%%\n", dptr->name, 
                    dptr->num_test, dptr->failed, percent * 100);
        }
        dptr = dptr->next;
    }
    return 1;
}

void abts_log_message(const char *fmt, ...)
{
    va_list args;
    update_status();

    if (verbose) {
        va_start(args, fmt);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-nonliteral"
        vfprintf(stderr, fmt, args);
#pragma GCC diagnostic pop
        va_end(args);
        fprintf(stderr, "\n");
        fflush(stderr);
    }
}

void abts_int_equal(abts_case *tc, const int expected, const int actual, int lineno)
{
    update_status();
    if (tc->failed) return;

    if (expected == actual) return;

    tc->failed = TRUE;
    if (verbose) {
        fprintf(stderr, "Line %d: expected <%d>, but saw <%d>\n", lineno, expected, actual);
        fflush(stderr);
    }
}

void abts_int_nequal(abts_case *tc, const int expected, const int actual, int lineno)
{
    update_status();
    if (tc->failed) return;

    if (expected != actual) return;

    tc->failed = TRUE;
    if (verbose) {
        fprintf(stderr, "Line %d: expected something other than <%d>, but saw <%d>\n",
                lineno, expected, actual);
        fflush(stderr);
    }
}

void abts_size_equal(abts_case *tc, size_t expected, size_t actual, int lineno)
{
    update_status();
    if (tc->failed) return;

    if (expected == actual) return;

    tc->failed = TRUE;
    if (verbose) {
        /* Note that the comparison is type-exact, reporting must be a best-fit */
        fprintf(stderr, "Line %d: expected %lu, but saw %lu\n", lineno, 
                (unsigned long)expected, (unsigned long)actual);
        fflush(stderr);
    }
}

void abts_str_equal(abts_case *tc, const char *expected, const char *actual, int lineno)
{
    update_status();
    if (tc->failed) return;

    if (!expected && !actual) return;
    if (expected && actual)
        if (!strcmp(expected, actual)) return;

    tc->failed = TRUE;
    if (verbose) {
        fprintf(stderr, "Line %d: expected <%s>, but saw <%s>\n", lineno, expected, actual);
        fflush(stderr);
    }
}

void abts_str_nequal(abts_case *tc, const char *expected, const char *actual,
                       size_t n, int lineno)
{
    update_status();
    if (tc->failed) return;

    if (!strncmp(expected, actual, n)) return;

    tc->failed = TRUE;
    if (verbose) {
        fprintf(stderr, "Line %d: expected something other than <%s>, but saw <%s>\n",
                lineno, expected, actual);
        fflush(stderr);
    }
}

void abts_ptr_notnull(abts_case *tc, const void *ptr, int lineno)
{
    update_status();
    if (tc->failed) return;

    if (ptr != NULL) return;

    tc->failed = TRUE;
    if (verbose) {
        fprintf(stderr, "Line %d: expected non-NULL, but saw NULL\n", lineno);
        fflush(stderr);
    }
}
 
void abts_ptr_equal(abts_case *tc, const void *expected, const void *actual, int lineno)
{
    update_status();
    if (tc->failed) return;

    if (expected == actual) return;

    tc->failed = TRUE;
    if (verbose) {
        fprintf(stderr, "Line %d: expected <%p>, but saw <%p>\n", lineno, expected, actual);
        fflush(stderr);
    }
}

void abts_fail(abts_case *tc, const char *message, int lineno)
{
    update_status();
    if (tc->failed) return;

    tc->failed = TRUE;
    if (verbose) {
        fprintf(stderr, "Line %d: %s\n", lineno, message);
        fflush(stderr);
    }
}
 
void abts_assert(abts_case *tc, const char *message, int condition, int lineno)
{
    update_status();
    if (tc->failed) return;

    if (condition) return;

    tc->failed = TRUE;
    if (verbose) {
        fprintf(stderr, "Line %d: %s\n", lineno, message);
        fflush(stderr);
    }
}

void abts_true(abts_case *tc, int condition, int lineno)
{
    update_status();
    if (tc->failed) return;

    if (condition) return;

    tc->failed = TRUE;
    if (verbose) {
        fprintf(stderr, "Line %d: Condition is false, but expected true\n", lineno);
        fflush(stderr);
    }
}

void abts_not_impl(abts_case *tc, const char *message, int lineno)
{
    update_status();

    tc->suite->not_impl++;
    if (verbose) {
        fprintf(stderr, "Line %d: %s\n", lineno, message);
        fflush(stderr);
    }
}

#if 0 /* modified by acetcom */
int main(int argc, const char *const argv[]) {
#else
void abts_init(int argc, const char *const argv[]) {
#endif
    int i;
#if 0 /* modified by acetcom */
    int rv;
    int list_provided = 0;
    abts_suite *suite = NULL;
   
    initialize();
#else
    int list_provided = 0;
#endif

    quiet = !isatty(STDOUT_FILENO);

    for (i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-v")) {
            verbose = 1;
            continue;
        }
        if (!strcmp(argv[i], "-x")) {
            exclude = 1;
            continue;
        }
        if (!strcmp(argv[i], "-l")) {
            list_tests = 1;
            continue;
        }
        if (!strcmp(argv[i], "-q")) {
            quiet = 1;
            continue;
        }
#if 1 /* modified by acetcom */
        if (!strcmp(argv[i], "-f")) {
            i++;
            continue;
        }
        if (!strcmp(argv[i], "-d")) {
            i++;
            continue;
        }
        if (!strcmp(argv[i], "-t")) {
            i++;
            continue;
        }
#endif
        if (argv[i][0] == '-') {
#if 0 /* modified by acetcom */
            fprintf(stderr, "Invalid option: `%s'\n", argv[i]);
            exit(1);
#else
            continue;
#endif
        }
        list_provided = 1;
    }

    if (list_provided) {
        /* Waste a little space here, because it is easier than counting the
         * number of tests listed.  Besides it is at most three char *.
         */
        testlist = calloc(argc + 1, sizeof(char *));
        for (i = 1; i < argc; i++) {
            testlist[i - 1] = argv[i];
        }
    }

#if 0 /* modified by acetcom */
    for (i = 0; i < (sizeof(alltests) / sizeof(struct testlist *)); i++) {
        suite = alltests[i].func(suite);
    }

    rv = report(suite);
    return rv;
#endif
}
       
#if 1 /* modified by acetcom */
static void show_help(const char *name)
{
    printf("Usage: %s [options] [test1] [test2] ...\n"
        "Options:\n"
       "   -c filename    : set configuration file\n"
       "   -e level       : set global log-level (default:info)\n"
       "   -m domain      : set log-domain (e.g. mme:sgw:gtp)\n"
       "   -d             : print lots of debugging information\n"
       "   -t             : print tracing information for developer\n"
       "   -v             : show version number and exit\n"
       "   -h             : show this message and exit\n"
       "   -v             : turn on verbose in test\n"
       "   -q             : turn off status in test\n"
       "   -x             : exclute test-unit (e.g. -x sctp-test)\n"
       "   -l             : list test-unit\n"
       "\n", name);
}

int abts_main(int argc, const char *const argv[], const char **argv_out)
{
    char *arg;
    int i, opt;
    ogs_getopt_t options;
    struct {
        char *config_file;
        char *log_level;
        char *domain_mask;

        bool enable_debug;
        bool enable_trace;
    } optarg;

    memset(&optarg, 0, sizeof(optarg));

    ogs_getopt_init(&options, (char**)argv);
    while ((opt = ogs_getopt(&options, "hvxlqc:e:m:dt")) != -1) {
        switch (opt) {
        case 'h':
            show_help(argv[0]);
            break;
        case 'v':
            verbose = 1;
            break;
        case 'x':
            exclude = 1;
            break;
        case 'l':
            list_tests = 1;
            break;
        case 'q':
            quiet = 1;
            break;
        case 'c':
            optarg.config_file = options.optarg;
            break;
        case 'e':
            optarg.log_level = options.optarg;
            break;
        case 'm':
            optarg.domain_mask = options.optarg;
            break;
        case 'd':
            optarg.enable_debug = true;
            break;
        case 't':
            optarg.enable_trace = true;
            break;
        case '?':
            fprintf(stderr, "%s: %s\n", argv[0], options.errmsg);
            show_help(argv[0]);
            return OGS_ERROR;
        default:
            fprintf(stderr, "%s: should not be reached\n", OGS_FUNC);
            exit(1);
        }
    }

    i = 0;
    while((arg = ogs_getopt_arg(&options))) {
        if (!testlist)
            testlist = calloc(argc + 1, sizeof(char *));
        testlist[i++] = arg;
    }

    if (optarg.enable_debug) optarg.log_level = (char*)"debug";
    if (optarg.enable_trace) optarg.log_level = (char*)"trace";

    i = 0;
    argv_out[i++] = argv[0];

    argv_out[i++] = "-e";
    if (!optarg.log_level) 
        argv_out[i++] = "error"; /* Default LOG Level : ERROR */
    else 
        argv_out[i++] = optarg.log_level;

    if (optarg.config_file) {
        argv_out[i++] = "-c";
        argv_out[i++] = optarg.config_file;
    }
    if (optarg.domain_mask) {
        argv_out[i++] = "-m";
        argv_out[i++] = optarg.domain_mask;
    }

    argv_out[i] = NULL;
    
    return OGS_OK;
}

static void abts_free(abts_suite *suite)
{
    sub_suite *ptr = NULL, *next_ptr = NULL;

    ptr = suite->head;
    while (ptr != NULL) {
        next_ptr = ptr->next;

        free((void*)ptr->name);
        free(ptr);
        ptr = next_ptr;
    }

    free(suite);
}

int abts_report(abts_suite *suite)
{
    int rv = report(suite);

    abts_free(suite);
    free(testlist);

    return rv;
}
#endif
