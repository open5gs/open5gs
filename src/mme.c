#include "mme/ogs-sctp.h"

#include "app/context.h"
#include "app/application.h"

#include "app-init.h"

extern int __mme_log_domain;

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __mme_log_domain

int app_initialize(app_param_t *param)
{
    int rv;

    param->name = "mme";
    rv = app_will_initialize(param);
    if (rv != OGS_OK) return rv;

    ogs_sctp_init(context_self()->config.usrsctp.udp_port);
    rv = mme_initialize();
    if (rv != OGS_OK)
    {
        ogs_error("Failed to intialize MME");
        return rv;
    }
    ogs_info("MME initialize...done");

    rv = app_did_initialize();
    if (rv != OGS_OK) return rv;

    return OGS_OK;
}

void app_terminate(void)
{
    app_will_terminate();

    ogs_info("MME try to terminate");
    mme_terminate();
    ogs_sctp_final();
    ogs_info("MME terminate...done");

    app_did_terminate();
}

