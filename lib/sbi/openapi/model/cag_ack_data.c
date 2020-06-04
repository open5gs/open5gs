
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cag_ack_data.h"

OpenAPI_cag_ack_data_t *OpenAPI_cag_ack_data_create(
    char *provisioning_time,
    OpenAPI_ue_update_status_e ue_update_status
    )
{
    OpenAPI_cag_ack_data_t *cag_ack_data_local_var = OpenAPI_malloc(sizeof(OpenAPI_cag_ack_data_t));
    if (!cag_ack_data_local_var) {
        return NULL;
    }
    cag_ack_data_local_var->provisioning_time = provisioning_time;
    cag_ack_data_local_var->ue_update_status = ue_update_status;

    return cag_ack_data_local_var;
}

void OpenAPI_cag_ack_data_free(OpenAPI_cag_ack_data_t *cag_ack_data)
{
    if (NULL == cag_ack_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(cag_ack_data->provisioning_time);
    ogs_free(cag_ack_data);
}

cJSON *OpenAPI_cag_ack_data_convertToJSON(OpenAPI_cag_ack_data_t *cag_ack_data)
{
    cJSON *item = NULL;

    if (cag_ack_data == NULL) {
        ogs_error("OpenAPI_cag_ack_data_convertToJSON() failed [CagAckData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!cag_ack_data->provisioning_time) {
        ogs_error("OpenAPI_cag_ack_data_convertToJSON() failed [provisioning_time]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "provisioningTime", cag_ack_data->provisioning_time) == NULL) {
        ogs_error("OpenAPI_cag_ack_data_convertToJSON() failed [provisioning_time]");
        goto end;
    }

    if (!cag_ack_data->ue_update_status) {
        ogs_error("OpenAPI_cag_ack_data_convertToJSON() failed [ue_update_status]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "ueUpdateStatus", OpenAPI_ue_update_status_ToString(cag_ack_data->ue_update_status)) == NULL) {
        ogs_error("OpenAPI_cag_ack_data_convertToJSON() failed [ue_update_status]");
        goto end;
    }

end:
    return item;
}

OpenAPI_cag_ack_data_t *OpenAPI_cag_ack_data_parseFromJSON(cJSON *cag_ack_dataJSON)
{
    OpenAPI_cag_ack_data_t *cag_ack_data_local_var = NULL;
    cJSON *provisioning_time = cJSON_GetObjectItemCaseSensitive(cag_ack_dataJSON, "provisioningTime");
    if (!provisioning_time) {
        ogs_error("OpenAPI_cag_ack_data_parseFromJSON() failed [provisioning_time]");
        goto end;
    }


    if (!cJSON_IsString(provisioning_time)) {
        ogs_error("OpenAPI_cag_ack_data_parseFromJSON() failed [provisioning_time]");
        goto end;
    }

    cJSON *ue_update_status = cJSON_GetObjectItemCaseSensitive(cag_ack_dataJSON, "ueUpdateStatus");
    if (!ue_update_status) {
        ogs_error("OpenAPI_cag_ack_data_parseFromJSON() failed [ue_update_status]");
        goto end;
    }

    OpenAPI_ue_update_status_e ue_update_statusVariable;

    if (!cJSON_IsString(ue_update_status)) {
        ogs_error("OpenAPI_cag_ack_data_parseFromJSON() failed [ue_update_status]");
        goto end;
    }
    ue_update_statusVariable = OpenAPI_ue_update_status_FromString(ue_update_status->valuestring);

    cag_ack_data_local_var = OpenAPI_cag_ack_data_create (
        ogs_strdup(provisioning_time->valuestring),
        ue_update_statusVariable
        );

    return cag_ack_data_local_var;
end:
    return NULL;
}

