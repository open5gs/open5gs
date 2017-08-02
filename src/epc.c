#define TRACE_MODULE _epc_main

#include "core_debug.h"
#include "core_signal.h"
#include "core_semaphore.h"
#include "core_proc.h"

#include "context.h"
#include "app.h"

static proc_id pgw_proc;
static semaphore_id pgw_sem;
static void *PROC_FUNC pgw_main(proc_id id, void *data);

static proc_id sgw_proc;
static semaphore_id sgw_sem;
static void *PROC_FUNC sgw_main(proc_id id, void *data);

static proc_id hss_proc;
static semaphore_id hss_sem;
static void *PROC_FUNC hss_main(proc_id id, void *data);

status_t app_initialize(char *config_path, char *log_path)
{
    status_t rv;
    int others = 0;

    rv = app_will_initialize(config_path, log_path);
    if (rv != CORE_OK) return rv;

    others = context_self()->trace_level.others;
    if (others)
    {
        d_trace_level(&_epc_main, others);
    }

    d_assert(semaphore_create(&pgw_sem, 0) == CORE_OK, 
            return CORE_ERROR, "semaphore_create() failed");
    rv = proc_create(&pgw_proc, pgw_main, NULL);
    if (rv != CORE_OK) return rv;

    d_assert(semaphore_create(&sgw_sem, 0) == CORE_OK, 
            return CORE_ERROR, "semaphore_create() failed");
    rv = proc_create(&sgw_proc, sgw_main, NULL);
    if (rv != CORE_OK) return rv;

    d_assert(semaphore_create(&hss_sem, 0) == CORE_OK, 
            return CORE_ERROR, "semaphore_create() failed");
    rv = proc_create(&hss_proc, hss_main, NULL);
    if (rv != CORE_OK) return rv;

    d_trace(1, "MME try to initialize\n");
    rv = mme_initialize();
    if (rv != CORE_OK) return rv;
    d_trace(1, "MME initialize...done\n");

    if (app_did_initialize(config_path, log_path) != CORE_OK) rv = CORE_ERROR;

    return rv;;
}

void app_terminate(void)
{
    app_will_terminate();

    d_trace(1, "MME try to terminate\n");
    mme_terminate();
    d_trace(1, "MME terminate...done\n");

    d_assert(semaphore_post(hss_sem) == CORE_OK,,
            "semaphore_post() failed");
    proc_delete(hss_proc);

    d_assert(semaphore_post(sgw_sem) == CORE_OK,,
            "semaphore_post() failed");
    proc_delete(sgw_proc);

    d_assert(semaphore_post(pgw_sem) == CORE_OK,,
            "semaphore_post() failed");
    proc_delete(pgw_proc);

    app_did_terminate();
}

static void *PROC_FUNC pgw_main(proc_id id, void *data)
{
    status_t rv;

    d_trace(1, "PGW try to initialize\n");
    rv = pgw_initialize();
    if (rv != CORE_OK) return NULL;
    d_trace(1, "PGW initialize...done\n");

    d_assert(semaphore_wait(pgw_sem) == CORE_OK, return NULL,
            "semaphore_wait() failed");
    d_assert(semaphore_delete(pgw_sem) == CORE_OK, return NULL,
            "semaphore_delete() failed");

    d_trace(1, "PGW try to terminate\n");
    pgw_terminate();
    d_trace(1, "PGW terminate...done\n");

    return NULL;
}

static void *PROC_FUNC sgw_main(proc_id id, void *data)
{
    status_t rv;

    d_trace(1, "SGW try to initialize\n");
    rv = sgw_initialize();
    if (rv != CORE_OK) return NULL;
    d_trace(1, "SGW initialize...done\n");

    d_assert(semaphore_wait(sgw_sem) == CORE_OK, return NULL,
            "semaphore_wait() failed");
    d_assert(semaphore_delete(sgw_sem) == CORE_OK, return NULL,
            "semaphore_delete() failed");

    d_trace(1, "SGW try to terminate\n");
    sgw_terminate();
    d_trace(1, "SGW terminate...done\n");

    return NULL;
}

static void *PROC_FUNC hss_main(proc_id id, void *data)
{
    status_t rv;

    d_trace(1, "HSS try to initialize\n");
    rv = hss_initialize();
    if (rv != CORE_OK) return NULL;
    d_trace(1, "HSS initialize...done\n");

    d_assert(semaphore_wait(hss_sem) == CORE_OK, return NULL,
            "semaphore_wait() failed");
    d_assert(semaphore_delete(hss_sem) == CORE_OK, return NULL,
            "semaphore_delete() failed");

    d_trace(1, "HSS try to terminate\n");
    hss_terminate();
    d_trace(1, "HSS terminate...done\n");

    return NULL;
}
