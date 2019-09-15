#include "ogs-sctp.h"
#include "ogs-app.h"

int app_initialize(char **argv)
{
    int rv;

    ogs_sctp_init(ogs_config()->usrsctp.udp_port);
    rv = mme_initialize();
    if (rv != OGS_OK) {
        ogs_error("Failed to intialize MME");
        return rv;
    }
    ogs_info("MME initialize...done");

    return OGS_OK;
}

void app_terminate(void)
{
    mme_terminate();
    ogs_sctp_final();
    ogs_info("MME terminate...done");
}

