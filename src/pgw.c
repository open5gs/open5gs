#include "app/context.h"
#include "app/application.h"

#include "app-init.h"

extern int __pgw_log_domain;

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __pgw_log_domain

int app_initialize(char **argv)
{
    int rv;

    rv = app_will_initialize(argv);
    if (rv != OGS_OK) return rv;

    rv = pgw_initialize();
    if (rv != OGS_OK)
    {
        ogs_error("Failed to intialize PGW");
        return rv;
    }
    ogs_info("PGW initialize...done");

    rv = app_did_initialize();
    if (rv != OGS_OK) return rv;

    return OGS_OK;
}

void app_terminate(void)
{
    app_will_terminate();

    ogs_info("PGW try to terminate");
    pgw_terminate();
    ogs_info("PGW terminate...done");

    app_did_terminate();
}
