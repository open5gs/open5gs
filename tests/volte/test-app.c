#include "mme/ogs-sctp.h"

#include "app/context.h"
#include "app/application.h"

#include "app-init.h"

static ogs_proc_mutex_t *pcrf_sem1 = NULL;
static ogs_proc_mutex_t *pcrf_sem2 = NULL;

static ogs_proc_mutex_t *pgw_sem1 = NULL;
static ogs_proc_mutex_t *pgw_sem2 = NULL;

static ogs_proc_mutex_t *sgw_sem1 = NULL;
static ogs_proc_mutex_t *sgw_sem2 = NULL;

static ogs_proc_mutex_t *hss_sem1 = NULL;
static ogs_proc_mutex_t *hss_sem2 = NULL;

static ogs_proc_mutex_t *mme_sem1 = NULL;
static ogs_proc_mutex_t *mme_sem2 = NULL;

int test_app_initialize(app_param_t *param)
{
    pid_t pid;
    int rv;
    int app = 0;

    rv = app_will_initialize(param);
    if (rv != OGS_OK) return rv;

    /************************* PCRF Process **********************/
    pcrf_sem1 = ogs_proc_mutex_create(0); /* copied to PCRF/PGW/SGW/HSS/MME */
    pcrf_sem2 = ogs_proc_mutex_create(0); /* copied to PCRF/PGW/SGW/HSS/MME */

    if (context_self()->config.parameter.no_pcrf == 0)
    {
        pid = fork();
        ogs_assert(pid >= 0);

        if (pid == 0)
        {
            ogs_info("PCRF try to initialize");
            rv = pcrf_initialize();
            ogs_assert(rv == OGS_OK);
            ogs_info("PCRF initialize...done");

            if (pcrf_sem1) ogs_proc_mutex_post(pcrf_sem1);
            if (pcrf_sem2) ogs_proc_mutex_wait(pcrf_sem2);

            if (rv == OGS_OK)
            {
                ogs_info("PCRF try to terminate");
                pcrf_terminate();
                ogs_info("PCRF terminate...done");
            }

            if (pcrf_sem1) ogs_proc_mutex_post(pcrf_sem1);

            /* allocated from parent process */
            if (pcrf_sem1) ogs_proc_mutex_delete(pcrf_sem1);
            if (pcrf_sem2) ogs_proc_mutex_delete(pcrf_sem2);

            context_final();
            ogs_core_finalize();

            _exit(EXIT_SUCCESS);
        }

        if (pcrf_sem1) ogs_proc_mutex_wait(pcrf_sem1);
    }


    /************************* PGW Process **********************/
    pgw_sem1 = ogs_proc_mutex_create(0); /* copied to PGW/SGW/HSS/MME */
    pgw_sem2 = ogs_proc_mutex_create(0); /* copied to PGW/SGW/HSS/MME */

    if (context_self()->config.parameter.no_pgw == 0)
    {
        pid = fork();
        ogs_assert(pid >= 0);

        if (pid == 0)
        {
            /* allocated from parent process */
            if (pcrf_sem1) ogs_proc_mutex_delete(pcrf_sem1);
            if (pcrf_sem2) ogs_proc_mutex_delete(pcrf_sem2);

            ogs_info("PGW try to initialize");
            rv = pgw_initialize();
            ogs_assert(rv == OGS_OK);
            ogs_info("PGW initialize...done");

            if (pgw_sem1) ogs_proc_mutex_post(pgw_sem1);
            if (pgw_sem2) ogs_proc_mutex_wait(pgw_sem2);

            if (rv == OGS_OK)
            {
                ogs_info("PGW try to terminate");
                pgw_terminate();
                ogs_info("PGW terminate...done");
            }

            if (pgw_sem1) ogs_proc_mutex_post(pgw_sem1);

            /* allocated from parent process */
            if (pgw_sem1) ogs_proc_mutex_delete(pgw_sem1);
            if (pgw_sem2) ogs_proc_mutex_delete(pgw_sem2);

            context_final();
            ogs_core_finalize();

            _exit(EXIT_SUCCESS);
        }

        if (pgw_sem1) ogs_proc_mutex_wait(pgw_sem1);
    }


    /************************* SGW Process **********************/
    sgw_sem1 = ogs_proc_mutex_create(0); /* copied to SGW/HSS/MME */
    sgw_sem2 = ogs_proc_mutex_create(0); /* copied to SGW/HSS/MME */

    if (context_self()->config.parameter.no_sgw == 0)
    {
        pid = fork();
        ogs_assert(pid >= 0);

        if (pid == 0)
        {
            /* allocated from parent process */
            if (pcrf_sem1) ogs_proc_mutex_delete(pcrf_sem1);
            if (pcrf_sem2) ogs_proc_mutex_delete(pcrf_sem2);
            if (pgw_sem1) ogs_proc_mutex_delete(pgw_sem1);
            if (pgw_sem2) ogs_proc_mutex_delete(pgw_sem2);

            ogs_info("SGW try to initialize");
            rv = sgw_initialize();
            ogs_assert(rv == OGS_OK);
            ogs_info("SGW initialize...done");

            if (sgw_sem1) ogs_proc_mutex_post(sgw_sem1);
            if (sgw_sem2) ogs_proc_mutex_wait(sgw_sem2);

            if (rv == OGS_OK)
            {
                ogs_info("SGW try to terminate");
                sgw_terminate();
                ogs_info("SGW terminate...done");
            }

            if (sgw_sem1) ogs_proc_mutex_post(sgw_sem1);

            /* allocated from parent process */
            if (sgw_sem1) ogs_proc_mutex_delete(sgw_sem1);
            if (sgw_sem2) ogs_proc_mutex_delete(sgw_sem2);

            context_final();
            ogs_core_finalize();

            _exit(EXIT_SUCCESS);
        }

        if (sgw_sem1) ogs_proc_mutex_wait(sgw_sem1);
    }


    /************************* HSS Process **********************/
    hss_sem1 = ogs_proc_mutex_create(0); /* copied to HSS/MME */
    hss_sem2 = ogs_proc_mutex_create(0); /* copied to HSS/MME */

    if (context_self()->config.parameter.no_hss == 0)
    {
        pid = fork();
        ogs_assert(pid >= 0);

        if (pid == 0)
        {
            /* allocated from parent process */
            if (pcrf_sem1) ogs_proc_mutex_delete(pcrf_sem1);
            if (pcrf_sem2) ogs_proc_mutex_delete(pcrf_sem2);
            if (pgw_sem1) ogs_proc_mutex_delete(pgw_sem1);
            if (pgw_sem2) ogs_proc_mutex_delete(pgw_sem2);
            if (sgw_sem1) ogs_proc_mutex_delete(sgw_sem1);
            if (sgw_sem2) ogs_proc_mutex_delete(sgw_sem2);

            ogs_info("HSS try to initialize");
            rv = hss_initialize();
            ogs_assert(rv == OGS_OK);
            ogs_info("HSS initialize...done");

            if (hss_sem1) ogs_proc_mutex_post(hss_sem1);
            if (hss_sem2) ogs_proc_mutex_wait(hss_sem2);

            if (rv == OGS_OK)
            {
                ogs_info("HSS try to terminate");
                hss_terminate();
                ogs_info("HSS terminate...done");
            }

            if (hss_sem1) ogs_proc_mutex_post(hss_sem1);

            if (hss_sem1) ogs_proc_mutex_delete(hss_sem1);
            if (hss_sem2) ogs_proc_mutex_delete(hss_sem2);

            context_final();
            ogs_core_finalize();

            _exit(EXIT_SUCCESS);
        }

        if (hss_sem1) ogs_proc_mutex_wait(hss_sem1);
    }

    /************************* MME Process **********************/
    mme_sem1 = ogs_proc_mutex_create(0); /* copied to MME */
    mme_sem2 = ogs_proc_mutex_create(0); /* copied to MME */

/*    if (context_self()->config.parameter.no_mme == 0) */
    {
        pid = fork();
        ogs_assert(pid >= 0);

        if (pid == 0)
        {
            /* allocated from parent process */
            if (pcrf_sem1) ogs_proc_mutex_delete(pcrf_sem1);
            if (pcrf_sem2) ogs_proc_mutex_delete(pcrf_sem2);
            if (pgw_sem1) ogs_proc_mutex_delete(pgw_sem1);
            if (pgw_sem2) ogs_proc_mutex_delete(pgw_sem2);
            if (sgw_sem1) ogs_proc_mutex_delete(sgw_sem1);
            if (sgw_sem2) ogs_proc_mutex_delete(sgw_sem2);
            if (hss_sem1) ogs_proc_mutex_delete(hss_sem1);
            if (hss_sem2) ogs_proc_mutex_delete(hss_sem2);

            ogs_info("MME try to initialize");
            ogs_sctp_init(context_self()->config.usrsctp.udp_port);
            rv = mme_initialize();
            ogs_assert(rv == OGS_OK);
            ogs_info("MME initialize...done");

            if (mme_sem1) ogs_proc_mutex_post(mme_sem1);
            if (mme_sem2) ogs_proc_mutex_wait(mme_sem2);

            if (rv == OGS_OK)
            {
                ogs_info("MME try to terminate");
                mme_terminate();
                ogs_sctp_final();
                ogs_info("MME terminate...done");
            }

            if (mme_sem1) ogs_proc_mutex_post(mme_sem1);

            if (mme_sem1) ogs_proc_mutex_delete(mme_sem1);
            if (mme_sem2) ogs_proc_mutex_delete(mme_sem2);

            context_final();
            ogs_core_finalize();

            _exit(EXIT_SUCCESS);
        }

        if (mme_sem1) ogs_proc_mutex_wait(mme_sem1);
    }

    rv = app_did_initialize();
    if (rv != OGS_OK) return rv;

    return OGS_OK;;
}

void test_app_terminate(void)
{
    app_will_terminate();

    /* if (context_self()->config.parameter.no_mme == 0) */
    {
        if (mme_sem2) ogs_proc_mutex_post(mme_sem2);
        if (mme_sem1) ogs_proc_mutex_wait(mme_sem1);
    }
    if (mme_sem1) ogs_proc_mutex_delete(mme_sem1);
    if (mme_sem2) ogs_proc_mutex_delete(mme_sem2);

    if (context_self()->config.parameter.no_hss == 0)
    {
        if (hss_sem2) ogs_proc_mutex_post(hss_sem2);
        if (hss_sem1) ogs_proc_mutex_wait(hss_sem1);
    }
    if (hss_sem1) ogs_proc_mutex_delete(hss_sem1);
    if (hss_sem2) ogs_proc_mutex_delete(hss_sem2);

    if (context_self()->config.parameter.no_sgw == 0)
    {
        if (sgw_sem2) ogs_proc_mutex_post(sgw_sem2);
        if (sgw_sem1) ogs_proc_mutex_wait(sgw_sem1);
    }
    if (sgw_sem1) ogs_proc_mutex_delete(sgw_sem1);
    if (sgw_sem2) ogs_proc_mutex_delete(sgw_sem2);

    if (context_self()->config.parameter.no_pgw == 0)
    {
        if (pgw_sem2) ogs_proc_mutex_post(pgw_sem2);
        if (pgw_sem1) ogs_proc_mutex_wait(pgw_sem1);
    }
    if (pgw_sem1) ogs_proc_mutex_delete(pgw_sem1);
    if (pgw_sem2) ogs_proc_mutex_delete(pgw_sem2);

    if (context_self()->config.parameter.no_pcrf == 0)
    {
        if (pcrf_sem2) ogs_proc_mutex_post(pcrf_sem2);
        if (pcrf_sem1) ogs_proc_mutex_wait(pcrf_sem1);
    }
    if (pcrf_sem1) ogs_proc_mutex_delete(pcrf_sem1);
    if (pcrf_sem2) ogs_proc_mutex_delete(pcrf_sem2);

    app_did_terminate();
}
