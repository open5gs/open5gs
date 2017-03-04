#define TRACE_MODULE _hss_init

#include "hss_ctx.h"
#include "hss_sm.h"

#include "s6a_app.h"

int hss_init(void)
{
    s6a_app_init(MODE_HSS);

    hss_ctx_init();
    hss_sm_init();

	return 0;
}

void hss_final(void)
{
    hss_sm_init();
    hss_ctx_final();

    s6a_app_final();
	
	return;
}
