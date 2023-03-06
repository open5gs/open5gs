
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pcf_ue_callback_info.h"

OpenAPI_pcf_ue_callback_info_t *OpenAPI_pcf_ue_callback_info_create(
    char *callback_uri,
    char *binding_info
)
{
    OpenAPI_pcf_ue_callback_info_t *pcf_ue_callback_info_local_var = ogs_malloc(sizeof(OpenAPI_pcf_ue_callback_info_t));
    ogs_assert(pcf_ue_callback_info_local_var);

    pcf_ue_callback_info_local_var->callback_uri = callback_uri;
    pcf_ue_callback_info_local_var->binding_info = binding_info;

    return pcf_ue_callback_info_local_var;
}

void OpenAPI_pcf_ue_callback_info_free(OpenAPI_pcf_ue_callback_info_t *pcf_ue_callback_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pcf_ue_callback_info) {
        return;
    }
    if (pcf_ue_callback_info->callback_uri) {
        ogs_free(pcf_ue_callback_info->callback_uri);
        pcf_ue_callback_info->callback_uri = NULL;
    }
    if (pcf_ue_callback_info->binding_info) {
        ogs_free(pcf_ue_callback_info->binding_info);
        pcf_ue_callback_info->binding_info = NULL;
    }
    ogs_free(pcf_ue_callback_info);
}

cJSON *OpenAPI_pcf_ue_callback_info_convertToJSON(OpenAPI_pcf_ue_callback_info_t *pcf_ue_callback_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pcf_ue_callback_info == NULL) {
        ogs_error("OpenAPI_pcf_ue_callback_info_convertToJSON() failed [PcfUeCallbackInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!pcf_ue_callback_info->callback_uri) {
        ogs_error("OpenAPI_pcf_ue_callback_info_convertToJSON() failed [callback_uri]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "callbackUri", pcf_ue_callback_info->callback_uri) == NULL) {
        ogs_error("OpenAPI_pcf_ue_callback_info_convertToJSON() failed [callback_uri]");
        goto end;
    }

    if (pcf_ue_callback_info->binding_info) {
    if (cJSON_AddStringToObject(item, "bindingInfo", pcf_ue_callback_info->binding_info) == NULL) {
        ogs_error("OpenAPI_pcf_ue_callback_info_convertToJSON() failed [binding_info]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_pcf_ue_callback_info_t *OpenAPI_pcf_ue_callback_info_parseFromJSON(cJSON *pcf_ue_callback_infoJSON)
{
    OpenAPI_pcf_ue_callback_info_t *pcf_ue_callback_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *callback_uri = NULL;
    cJSON *binding_info = NULL;
    callback_uri = cJSON_GetObjectItemCaseSensitive(pcf_ue_callback_infoJSON, "callbackUri");
    if (!callback_uri) {
        ogs_error("OpenAPI_pcf_ue_callback_info_parseFromJSON() failed [callback_uri]");
        goto end;
    }
    if (!cJSON_IsString(callback_uri)) {
        ogs_error("OpenAPI_pcf_ue_callback_info_parseFromJSON() failed [callback_uri]");
        goto end;
    }

    binding_info = cJSON_GetObjectItemCaseSensitive(pcf_ue_callback_infoJSON, "bindingInfo");
    if (binding_info) {
    if (!cJSON_IsString(binding_info) && !cJSON_IsNull(binding_info)) {
        ogs_error("OpenAPI_pcf_ue_callback_info_parseFromJSON() failed [binding_info]");
        goto end;
    }
    }

    pcf_ue_callback_info_local_var = OpenAPI_pcf_ue_callback_info_create (
        ogs_strdup(callback_uri->valuestring),
        binding_info && !cJSON_IsNull(binding_info) ? ogs_strdup(binding_info->valuestring) : NULL
    );

    return pcf_ue_callback_info_local_var;
end:
    return NULL;
}

OpenAPI_pcf_ue_callback_info_t *OpenAPI_pcf_ue_callback_info_copy(OpenAPI_pcf_ue_callback_info_t *dst, OpenAPI_pcf_ue_callback_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pcf_ue_callback_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pcf_ue_callback_info_convertToJSON() failed");
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

    OpenAPI_pcf_ue_callback_info_free(dst);
    dst = OpenAPI_pcf_ue_callback_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

