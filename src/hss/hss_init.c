#include "app/context.h"

#include "hss_context.h"
#include "hss_fd_path.h"

static int initialized = 0;

int hss_initialize(void)
{
    int rv;

    rv = hss_context_init();
    if (rv != OGS_OK) return rv;

    rv = hss_context_parse_config();
    if (rv != OGS_OK) return rv;

    rv = context_setup_log_module();
    if (rv != OGS_OK) return rv;

    rv = hss_db_init();
    if (rv != OGS_OK) return rv;

    rv = hss_fd_init();
    if (rv != OGS_OK) return OGS_ERROR;

    initialized = 1;

	return OGS_OK;
}

void hss_terminate(void)
{
    if (!initialized) return;

    hss_fd_final();

    hss_db_final();
    hss_context_final();
	
	return;
}
