
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sor_data.h"

OpenAPI_sor_data_t *OpenAPI_sor_data_create(
    char *provisioning_time,
    OpenAPI_ue_update_status_e ue_update_status,
    char *sor_xmac_iue,
    char *sor_mac_iue,
    bool is_me_support_of_sor_cmci,
    int me_support_of_sor_cmci
)
{
    OpenAPI_sor_data_t *sor_data_local_var = ogs_malloc(sizeof(OpenAPI_sor_data_t));
    ogs_assert(sor_data_local_var);

    sor_data_local_var->provisioning_time = provisioning_time;
    sor_data_local_var->ue_update_status = ue_update_status;
    sor_data_local_var->sor_xmac_iue = sor_xmac_iue;
    sor_data_local_var->sor_mac_iue = sor_mac_iue;
    sor_data_local_var->is_me_support_of_sor_cmci = is_me_support_of_sor_cmci;
    sor_data_local_var->me_support_of_sor_cmci = me_support_of_sor_cmci;

    return sor_data_local_var;
}

void OpenAPI_sor_data_free(OpenAPI_sor_data_t *sor_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == sor_data) {
        return;
    }
    if (sor_data->provisioning_time) {
        ogs_free(sor_data->provisioning_time);
        sor_data->provisioning_time = NULL;
    }
    if (sor_data->sor_xmac_iue) {
        ogs_free(sor_data->sor_xmac_iue);
        sor_data->sor_xmac_iue = NULL;
    }
    if (sor_data->sor_mac_iue) {
        ogs_free(sor_data->sor_mac_iue);
        sor_data->sor_mac_iue = NULL;
    }
    ogs_free(sor_data);
}

cJSON *OpenAPI_sor_data_convertToJSON(OpenAPI_sor_data_t *sor_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (sor_data == NULL) {
        ogs_error("OpenAPI_sor_data_convertToJSON() failed [SorData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!sor_data->provisioning_time) {
        ogs_error("OpenAPI_sor_data_convertToJSON() failed [provisioning_time]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "provisioningTime", sor_data->provisioning_time) == NULL) {
        ogs_error("OpenAPI_sor_data_convertToJSON() failed [provisioning_time]");
        goto end;
    }

    if (sor_data->ue_update_status == OpenAPI_ue_update_status_NULL) {
        ogs_error("OpenAPI_sor_data_convertToJSON() failed [ue_update_status]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "ueUpdateStatus", OpenAPI_ue_update_status_ToString(sor_data->ue_update_status)) == NULL) {
        ogs_error("OpenAPI_sor_data_convertToJSON() failed [ue_update_status]");
        goto end;
    }

    if (sor_data->sor_xmac_iue) {
    if (cJSON_AddStringToObject(item, "sorXmacIue", sor_data->sor_xmac_iue) == NULL) {
        ogs_error("OpenAPI_sor_data_convertToJSON() failed [sor_xmac_iue]");
        goto end;
    }
    }

    if (sor_data->sor_mac_iue) {
    if (cJSON_AddStringToObject(item, "sorMacIue", sor_data->sor_mac_iue) == NULL) {
        ogs_error("OpenAPI_sor_data_convertToJSON() failed [sor_mac_iue]");
        goto end;
    }
    }

    if (sor_data->is_me_support_of_sor_cmci) {
    if (cJSON_AddBoolToObject(item, "meSupportOfSorCmci", sor_data->me_support_of_sor_cmci) == NULL) {
        ogs_error("OpenAPI_sor_data_convertToJSON() failed [me_support_of_sor_cmci]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_sor_data_t *OpenAPI_sor_data_parseFromJSON(cJSON *sor_dataJSON)
{
    OpenAPI_sor_data_t *sor_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *provisioning_time = NULL;
    cJSON *ue_update_status = NULL;
    OpenAPI_ue_update_status_e ue_update_statusVariable = 0;
    cJSON *sor_xmac_iue = NULL;
    cJSON *sor_mac_iue = NULL;
    cJSON *me_support_of_sor_cmci = NULL;
    provisioning_time = cJSON_GetObjectItemCaseSensitive(sor_dataJSON, "provisioningTime");
    if (!provisioning_time) {
        ogs_error("OpenAPI_sor_data_parseFromJSON() failed [provisioning_time]");
        goto end;
    }
    if (!cJSON_IsString(provisioning_time) && !cJSON_IsNull(provisioning_time)) {
        ogs_error("OpenAPI_sor_data_parseFromJSON() failed [provisioning_time]");
        goto end;
    }

    ue_update_status = cJSON_GetObjectItemCaseSensitive(sor_dataJSON, "ueUpdateStatus");
    if (!ue_update_status) {
        ogs_error("OpenAPI_sor_data_parseFromJSON() failed [ue_update_status]");
        goto end;
    }
    if (!cJSON_IsString(ue_update_status)) {
        ogs_error("OpenAPI_sor_data_parseFromJSON() failed [ue_update_status]");
        goto end;
    }
    ue_update_statusVariable = OpenAPI_ue_update_status_FromString(ue_update_status->valuestring);

    sor_xmac_iue = cJSON_GetObjectItemCaseSensitive(sor_dataJSON, "sorXmacIue");
    if (sor_xmac_iue) {
    if (!cJSON_IsString(sor_xmac_iue) && !cJSON_IsNull(sor_xmac_iue)) {
        ogs_error("OpenAPI_sor_data_parseFromJSON() failed [sor_xmac_iue]");
        goto end;
    }
    }

    sor_mac_iue = cJSON_GetObjectItemCaseSensitive(sor_dataJSON, "sorMacIue");
    if (sor_mac_iue) {
    if (!cJSON_IsString(sor_mac_iue) && !cJSON_IsNull(sor_mac_iue)) {
        ogs_error("OpenAPI_sor_data_parseFromJSON() failed [sor_mac_iue]");
        goto end;
    }
    }

    me_support_of_sor_cmci = cJSON_GetObjectItemCaseSensitive(sor_dataJSON, "meSupportOfSorCmci");
    if (me_support_of_sor_cmci) {
    if (!cJSON_IsBool(me_support_of_sor_cmci)) {
        ogs_error("OpenAPI_sor_data_parseFromJSON() failed [me_support_of_sor_cmci]");
        goto end;
    }
    }

    sor_data_local_var = OpenAPI_sor_data_create (
        ogs_strdup(provisioning_time->valuestring),
        ue_update_statusVariable,
        sor_xmac_iue && !cJSON_IsNull(sor_xmac_iue) ? ogs_strdup(sor_xmac_iue->valuestring) : NULL,
        sor_mac_iue && !cJSON_IsNull(sor_mac_iue) ? ogs_strdup(sor_mac_iue->valuestring) : NULL,
        me_support_of_sor_cmci ? true : false,
        me_support_of_sor_cmci ? me_support_of_sor_cmci->valueint : 0
    );

    return sor_data_local_var;
end:
    return NULL;
}

OpenAPI_sor_data_t *OpenAPI_sor_data_copy(OpenAPI_sor_data_t *dst, OpenAPI_sor_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sor_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sor_data_convertToJSON() failed");
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

    OpenAPI_sor_data_free(dst);
    dst = OpenAPI_sor_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

