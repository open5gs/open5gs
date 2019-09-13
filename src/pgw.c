#include "ogs-app.h"

int app_initialize(char **argv)
{
    int rv;

    rv = pgw_initialize();
    if (rv != OGS_OK) {
        ogs_error("Failed to intialize PGW");
        return rv;
    }
    ogs_info("PGW initialize...done");

    return OGS_OK;
}

void app_terminate(void)
{
    pgw_terminate();
    ogs_info("PGW terminate...done");
}
