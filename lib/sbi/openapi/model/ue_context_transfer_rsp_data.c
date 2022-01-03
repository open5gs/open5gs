
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_context_transfer_rsp_data.h"

OpenAPI_ue_context_transfer_rsp_data_t *OpenAPI_ue_context_transfer_rsp_data_create(
    OpenAPI_ue_context_t *ue_context,
    OpenAPI_n2_info_content_t *ue_radio_capability,
    OpenAPI_n2_info_content_t *ue_nbiot_radio_capability,
    char *supported_features
)
{
    OpenAPI_ue_context_transfer_rsp_data_t *ue_context_transfer_rsp_data_local_var = ogs_malloc(sizeof(OpenAPI_ue_context_transfer_rsp_data_t));
    ogs_assert(ue_context_transfer_rsp_data_local_var);

    ue_context_transfer_rsp_data_local_var->ue_context = ue_context;
    ue_context_transfer_rsp_data_local_var->ue_radio_capability = ue_radio_capability;
    ue_context_transfer_rsp_data_local_var->ue_nbiot_radio_capability = ue_nbiot_radio_capability;
    ue_context_transfer_rsp_data_local_var->supported_features = supported_features;

    return ue_context_transfer_rsp_data_local_var;
}

void OpenAPI_ue_context_transfer_rsp_data_free(OpenAPI_ue_context_transfer_rsp_data_t *ue_context_transfer_rsp_data)
{
    if (NULL == ue_context_transfer_rsp_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_ue_context_free(ue_context_transfer_rsp_data->ue_context);
    OpenAPI_n2_info_content_free(ue_context_transfer_rsp_data->ue_radio_capability);
    OpenAPI_n2_info_content_free(ue_context_transfer_rsp_data->ue_nbiot_radio_capability);
    ogs_free(ue_context_transfer_rsp_data->supported_features);
    ogs_free(ue_context_transfer_rsp_data);
}

cJSON *OpenAPI_ue_context_transfer_rsp_data_convertToJSON(OpenAPI_ue_context_transfer_rsp_data_t *ue_context_transfer_rsp_data)
{
    cJSON *item = NULL;

    if (ue_context_transfer_rsp_data == NULL) {
        ogs_error("OpenAPI_ue_context_transfer_rsp_data_convertToJSON() failed [UeContextTransferRspData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    cJSON *ue_context_local_JSON = OpenAPI_ue_context_convertToJSON(ue_context_transfer_rsp_data->ue_context);
    if (ue_context_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_context_transfer_rsp_data_convertToJSON() failed [ue_context]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ueContext", ue_context_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_context_transfer_rsp_data_convertToJSON() failed [ue_context]");
        goto end;
    }

    if (ue_context_transfer_rsp_data->ue_radio_capability) {
    cJSON *ue_radio_capability_local_JSON = OpenAPI_n2_info_content_convertToJSON(ue_context_transfer_rsp_data->ue_radio_capability);
    if (ue_radio_capability_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_context_transfer_rsp_data_convertToJSON() failed [ue_radio_capability]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ueRadioCapability", ue_radio_capability_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_context_transfer_rsp_data_convertToJSON() failed [ue_radio_capability]");
        goto end;
    }
    }

    if (ue_context_transfer_rsp_data->ue_nbiot_radio_capability) {
    cJSON *ue_nbiot_radio_capability_local_JSON = OpenAPI_n2_info_content_convertToJSON(ue_context_transfer_rsp_data->ue_nbiot_radio_capability);
    if (ue_nbiot_radio_capability_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_context_transfer_rsp_data_convertToJSON() failed [ue_nbiot_radio_capability]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ueNbiotRadioCapability", ue_nbiot_radio_capability_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_context_transfer_rsp_data_convertToJSON() failed [ue_nbiot_radio_capability]");
        goto end;
    }
    }

    if (ue_context_transfer_rsp_data->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", ue_context_transfer_rsp_data->supported_features) == NULL) {
        ogs_error("OpenAPI_ue_context_transfer_rsp_data_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ue_context_transfer_rsp_data_t *OpenAPI_ue_context_transfer_rsp_data_parseFromJSON(cJSON *ue_context_transfer_rsp_dataJSON)
{
    OpenAPI_ue_context_transfer_rsp_data_t *ue_context_transfer_rsp_data_local_var = NULL;
    cJSON *ue_context = cJSON_GetObjectItemCaseSensitive(ue_context_transfer_rsp_dataJSON, "ueContext");
    if (!ue_context) {
        ogs_error("OpenAPI_ue_context_transfer_rsp_data_parseFromJSON() failed [ue_context]");
        goto end;
    }

    OpenAPI_ue_context_t *ue_context_local_nonprim = NULL;
    ue_context_local_nonprim = OpenAPI_ue_context_parseFromJSON(ue_context);

    cJSON *ue_radio_capability = cJSON_GetObjectItemCaseSensitive(ue_context_transfer_rsp_dataJSON, "ueRadioCapability");

    OpenAPI_n2_info_content_t *ue_radio_capability_local_nonprim = NULL;
    if (ue_radio_capability) {
    ue_radio_capability_local_nonprim = OpenAPI_n2_info_content_parseFromJSON(ue_radio_capability);
    }

    cJSON *ue_nbiot_radio_capability = cJSON_GetObjectItemCaseSensitive(ue_context_transfer_rsp_dataJSON, "ueNbiotRadioCapability");

    OpenAPI_n2_info_content_t *ue_nbiot_radio_capability_local_nonprim = NULL;
    if (ue_nbiot_radio_capability) {
    ue_nbiot_radio_capability_local_nonprim = OpenAPI_n2_info_content_parseFromJSON(ue_nbiot_radio_capability);
    }

    cJSON *supported_features = cJSON_GetObjectItemCaseSensitive(ue_context_transfer_rsp_dataJSON, "supportedFeatures");

    if (supported_features) {
    if (!cJSON_IsString(supported_features)) {
        ogs_error("OpenAPI_ue_context_transfer_rsp_data_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    ue_context_transfer_rsp_data_local_var = OpenAPI_ue_context_transfer_rsp_data_create (
        ue_context_local_nonprim,
        ue_radio_capability ? ue_radio_capability_local_nonprim : NULL,
        ue_nbiot_radio_capability ? ue_nbiot_radio_capability_local_nonprim : NULL,
        supported_features ? ogs_strdup(supported_features->valuestring) : NULL
    );

    return ue_context_transfer_rsp_data_local_var;
end:
    return NULL;
}

OpenAPI_ue_context_transfer_rsp_data_t *OpenAPI_ue_context_transfer_rsp_data_copy(OpenAPI_ue_context_transfer_rsp_data_t *dst, OpenAPI_ue_context_transfer_rsp_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ue_context_transfer_rsp_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ue_context_transfer_rsp_data_convertToJSON() failed");
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

    OpenAPI_ue_context_transfer_rsp_data_free(dst);
    dst = OpenAPI_ue_context_transfer_rsp_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

