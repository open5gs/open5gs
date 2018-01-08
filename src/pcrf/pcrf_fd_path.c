#define TRACE_MODULE _pcrf_fd_path

#include "core_debug.h"

#include "fd/fd_lib.h"

#include "pcrf_context.h"
#include "pcrf_fd_path.h"

int pcrf_fd_init(void)
{
    CHECK_FCT( fd_init(FD_MODE_CLIENT|FD_MODE_SERVER,
                pcrf_self()->fd_conf_path, pcrf_self()->fd_config) );

    CHECK_FCT( pcrf_gx_init() );
    CHECK_FCT( pcrf_rx_init() );

	return 0;
}

void pcrf_fd_final(void)
{
    pcrf_gx_final();
    pcrf_rx_final();

    fd_final();
}
