
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "acknowledge_info.h"

OpenAPI_acknowledge_info_t *OpenAPI_acknowledge_info_create(
    char *sor_mac_iue,
    char *upu_mac_iue,
    char *secured_packet,
    char *provisioning_time
    )
{
    OpenAPI_acknowledge_info_t *acknowledge_info_local_var = OpenAPI_malloc(sizeof(OpenAPI_acknowledge_info_t));
    if (!acknowledge_info_local_var) {
        return NULL;
    }
    acknowledge_info_local_var->sor_mac_iue = sor_mac_iue;
    acknowledge_info_local_var->upu_mac_iue = upu_mac_iue;
    acknowledge_info_local_var->secured_packet = secured_packet;
    acknowledge_info_local_var->provisioning_time = provisioning_time;

    return acknowledge_info_local_var;
}

void OpenAPI_acknowledge_info_free(OpenAPI_acknowledge_info_t *acknowledge_info)
{
    if (NULL == acknowledge_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(acknowledge_info->sor_mac_iue);
    ogs_free(acknowledge_info->upu_mac_iue);
    ogs_free(acknowledge_info->secured_packet);
    ogs_free(acknowledge_info->provisioning_time);
    ogs_free(acknowledge_info);
}

cJSON *OpenAPI_acknowledge_info_convertToJSON(OpenAPI_acknowledge_info_t *acknowledge_info)
{
    cJSON *item = NULL;

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

    if (acknowledge_info->secured_packet) {
        if (cJSON_AddStringToObject(item, "securedPacket", acknowledge_info->secured_packet) == NULL) {
            ogs_error("OpenAPI_acknowledge_info_convertToJSON() failed [secured_packet]");
            goto end;
        }
    }

    if (!acknowledge_info->provisioning_time) {
        ogs_error("OpenAPI_acknowledge_info_convertToJSON() failed [provisioning_time]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "provisioningTime", acknowledge_info->provisioning_time) == NULL) {
        ogs_error("OpenAPI_acknowledge_info_convertToJSON() failed [provisioning_time]");
        goto end;
    }

end:
    return item;
}

OpenAPI_acknowledge_info_t *OpenAPI_acknowledge_info_parseFromJSON(cJSON *acknowledge_infoJSON)
{
    OpenAPI_acknowledge_info_t *acknowledge_info_local_var = NULL;
    cJSON *sor_mac_iue = cJSON_GetObjectItemCaseSensitive(acknowledge_infoJSON, "sorMacIue");

    if (sor_mac_iue) {
        if (!cJSON_IsString(sor_mac_iue)) {
            ogs_error("OpenAPI_acknowledge_info_parseFromJSON() failed [sor_mac_iue]");
            goto end;
        }
    }

    cJSON *upu_mac_iue = cJSON_GetObjectItemCaseSensitive(acknowledge_infoJSON, "upuMacIue");

    if (upu_mac_iue) {
        if (!cJSON_IsString(upu_mac_iue)) {
            ogs_error("OpenAPI_acknowledge_info_parseFromJSON() failed [upu_mac_iue]");
            goto end;
        }
    }

    cJSON *secured_packet = cJSON_GetObjectItemCaseSensitive(acknowledge_infoJSON, "securedPacket");

    if (secured_packet) {
        if (!cJSON_IsString(secured_packet)) {
            ogs_error("OpenAPI_acknowledge_info_parseFromJSON() failed [secured_packet]");
            goto end;
        }
    }

    cJSON *provisioning_time = cJSON_GetObjectItemCaseSensitive(acknowledge_infoJSON, "provisioningTime");
    if (!provisioning_time) {
        ogs_error("OpenAPI_acknowledge_info_parseFromJSON() failed [provisioning_time]");
        goto end;
    }


    if (!cJSON_IsString(provisioning_time)) {
        ogs_error("OpenAPI_acknowledge_info_parseFromJSON() failed [provisioning_time]");
        goto end;
    }

    acknowledge_info_local_var = OpenAPI_acknowledge_info_create (
        sor_mac_iue ? ogs_strdup(sor_mac_iue->valuestring) : NULL,
        upu_mac_iue ? ogs_strdup(upu_mac_iue->valuestring) : NULL,
        secured_packet ? ogs_strdup(secured_packet->valuestring) : NULL,
        ogs_strdup(provisioning_time->valuestring)
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

