#define TRACE_MODULE _pcrf_fd_path

#include "core_debug.h"

#include "fd/fd_lib.h"

#include "pcrf_context.h"
#include "pcrf_fd_path.h"

status_t pcrf_fd_init(void)
{
    status_t rv;
    int ret = fd_init(FD_MODE_CLIENT|FD_MODE_SERVER,
                pcrf_self()->fd_conf_path, pcrf_self()->fd_config);
    d_assert(ret == 0, return CORE_ERROR,);

    rv = pcrf_gx_init();
    d_assert(rv == CORE_OK, return CORE_ERROR,);
    rv = pcrf_rx_init();
    d_assert(rv == CORE_OK, return CORE_ERROR,);

	return CORE_OK;
}

void pcrf_fd_final(void)
{
    pcrf_gx_final();
    pcrf_rx_final();

    fd_final();
}
