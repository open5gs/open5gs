#define TRACE_MODULE _pcrf_init

#include "pcrf_context.h"
#include "pcrf_fd_path.h"

static int initialized = 0;

status_t pcrf_initialize(void)
{
    status_t rv;

    rv = pcrf_context_init();
    if (rv != CORE_OK) return rv;

    rv = pcrf_context_parse_config();
    if (rv != CORE_OK) return rv;

    rv = pcrf_context_setup_trace_module();
    if (rv != CORE_OK) return rv;

    rv = pcrf_db_init();
    if (rv != CORE_OK) return rv;

    rv = pcrf_fd_init();
    if (rv != CORE_OK) return CORE_ERROR;

    initialized = 1;

	return CORE_OK;
}

void pcrf_terminate(void)
{
    if (!initialized) return;

    pcrf_fd_final();

    pcrf_db_final();
    pcrf_context_final();
	
	return;
}
