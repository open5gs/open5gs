#include "app/context.h"
#include "app/application.h"

#include "app_init.h"

extern int __hss_log_domain;

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __hss_log_domain

int app_initialize(app_param_t *param)
{
    int rv;

    param->name = "hss";
    rv = app_will_initialize(param);
    if (rv != OGS_OK) return rv;

    rv = hss_initialize();
    if (rv != OGS_OK)
    {
        ogs_error("Failed to intialize HSS");
        return rv;
    }
    ogs_info("HSS initialize...done");

    rv = app_did_initialize();
    if (rv != OGS_OK) return rv;

    return OGS_OK;
}

void app_terminate(void)
{
    app_will_terminate();

    ogs_info("HSS try to terminate");
    hss_terminate();
    ogs_info("HSS terminate...done");

    app_did_terminate();
}
