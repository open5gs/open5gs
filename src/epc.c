#define TRACE_MODULE _epc_main

#include "core_debug.h"
#include "core_signal.h"
#include "core_semaphore.h"
#include "core_proc.h"

#include "context.h"
#include "app.h"

static proc_id pgw_proc;
static void *PROC_FUNC pgw_start_func(proc_id id, void *data);
static void *PROC_FUNC pgw_stop_func(proc_id id, void *data);

static proc_id sgw_proc;
static void *PROC_FUNC sgw_start_func(proc_id id, void *data);
static void *PROC_FUNC sgw_stop_func(proc_id id, void *data);

static proc_id hss_proc;
static void *PROC_FUNC hss_start_func(proc_id id, void *data);
static void *PROC_FUNC hss_stop_func(proc_id id, void *data);

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

    rv = proc_create(&pgw_proc, pgw_start_func, pgw_stop_func, NULL);
    if (rv != CORE_OK) return rv;

    rv = proc_create(&sgw_proc, sgw_start_func, sgw_stop_func, NULL);
    if (rv != CORE_OK) return rv;

    rv = proc_create(&hss_proc, hss_start_func, hss_stop_func, NULL);
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

    proc_delete(hss_proc);
    proc_delete(sgw_proc);
    proc_delete(pgw_proc);

    app_did_terminate();
}

static void *PROC_FUNC pgw_start_func(proc_id id, void *data)
{
    status_t rv;

    d_trace(1, "PGW try to initialize\n");
    rv = pgw_initialize();
    if (rv != CORE_OK) return NULL;
    d_trace(1, "PGW initialize...done\n");

    return NULL;
}

static void *PROC_FUNC pgw_stop_func(proc_id id, void *data)
{
    d_trace(1, "PGW try to terminate\n");
    pgw_terminate();
    d_trace(1, "PGW terminate...done\n");

    return NULL;
}

static void *PROC_FUNC sgw_start_func(proc_id id, void *data)
{
    status_t rv;

    d_trace(1, "SGW try to initialize\n");
    rv = sgw_initialize();
    if (rv != CORE_OK) return NULL;
    d_trace(1, "SGW initialize...done\n");

    return NULL;
}

static void *PROC_FUNC sgw_stop_func(proc_id id, void *data)
{
    d_trace(1, "SGW try to terminate\n");
    sgw_terminate();
    d_trace(1, "SGW terminate...done\n");

    return NULL;
}

static void *PROC_FUNC hss_start_func(proc_id id, void *data)
{
    status_t rv;

    d_trace(1, "HSS try to initialize\n");
    rv = hss_initialize();
    if (rv != CORE_OK) return NULL;
    d_trace(1, "HSS initialize...done\n");

    return NULL;
}

static void *PROC_FUNC hss_stop_func(proc_id id, void *data)
{
    d_trace(1, "HSS try to terminate\n");
    hss_terminate();
    d_trace(1, "HSS terminate...done\n");

    return NULL;
}
