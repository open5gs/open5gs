#include "fd/fd-lib.h"

#include "pcrf_context.h"
#include "pcrf_fd_path.h"

int pcrf_fd_init(void)
{
    int rv;
    int ret = fd_init(FD_MODE_CLIENT|FD_MODE_SERVER,
                pcrf_self()->fd_conf_path, pcrf_self()->fd_config);
    ogs_assert(ret == 0);

    rv = pcrf_gx_init();
    ogs_assert(rv == OGS_OK);
    rv = pcrf_rx_init();
    ogs_assert(rv == OGS_OK);

	return OGS_OK;
}

void pcrf_fd_final(void)
{
    pcrf_gx_final();
    pcrf_rx_final();

    fd_final();
}
