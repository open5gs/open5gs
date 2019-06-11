#include "app/context.h"
#include "app/application.h"

#include "app-init.h"

extern int __sgw_log_domain;

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __sgw_log_domain

int app_initialize(app_param_t *param)
{
    int rv;

    param->name = "sgw";
    rv = app_will_initialize(param);
    if (rv != OGS_OK) return rv;

    rv = sgw_initialize();
    if (rv != OGS_OK)
    {
        ogs_error("Failed to intialize SGW");
        return rv;
    }
    ogs_info("SGW initialize...done");

    rv = app_did_initialize();
    if (rv != OGS_OK) return rv;

    return OGS_OK;
}

void app_terminate(void)
{
    app_will_terminate();

    ogs_info("SGW try to terminate");
    sgw_terminate();
    ogs_info("SGW terminate...done");

    app_did_terminate();
}
