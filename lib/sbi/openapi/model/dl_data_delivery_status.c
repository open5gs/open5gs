
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dl_data_delivery_status.h"

OpenAPI_dl_data_delivery_status_t *OpenAPI_dl_data_delivery_status_create(
    )
{
    OpenAPI_dl_data_delivery_status_t *dl_data_delivery_status_local_var = OpenAPI_malloc(sizeof(OpenAPI_dl_data_delivery_status_t));
    if (!dl_data_delivery_status_local_var) {
        return NULL;
    }

    return dl_data_delivery_status_local_var;
}

void OpenAPI_dl_data_delivery_status_free(OpenAPI_dl_data_delivery_status_t *dl_data_delivery_status)
{
    if (NULL == dl_data_delivery_status) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(dl_data_delivery_status);
}

cJSON *OpenAPI_dl_data_delivery_status_convertToJSON(OpenAPI_dl_data_delivery_status_t *dl_data_delivery_status)
{
    cJSON *item = NULL;

    if (dl_data_delivery_status == NULL) {
        ogs_error("OpenAPI_dl_data_delivery_status_convertToJSON() failed [DlDataDeliveryStatus]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_dl_data_delivery_status_t *OpenAPI_dl_data_delivery_status_parseFromJSON(cJSON *dl_data_delivery_statusJSON)
{
    OpenAPI_dl_data_delivery_status_t *dl_data_delivery_status_local_var = NULL;
    dl_data_delivery_status_local_var = OpenAPI_dl_data_delivery_status_create (
        );

    return dl_data_delivery_status_local_var;
end:
    return NULL;
}

