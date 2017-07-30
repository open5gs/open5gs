#define TRACE_MODULE _hss_init

#include "hss_context.h"
#include "hss_s6a_handler.h"

status_t hss_initialize(void)
{
    status_t rv;

    rv = hss_context_init();
    if (rv != CORE_OK) return rv;

    rv = hss_context_parse_config();
    if (rv != CORE_OK) return rv;

    rv = hss_db_init();
    if (rv != CORE_OK) return rv;

    rv = hss_s6a_init();
    if (rv != CORE_OK) return rv;

	return CORE_OK;
}

void hss_terminate(void)
{
    hss_s6a_final();
    hss_db_final();
    hss_context_final();
	
	return;
}
