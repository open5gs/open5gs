
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "acknowledge_info.h"

OpenAPI_acknowledge_info_t *OpenAPI_acknowledge_info_create(
    char *sor_mac_iue,
    char *upu_mac_iue,
    char *provisioning_time,
    char *sor_transparent_container,
    bool is_ue_not_reachable,
    int ue_not_reachable,
    char *upu_transparent_container
)
{
    OpenAPI_acknowledge_info_t *acknowledge_info_local_var = ogs_malloc(sizeof(OpenAPI_acknowledge_info_t));
    ogs_assert(acknowledge_info_local_var);

    acknowledge_info_local_var->sor_mac_iue = sor_mac_iue;
    acknowledge_info_local_var->upu_mac_iue = upu_mac_iue;
    acknowledge_info_local_var->provisioning_time = provisioning_time;
    acknowledge_info_local_var->sor_transparent_container = sor_transparent_container;
    acknowledge_info_local_var->is_ue_not_reachable = is_ue_not_reachable;
    acknowledge_info_local_var->ue_not_reachable = ue_not_reachable;
    acknowledge_info_local_var->upu_transparent_container = upu_transparent_container;

    return acknowledge_info_local_var;
}

void OpenAPI_acknowledge_info_free(OpenAPI_acknowledge_info_t *acknowledge_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == acknowledge_info) {
        return;
    }
    if (acknowledge_info->sor_mac_iue) {
        ogs_free(acknowledge_info->sor_mac_iue);
        acknowledge_info->sor_mac_iue = NULL;
    }
    if (acknowledge_info->upu_mac_iue) {
        ogs_free(acknowledge_info->upu_mac_iue);
        acknowledge_info->upu_mac_iue = NULL;
    }
    if (acknowledge_info->provisioning_time) {
        ogs_free(acknowledge_info->provisioning_time);
        acknowledge_info->provisioning_time = NULL;
    }
    if (acknowledge_info->sor_transparent_container) {
        ogs_free(acknowledge_info->sor_transparent_container);
        acknowledge_info->sor_transparent_container = NULL;
    }
    if (acknowledge_info->upu_transparent_container) {
        ogs_free(acknowledge_info->upu_transparent_container);
        acknowledge_info->upu_transparent_container = NULL;
    }
    ogs_free(acknowledge_info);
}

cJSON *OpenAPI_acknowledge_info_convertToJSON(OpenAPI_acknowledge_info_t *acknowledge_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (acknowledge_info == NULL) {
        ogs_error("OpenAPI_acknowledge_info_convertToJSON() failed [AcknowledgeInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (acknowledge_info->sor_mac_iue) {
    if (cJSON_AddStringToObject(item, "sorMacIue", acknowledge_info->sor_mac_iue) == NULL) {
        ogs_error("OpenAPI_acknowledge_info_convertToJSON() failed [sor_mac_iue]");
        goto end;
    }
    }

    if (acknowledge_info->upu_mac_iue) {
    if (cJSON_AddStringToObject(item, "upuMacIue", acknowledge_info->upu_mac_iue) == NULL) {
        ogs_error("OpenAPI_acknowledge_info_convertToJSON() failed [upu_mac_iue]");
        goto end;
    }
    }

    if (!acknowledge_info->provisioning_time) {
        ogs_error("OpenAPI_acknowledge_info_convertToJSON() failed [provisioning_time]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "provisioningTime", acknowledge_info->provisioning_time) == NULL) {
        ogs_error("OpenAPI_acknowledge_info_convertToJSON() failed [provisioning_time]");
        goto end;
    }

    if (acknowledge_info->sor_transparent_container) {
    if (cJSON_AddStringToObject(item, "sorTransparentContainer", acknowledge_info->sor_transparent_container) == NULL) {
        ogs_error("OpenAPI_acknowledge_info_convertToJSON() failed [sor_transparent_container]");
        goto end;
    }
    }

    if (acknowledge_info->is_ue_not_reachable) {
    if (cJSON_AddBoolToObject(item, "ueNotReachable", acknowledge_info->ue_not_reachable) == NULL) {
        ogs_error("OpenAPI_acknowledge_info_convertToJSON() failed [ue_not_reachable]");
        goto end;
    }
    }

    if (acknowledge_info->upu_transparent_container) {
    if (cJSON_AddStringToObject(item, "upuTransparentContainer", acknowledge_info->upu_transparent_container) == NULL) {
        ogs_error("OpenAPI_acknowledge_info_convertToJSON() failed [upu_transparent_container]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_acknowledge_info_t *OpenAPI_acknowledge_info_parseFromJSON(cJSON *acknowledge_infoJSON)
{
    OpenAPI_acknowledge_info_t *acknowledge_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *sor_mac_iue = NULL;
    cJSON *upu_mac_iue = NULL;
    cJSON *provisioning_time = NULL;
    cJSON *sor_transparent_container = NULL;
    cJSON *ue_not_reachable = NULL;
    cJSON *upu_transparent_container = NULL;
    sor_mac_iue = cJSON_GetObjectItemCaseSensitive(acknowledge_infoJSON, "sorMacIue");
    if (sor_mac_iue) {
    if (!cJSON_IsString(sor_mac_iue) && !cJSON_IsNull(sor_mac_iue)) {
        ogs_error("OpenAPI_acknowledge_info_parseFromJSON() failed [sor_mac_iue]");
        goto end;
    }
    }

    upu_mac_iue = cJSON_GetObjectItemCaseSensitive(acknowledge_infoJSON, "upuMacIue");
    if (upu_mac_iue) {
    if (!cJSON_IsString(upu_mac_iue) && !cJSON_IsNull(upu_mac_iue)) {
        ogs_error("OpenAPI_acknowledge_info_parseFromJSON() failed [upu_mac_iue]");
        goto end;
    }
    }

    provisioning_time = cJSON_GetObjectItemCaseSensitive(acknowledge_infoJSON, "provisioningTime");
    if (!provisioning_time) {
        ogs_error("OpenAPI_acknowledge_info_parseFromJSON() failed [provisioning_time]");
        goto end;
    }
    if (!cJSON_IsString(provisioning_time) && !cJSON_IsNull(provisioning_time)) {
        ogs_error("OpenAPI_acknowledge_info_parseFromJSON() failed [provisioning_time]");
        goto end;
    }

    sor_transparent_container = cJSON_GetObjectItemCaseSensitive(acknowledge_infoJSON, "sorTransparentContainer");
    if (sor_transparent_container) {
    if (!cJSON_IsString(sor_transparent_container) && !cJSON_IsNull(sor_transparent_container)) {
        ogs_error("OpenAPI_acknowledge_info_parseFromJSON() failed [sor_transparent_container]");
        goto end;
    }
    }

    ue_not_reachable = cJSON_GetObjectItemCaseSensitive(acknowledge_infoJSON, "ueNotReachable");
    if (ue_not_reachable) {
    if (!cJSON_IsBool(ue_not_reachable)) {
        ogs_error("OpenAPI_acknowledge_info_parseFromJSON() failed [ue_not_reachable]");
        goto end;
    }
    }

    upu_transparent_container = cJSON_GetObjectItemCaseSensitive(acknowledge_infoJSON, "upuTransparentContainer");
    if (upu_transparent_container) {
    if (!cJSON_IsString(upu_transparent_container) && !cJSON_IsNull(upu_transparent_container)) {
        ogs_error("OpenAPI_acknowledge_info_parseFromJSON() failed [upu_transparent_container]");
        goto end;
    }
    }

    acknowledge_info_local_var = OpenAPI_acknowledge_info_create (
        sor_mac_iue && !cJSON_IsNull(sor_mac_iue) ? ogs_strdup(sor_mac_iue->valuestring) : NULL,
        upu_mac_iue && !cJSON_IsNull(upu_mac_iue) ? ogs_strdup(upu_mac_iue->valuestring) : NULL,
        ogs_strdup(provisioning_time->valuestring),
        sor_transparent_container && !cJSON_IsNull(sor_transparent_container) ? ogs_strdup(sor_transparent_container->valuestring) : NULL,
        ue_not_reachable ? true : false,
        ue_not_reachable ? ue_not_reachable->valueint : 0,
        upu_transparent_container && !cJSON_IsNull(upu_transparent_container) ? ogs_strdup(upu_transparent_container->valuestring) : NULL
    );

    return acknowledge_info_local_var;
end:
    return NULL;
}

OpenAPI_acknowledge_info_t *OpenAPI_acknowledge_info_copy(OpenAPI_acknowledge_info_t *dst, OpenAPI_acknowledge_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_acknowledge_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_acknowledge_info_convertToJSON() failed");
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

    OpenAPI_acknowledge_info_free(dst);
    dst = OpenAPI_acknowledge_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

