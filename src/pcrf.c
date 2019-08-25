#include "app/context.h"
#include "app/application.h"

#include "app-init.h"

extern int __pcrf_log_domain;

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __pcrf_log_domain

int app_initialize(char **argv)
{
    int rv;

    rv = app_will_initialize(argv);
    if (rv != OGS_OK) return rv;

    rv = pcrf_initialize();
    if (rv != OGS_OK)
    {
        ogs_error("Failed to intialize PCRF");
        return rv;
    }
    ogs_info("PCRF initialize...done");

    rv = app_did_initialize();
    if (rv != OGS_OK) return rv;

    return OGS_OK;
}

void app_terminate(void)
{
    app_will_terminate();

    ogs_info("PCRF try to terminate");
    pcrf_terminate();
    ogs_info("PCRF terminate...done");

    app_did_terminate();
}
