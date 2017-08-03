#define TRACE_MODULE _epc_main

#include "core_debug.h"
#include "core_signal.h"
#include "core_semaphore.h"
#include "core_proc.h"

#include "context.h"
#include "app.h"

static proc_id pgw_proc = 0;
static status_t PROC_FUNC pgw_start_func(proc_id id, void *data);
static status_t PROC_FUNC pgw_stop_func(proc_id id, void *data);

static proc_id sgw_proc = 0;
static status_t PROC_FUNC sgw_start_func(proc_id id, void *data);
static status_t PROC_FUNC sgw_stop_func(proc_id id, void *data);

static proc_id hss_proc = 0;
static status_t PROC_FUNC hss_start_func(proc_id id, void *data);
static status_t PROC_FUNC hss_stop_func(proc_id id, void *data);

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

    if (context_self()->hidden.disable_pgw == 0)
    {
        rv = proc_create(&pgw_proc, pgw_start_func, pgw_stop_func, NULL);
        if (rv != CORE_OK) return rv;
    }

    if (context_self()->hidden.disable_sgw == 0)
    {
        rv = proc_create(&sgw_proc, sgw_start_func, sgw_stop_func, NULL);
        if (rv != CORE_OK) return rv;
    }

    if (context_self()->hidden.disable_hss == 0)
    {
        rv = proc_create(&hss_proc, hss_start_func, hss_stop_func, NULL);
        if (rv != CORE_OK) return rv;
    }

    d_trace(1, "MME try to initialize\n");
    rv = mme_initialize();
    d_assert(rv == CORE_OK, return rv, "Failed to intialize MME");
    d_trace(1, "MME initialize...done\n");

    rv = app_did_initialize(config_path, log_path);
    if (rv != CORE_OK) return rv;

    return CORE_OK;;
}

void app_terminate(void)
{
    app_will_terminate();

    d_trace(1, "MME try to terminate\n");
    mme_terminate();
    d_trace(1, "MME terminate...done\n");

    if (hss_proc)
        proc_delete(hss_proc);
    if (sgw_proc)
        proc_delete(sgw_proc);
    if (pgw_proc)
        proc_delete(pgw_proc);

    app_did_terminate();
}

static status_t PROC_FUNC pgw_start_func(proc_id id, void *data)
{
    status_t rv;

    d_trace(1, "PGW try to initialize\n");
    rv = pgw_initialize();
    d_assert(rv == CORE_OK, return rv, "Failed to intialize PGW");
    d_trace(1, "PGW initialize...done\n");

    return CORE_OK;
}

static status_t PROC_FUNC pgw_stop_func(proc_id id, void *data)
{
    d_trace(1, "PGW try to terminate\n");
    pgw_terminate();
    d_trace(1, "PGW terminate...done\n");

    return CORE_OK;
}

static status_t PROC_FUNC sgw_start_func(proc_id id, void *data)
{
    status_t rv;

    d_trace(1, "SGW try to initialize\n");
    rv = sgw_initialize();
    d_assert(rv == CORE_OK, return rv, "Failed to intialize SGW");
    d_trace(1, "SGW initialize...done\n");

    return CORE_OK;
}

static status_t PROC_FUNC sgw_stop_func(proc_id id, void *data)
{
    d_trace(1, "SGW try to terminate\n");
    sgw_terminate();
    d_trace(1, "SGW terminate...done\n");

    return CORE_OK;
}

static status_t PROC_FUNC hss_start_func(proc_id id, void *data)
{
    status_t rv;

    d_trace(1, "HSS try to initialize\n");
    rv = hss_initialize();
    d_assert(rv == CORE_OK, return rv, "Failed to intialize HSS");
    d_trace(1, "HSS initialize...done\n");

    return CORE_OK;
}

static status_t PROC_FUNC hss_stop_func(proc_id id, void *data)
{
    d_trace(1, "HSS try to terminate\n");
    hss_terminate();
    d_trace(1, "HSS terminate...done\n");

    return CORE_OK;
}
