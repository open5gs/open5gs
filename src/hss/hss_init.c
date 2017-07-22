#define TRACE_MODULE _hss_init

#include "s6a_lib.h"
#include "hss_context.h"
#include "hss_s6a_handler.h"

status_t hss_initialize(void)
{
    status_t rv;
    int ret;

    ret = s6a_init(MODE_HSS);
    if (ret != 0) return CORE_ERROR;

    rv = hss_context_init();
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
    s6a_final();
	
	return;
}
