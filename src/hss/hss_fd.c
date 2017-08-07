#define TRACE_MODULE _hss_fd

#include "fd_lib.h"

#include "hss_context.h"

int hss_fd_init(void)
{
    fd_context_init(FD_MODE_SERVER);

    if (hss_self()->fd_conf_path == NULL)
    {
        /* This is default diameter configuration if there is no config file 
         * The Configuration : No TLS, Only TCP */

        fd_self()->cnf_diamid = HSS_IDENTITY;
        fd_self()->cnf_diamrlm = FD_REALM;
        fd_self()->cnf_addr = hss_self()->hss_s6a_addr;
        fd_self()->cnf_port = hss_self()->hss_s6a_port;
        fd_self()->cnf_port_tls = hss_self()->hss_s6a_tls_port;

        fd_self()->pi_diamid = MME_IDENTITY;
        fd_self()->pi_addr = hss_self()->mme_s6a_addr;
        fd_self()->pic_port = hss_self()->mme_s6a_port;
    }

    CHECK_FCT( fd_init(hss_self()->fd_conf_path) );

	return 0;
}

void hss_fd_final(void)
{
    fd_final();
}
