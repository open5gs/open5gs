#include "ogs-app.h"

int app_initialize(char **argv)
{
    int rv;

    rv = sgw_initialize();
    if (rv != OGS_OK) {
        ogs_error("Failed to intialize SGW");
        return rv;
    }
    ogs_info("SGW initialize...done");

    return OGS_OK;
}

void app_terminate(void)
{
    sgw_terminate();
    ogs_info("SGW terminate...done");
}
