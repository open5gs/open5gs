#define TRACE_MODULE _mme_fd

#include "fd_lib.h"

#include "mme_context.h"

int mme_fd_init(void)
{
    fd_context_init(FD_MODE_CLIENT);

    if (mme_self()->fd_conf_path == NULL)
    {
        /* This is default diameter configuration if there is no config file 
         * The Configuration : No TLS, Only TCP */

        fd_self()->cnf_diamid = MME_IDENTITY;
        fd_self()->cnf_diamrlm = FD_REALM;
        fd_self()->cnf_addr = mme_self()->mme_s6a_addr;
        fd_self()->cnf_port = mme_self()->mme_s6a_port;
        fd_self()->cnf_port_tls = mme_self()->mme_s6a_tls_port;

        fd_self()->pi_diamid = HSS_IDENTITY;
        fd_self()->pi_addr = mme_self()->hss_s6a_addr;
        fd_self()->pic_port = mme_self()->hss_s6a_port;
    }

    CHECK_FCT( fd_init(mme_self()->fd_conf_path) );

	return 0;
}

void mme_fd_final(void)
{
    fd_final();
}
