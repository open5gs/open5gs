#include "ogs-app.h"

int app_initialize(const char *const argv[])
{
    int rv;

    rv = aanf_initialize();
    if (rv != OGS_OK) {
        ogs_error("Failed to initialize AANF");
        return rv;
    }
    ogs_info("AANF initialize...done");

    return OGS_OK;
}

void app_terminate(void)
{
    aanf_terminate();
    ogs_info("AANF terminate...done");
}
