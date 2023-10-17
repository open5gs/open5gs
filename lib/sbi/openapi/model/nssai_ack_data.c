
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nssai_ack_data.h"

OpenAPI_nssai_ack_data_t *OpenAPI_nssai_ack_data_create(
    char *provisioning_time,
    OpenAPI_ue_update_status_e ue_update_status
)
{
    OpenAPI_nssai_ack_data_t *nssai_ack_data_local_var = ogs_malloc(sizeof(OpenAPI_nssai_ack_data_t));
    ogs_assert(nssai_ack_data_local_var);

    nssai_ack_data_local_var->provisioning_time = provisioning_time;
    nssai_ack_data_local_var->ue_update_status = ue_update_status;

    return nssai_ack_data_local_var;
}

void OpenAPI_nssai_ack_data_free(OpenAPI_nssai_ack_data_t *nssai_ack_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == nssai_ack_data) {
        return;
    }
    if (nssai_ack_data->provisioning_time) {
        ogs_free(nssai_ack_data->provisioning_time);
        nssai_ack_data->provisioning_time = NULL;
    }
    ogs_free(nssai_ack_data);
}

cJSON *OpenAPI_nssai_ack_data_convertToJSON(OpenAPI_nssai_ack_data_t *nssai_ack_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (nssai_ack_data == NULL) {
        ogs_error("OpenAPI_nssai_ack_data_convertToJSON() failed [NssaiAckData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!nssai_ack_data->provisioning_time) {
        ogs_error("OpenAPI_nssai_ack_data_convertToJSON() failed [provisioning_time]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "provisioningTime", nssai_ack_data->provisioning_time) == NULL) {
        ogs_error("OpenAPI_nssai_ack_data_convertToJSON() failed [provisioning_time]");
        goto end;
    }

    if (nssai_ack_data->ue_update_status == OpenAPI_ue_update_status_NULL) {
        ogs_error("OpenAPI_nssai_ack_data_convertToJSON() failed [ue_update_status]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "ueUpdateStatus", OpenAPI_ue_update_status_ToString(nssai_ack_data->ue_update_status)) == NULL) {
        ogs_error("OpenAPI_nssai_ack_data_convertToJSON() failed [ue_update_status]");
        goto end;
    }

end:
    return item;
}

OpenAPI_nssai_ack_data_t *OpenAPI_nssai_ack_data_parseFromJSON(cJSON *nssai_ack_dataJSON)
{
    OpenAPI_nssai_ack_data_t *nssai_ack_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *provisioning_time = NULL;
    cJSON *ue_update_status = NULL;
    OpenAPI_ue_update_status_e ue_update_statusVariable = 0;
    provisioning_time = cJSON_GetObjectItemCaseSensitive(nssai_ack_dataJSON, "provisioningTime");
    if (!provisioning_time) {
        ogs_error("OpenAPI_nssai_ack_data_parseFromJSON() failed [provisioning_time]");
        goto end;
    }
    if (!cJSON_IsString(provisioning_time) && !cJSON_IsNull(provisioning_time)) {
        ogs_error("OpenAPI_nssai_ack_data_parseFromJSON() failed [provisioning_time]");
        goto end;
    }

    ue_update_status = cJSON_GetObjectItemCaseSensitive(nssai_ack_dataJSON, "ueUpdateStatus");
    if (!ue_update_status) {
        ogs_error("OpenAPI_nssai_ack_data_parseFromJSON() failed [ue_update_status]");
        goto end;
    }
    if (!cJSON_IsString(ue_update_status)) {
        ogs_error("OpenAPI_nssai_ack_data_parseFromJSON() failed [ue_update_status]");
        goto end;
    }
    ue_update_statusVariable = OpenAPI_ue_update_status_FromString(ue_update_status->valuestring);

    nssai_ack_data_local_var = OpenAPI_nssai_ack_data_create (
        ogs_strdup(provisioning_time->valuestring),
        ue_update_statusVariable
    );

    return nssai_ack_data_local_var;
end:
    return NULL;
}

OpenAPI_nssai_ack_data_t *OpenAPI_nssai_ack_data_copy(OpenAPI_nssai_ack_data_t *dst, OpenAPI_nssai_ack_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nssai_ack_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nssai_ack_data_convertToJSON() failed");
        return NULL;
    }

    content = cJSON_Print(item);
    cJSON_Delete(item);

    if (!content) {
        ogs_error("cJSON_Print() failed");
        return NULL;
    }

    item = cJSON_Parse(content);
    ogs_free(content);
    if (!item) {
        ogs_error("cJSON_Parse() failed");
        return NULL;
    }

    OpenAPI_nssai_ack_data_free(dst);
    dst = OpenAPI_nssai_ack_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

