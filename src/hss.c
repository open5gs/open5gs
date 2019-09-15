#include "ogs-app.h"

int app_initialize(char **argv)
{
    int rv;

    rv = hss_initialize();
    if (rv != OGS_OK) {
        ogs_error("Failed to intialize HSS");
        return rv;
    }
    ogs_info("HSS initialize...done");

    return OGS_OK;
}

void app_terminate(void)
{
    hss_terminate();
    ogs_info("HSS terminate...done");
}
