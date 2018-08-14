#define TRACE_MODULE _testapp

#include "core_general.h"
#include "core_debug.h"
#include "core_semaphore.h"

#include "common/context.h"
#include "common/application.h"

#include "app_init.h"

static semaphore_id pcrf_sem1 = 0;
static semaphore_id pcrf_sem2 = 0;

static semaphore_id pgw_sem1 = 0;
static semaphore_id pgw_sem2 = 0;

static semaphore_id sgw_sem1 = 0;
static semaphore_id sgw_sem2 = 0;

static semaphore_id hss_sem1 = 0;
static semaphore_id hss_sem2 = 0;

static semaphore_id mme_sem1 = 0;
static semaphore_id mme_sem2 = 0;

status_t test_app_initialize(
        const char *config_path, const char *log_path, const char *pid_path)
{
    pid_t pid;
    status_t rv;
    int app = 0;

    rv = app_will_initialize(config_path, log_path);
    if (rv != CORE_OK) return rv;

    app = context_self()->logger.trace.app;
    if (app)
    {
        d_trace_level(&_testapp, app);
    }


    /************************* PCRF Process **********************/
    semaphore_create(&pcrf_sem1, 0); /* copied to PCRF/PGW/SGW/HSS process */
    semaphore_create(&pcrf_sem2, 0); /* copied to PCRF/PGW/SGW/HSS process */

    if (context_self()->parameter.no_pcrf == 0)
    {
        pid = fork();
        d_assert(pid >= 0, _exit(EXIT_FAILURE), "fork() failed");

        if (pid == 0)
        {
            d_trace(1, "PCRF try to initialize\n");
            rv = pcrf_initialize();
            d_assert(rv == CORE_OK,, "Failed to intialize PCRF");
            d_trace(1, "PCRF initialize...done\n");

            if (pcrf_sem1) semaphore_post(pcrf_sem1);
            if (pcrf_sem2) semaphore_wait(pcrf_sem2);

            if (rv == CORE_OK)
            {
                d_trace(1, "PCRF try to terminate\n");
                pcrf_terminate();
                d_trace(1, "PCRF terminate...done\n");
            }

            if (pcrf_sem1) semaphore_post(pcrf_sem1);

            /* allocated from parent process */
            if (pcrf_sem1) semaphore_delete(pcrf_sem1);
            if (pcrf_sem2) semaphore_delete(pcrf_sem2);

            app_did_terminate();

            core_terminate();

            _exit(EXIT_SUCCESS);
        }

        if (pcrf_sem1) semaphore_wait(pcrf_sem1);
    }


    /************************* PGW Process **********************/

    semaphore_create(&pgw_sem1, 0); /* copied to PGW/SGW/HSS process */
    semaphore_create(&pgw_sem2, 0); /* copied to PGW/SGW/HSS process */

    if (context_self()->parameter.no_pgw == 0)
    {
        pid = fork();
        d_assert(pid >= 0, _exit(EXIT_FAILURE), "fork() failed");

        if (pid == 0)
        {
            /* allocated from parent process */
            if (pcrf_sem1) semaphore_delete(pcrf_sem1);
            if (pcrf_sem2) semaphore_delete(pcrf_sem2);

            d_trace(1, "PGW try to initialize\n");
            rv = pgw_initialize();
            d_assert(rv == CORE_OK,, "Failed to intialize PGW");
            d_trace(1, "PGW initialize...done\n");

            if (pgw_sem1) semaphore_post(pgw_sem1);
            if (pgw_sem2) semaphore_wait(pgw_sem2);

            if (rv == CORE_OK)
            {
                d_trace(1, "PGW try to terminate\n");
                pgw_terminate();
                d_trace(1, "PGW terminate...done\n");
            }

            if (pgw_sem1) semaphore_post(pgw_sem1);

            /* allocated from parent process */
            if (pgw_sem1) semaphore_delete(pgw_sem1);
            if (pgw_sem2) semaphore_delete(pgw_sem2);

            app_did_terminate();

            core_terminate();

            _exit(EXIT_SUCCESS);
        }

        if (pgw_sem1) semaphore_wait(pgw_sem1);
    }


    /************************* SGW Process **********************/

    semaphore_create(&sgw_sem1, 0); /* copied to SGW/HSS process */
    semaphore_create(&sgw_sem2, 0); /* copied to SGW/HSS process */

    if (context_self()->parameter.no_sgw == 0)
    {
        pid = fork();
        d_assert(pid >= 0, _exit(EXIT_FAILURE), "fork() failed");

        if (pid == 0)
        {
            /* allocated from parent process */
            if (pcrf_sem1) semaphore_delete(pcrf_sem1);
            if (pcrf_sem2) semaphore_delete(pcrf_sem2);
            if (pgw_sem1) semaphore_delete(pgw_sem1);
            if (pgw_sem2) semaphore_delete(pgw_sem2);

            d_trace(1, "SGW try to initialize\n");
            rv = sgw_initialize();
            d_assert(rv == CORE_OK,, "Failed to intialize SGW");
            d_trace(1, "SGW initialize...done\n");

            if (sgw_sem1) semaphore_post(sgw_sem1);
            if (sgw_sem2) semaphore_wait(sgw_sem2);

            if (rv == CORE_OK)
            {
                d_trace(1, "SGW try to terminate\n");
                sgw_terminate();
                d_trace(1, "SGW terminate...done\n");
            }

            if (sgw_sem1) semaphore_post(sgw_sem1);

            /* allocated from parent process */
            if (sgw_sem1) semaphore_delete(sgw_sem1);
            if (sgw_sem2) semaphore_delete(sgw_sem2);

            app_did_terminate();

            core_terminate();

            _exit(EXIT_SUCCESS);
        }

        if (sgw_sem1) semaphore_wait(sgw_sem1);
    }


    /************************* HSS Process **********************/

    semaphore_create(&hss_sem1, 0); /* copied to HSS process */
    semaphore_create(&hss_sem2, 0); /* copied to HSS process */

    if (context_self()->parameter.no_hss == 0)
    {
        pid = fork();
        d_assert(pid >= 0, _exit(EXIT_FAILURE), "fork() failed");

        if (pid == 0)
        {
            /* allocated from parent process */
            if (pcrf_sem1) semaphore_delete(pcrf_sem1);
            if (pcrf_sem2) semaphore_delete(pcrf_sem2);
            if (pgw_sem1) semaphore_delete(pgw_sem1);
            if (pgw_sem2) semaphore_delete(pgw_sem2);
            if (sgw_sem1) semaphore_delete(sgw_sem1);
            if (sgw_sem2) semaphore_delete(sgw_sem2);

            d_trace(1, "HSS try to initialize\n");
            rv = hss_initialize();
            d_assert(rv == CORE_OK,, "Failed to intialize HSS");
            d_trace(1, "HSS initialize...done\n");

            if (hss_sem1) semaphore_post(hss_sem1);
            if (hss_sem2) semaphore_wait(hss_sem2);

            if (rv == CORE_OK)
            {
                d_trace(1, "HSS try to terminate\n");
                hss_terminate();
                d_trace(1, "HSS terminate...done\n");
            }

            if (hss_sem1) semaphore_post(hss_sem1);

            if (hss_sem1) semaphore_delete(hss_sem1);
            if (hss_sem2) semaphore_delete(hss_sem2);

            app_did_terminate();

            core_terminate();

            _exit(EXIT_SUCCESS);
        }

        if (hss_sem1) semaphore_wait(hss_sem1);
    }

    /************************* MME Process **********************/

    semaphore_create(&mme_sem1, 0); /* copied to MME process */
    semaphore_create(&mme_sem2, 0); /* copied to MME process */

/*    if (context_self()->parameter.no_mme == 0) */
    {
        pid = fork();
        d_assert(pid >= 0, _exit(EXIT_FAILURE), "fork() failed");

        if (pid == 0)
        {
            /* allocated from parent process */
            if (pcrf_sem1) semaphore_delete(pcrf_sem1);
            if (pcrf_sem2) semaphore_delete(pcrf_sem2);
            if (pgw_sem1) semaphore_delete(pgw_sem1);
            if (pgw_sem2) semaphore_delete(pgw_sem2);
            if (sgw_sem1) semaphore_delete(sgw_sem1);
            if (sgw_sem2) semaphore_delete(sgw_sem2);
            if (hss_sem1) semaphore_delete(hss_sem1);
            if (hss_sem2) semaphore_delete(hss_sem2);

            d_trace(1, "MME try to initialize\n");
            rv = mme_initialize();
            d_assert(rv == CORE_OK,, "Failed to intialize MME");
            d_trace(1, "MME initialize...done\n");

            if (mme_sem1) semaphore_post(mme_sem1);
            if (mme_sem2) semaphore_wait(mme_sem2);

            if (rv == CORE_OK)
            {
                d_trace(1, "MME try to terminate\n");
                mme_terminate();
                d_trace(1, "MME terminate...done\n");
            }

            if (mme_sem1) semaphore_post(mme_sem1);

            if (mme_sem1) semaphore_delete(mme_sem1);
            if (mme_sem2) semaphore_delete(mme_sem2);

            app_did_terminate();

            core_terminate();

            _exit(EXIT_SUCCESS);
        }

        if (mme_sem1) semaphore_wait(mme_sem1);
    }

    rv = app_did_initialize();
    if (rv != CORE_OK) return rv;

    return CORE_OK;;
}

void test_app_terminate(void)
{
    app_will_terminate();

    /* if (context_self()->parameter.no_mme == 0) */
    {
        if (mme_sem2) semaphore_post(mme_sem2);
        if (mme_sem1) semaphore_wait(mme_sem1);
    }
    if (mme_sem1) semaphore_delete(mme_sem1);
    if (mme_sem2) semaphore_delete(mme_sem2);

    if (context_self()->parameter.no_hss == 0)
    {
        if (hss_sem2) semaphore_post(hss_sem2);
        if (hss_sem1) semaphore_wait(hss_sem1);
    }
    if (hss_sem1) semaphore_delete(hss_sem1);
    if (hss_sem2) semaphore_delete(hss_sem2);

    if (context_self()->parameter.no_sgw == 0)
    {
        if (sgw_sem2) semaphore_post(sgw_sem2);
        if (sgw_sem1) semaphore_wait(sgw_sem1);
    }
    if (sgw_sem1) semaphore_delete(sgw_sem1);
    if (sgw_sem2) semaphore_delete(sgw_sem2);

    if (context_self()->parameter.no_pgw == 0)
    {
        if (pgw_sem2) semaphore_post(pgw_sem2);
        if (pgw_sem1) semaphore_wait(pgw_sem1);
    }
    if (pgw_sem1) semaphore_delete(pgw_sem1);
    if (pgw_sem2) semaphore_delete(pgw_sem2);

    if (context_self()->parameter.no_pcrf == 0)
    {
        if (pcrf_sem2) semaphore_post(pcrf_sem2);
        if (pcrf_sem1) semaphore_wait(pcrf_sem1);
    }
    if (pcrf_sem1) semaphore_delete(pcrf_sem1);
    if (pcrf_sem2) semaphore_delete(pcrf_sem2);

    app_did_terminate();
}
