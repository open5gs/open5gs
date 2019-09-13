#include "ogs-app.h"

int app_initialize(char **argv)
{
    int rv;

    rv = pcrf_initialize();
    if (rv != OGS_OK) {
        ogs_error("Failed to intialize PCRF");
        return rv;
    }
    ogs_info("PCRF initialize...done");

    return OGS_OK;
}

void app_terminate(void)
{
    pcrf_terminate();
    ogs_info("PCRF terminate...done");
}
