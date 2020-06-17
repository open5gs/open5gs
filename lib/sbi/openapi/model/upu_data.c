
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "upu_data.h"

OpenAPI_upu_data_t *OpenAPI_upu_data_create(
    char *provisioning_time,
    OpenAPI_ue_update_status_e ue_update_status,
    char *upu_xmac_iue,
    char *upu_mac_iue
    )
{
    OpenAPI_upu_data_t *upu_data_local_var = OpenAPI_malloc(sizeof(OpenAPI_upu_data_t));
    if (!upu_data_local_var) {
        return NULL;
    }
    upu_data_local_var->provisioning_time = provisioning_time;
    upu_data_local_var->ue_update_status = ue_update_status;
    upu_data_local_var->upu_xmac_iue = upu_xmac_iue;
    upu_data_local_var->upu_mac_iue = upu_mac_iue;

    return upu_data_local_var;
}

void OpenAPI_upu_data_free(OpenAPI_upu_data_t *upu_data)
{
    if (NULL == upu_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(upu_data->provisioning_time);
    ogs_free(upu_data->upu_xmac_iue);
    ogs_free(upu_data->upu_mac_iue);
    ogs_free(upu_data);
}

cJSON *OpenAPI_upu_data_convertToJSON(OpenAPI_upu_data_t *upu_data)
{
    cJSON *item = NULL;

    if (upu_data == NULL) {
        ogs_error("OpenAPI_upu_data_convertToJSON() failed [UpuData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!upu_data->provisioning_time) {
        ogs_error("OpenAPI_upu_data_convertToJSON() failed [provisioning_time]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "provisioningTime", upu_data->provisioning_time) == NULL) {
        ogs_error("OpenAPI_upu_data_convertToJSON() failed [provisioning_time]");
        goto end;
    }

    if (!upu_data->ue_update_status) {
        ogs_error("OpenAPI_upu_data_convertToJSON() failed [ue_update_status]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "ueUpdateStatus", OpenAPI_ue_update_status_ToString(upu_data->ue_update_status)) == NULL) {
        ogs_error("OpenAPI_upu_data_convertToJSON() failed [ue_update_status]");
        goto end;
    }

    if (upu_data->upu_xmac_iue) {
        if (cJSON_AddStringToObject(item, "upuXmacIue", upu_data->upu_xmac_iue) == NULL) {
            ogs_error("OpenAPI_upu_data_convertToJSON() failed [upu_xmac_iue]");
            goto end;
        }
    }

    if (upu_data->upu_mac_iue) {
        if (cJSON_AddStringToObject(item, "upuMacIue", upu_data->upu_mac_iue) == NULL) {
            ogs_error("OpenAPI_upu_data_convertToJSON() failed [upu_mac_iue]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_upu_data_t *OpenAPI_upu_data_parseFromJSON(cJSON *upu_dataJSON)
{
    OpenAPI_upu_data_t *upu_data_local_var = NULL;
    cJSON *provisioning_time = cJSON_GetObjectItemCaseSensitive(upu_dataJSON, "provisioningTime");
    if (!provisioning_time) {
        ogs_error("OpenAPI_upu_data_parseFromJSON() failed [provisioning_time]");
        goto end;
    }


    if (!cJSON_IsString(provisioning_time)) {
        ogs_error("OpenAPI_upu_data_parseFromJSON() failed [provisioning_time]");
        goto end;
    }

    cJSON *ue_update_status = cJSON_GetObjectItemCaseSensitive(upu_dataJSON, "ueUpdateStatus");
    if (!ue_update_status) {
        ogs_error("OpenAPI_upu_data_parseFromJSON() failed [ue_update_status]");
        goto end;
    }

    OpenAPI_ue_update_status_e ue_update_statusVariable;

    if (!cJSON_IsString(ue_update_status)) {
        ogs_error("OpenAPI_upu_data_parseFromJSON() failed [ue_update_status]");
        goto end;
    }
    ue_update_statusVariable = OpenAPI_ue_update_status_FromString(ue_update_status->valuestring);

    cJSON *upu_xmac_iue = cJSON_GetObjectItemCaseSensitive(upu_dataJSON, "upuXmacIue");

    if (upu_xmac_iue) {
        if (!cJSON_IsString(upu_xmac_iue)) {
            ogs_error("OpenAPI_upu_data_parseFromJSON() failed [upu_xmac_iue]");
            goto end;
        }
    }

    cJSON *upu_mac_iue = cJSON_GetObjectItemCaseSensitive(upu_dataJSON, "upuMacIue");

    if (upu_mac_iue) {
        if (!cJSON_IsString(upu_mac_iue)) {
            ogs_error("OpenAPI_upu_data_parseFromJSON() failed [upu_mac_iue]");
            goto end;
        }
    }

    upu_data_local_var = OpenAPI_upu_data_create (
        ogs_strdup(provisioning_time->valuestring),
        ue_update_statusVariable,
        upu_xmac_iue ? ogs_strdup(upu_xmac_iue->valuestring) : NULL,
        upu_mac_iue ? ogs_strdup(upu_mac_iue->valuestring) : NULL
        );

    return upu_data_local_var;
end:
    return NULL;
}

OpenAPI_upu_data_t *OpenAPI_upu_data_copy(OpenAPI_upu_data_t *dst, OpenAPI_upu_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_upu_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_upu_data_convertToJSON() failed");
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

    OpenAPI_upu_data_free(dst);
    dst = OpenAPI_upu_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

