
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "af_coordination_info.h"

OpenAPI_af_coordination_info_t *OpenAPI_af_coordination_info_create(
    char *source_dnai,
    char *source_ue_ipv4_addr,
    char *source_ue_ipv6_prefix,
    OpenAPI_list_t *notification_info_list
)
{
    OpenAPI_af_coordination_info_t *af_coordination_info_local_var = ogs_malloc(sizeof(OpenAPI_af_coordination_info_t));
    ogs_assert(af_coordination_info_local_var);

    af_coordination_info_local_var->source_dnai = source_dnai;
    af_coordination_info_local_var->source_ue_ipv4_addr = source_ue_ipv4_addr;
    af_coordination_info_local_var->source_ue_ipv6_prefix = source_ue_ipv6_prefix;
    af_coordination_info_local_var->notification_info_list = notification_info_list;

    return af_coordination_info_local_var;
}

void OpenAPI_af_coordination_info_free(OpenAPI_af_coordination_info_t *af_coordination_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == af_coordination_info) {
        return;
    }
    if (af_coordination_info->source_dnai) {
        ogs_free(af_coordination_info->source_dnai);
        af_coordination_info->source_dnai = NULL;
    }
    if (af_coordination_info->source_ue_ipv4_addr) {
        ogs_free(af_coordination_info->source_ue_ipv4_addr);
        af_coordination_info->source_ue_ipv4_addr = NULL;
    }
    if (af_coordination_info->source_ue_ipv6_prefix) {
        ogs_free(af_coordination_info->source_ue_ipv6_prefix);
        af_coordination_info->source_ue_ipv6_prefix = NULL;
    }
    if (af_coordination_info->notification_info_list) {
        OpenAPI_list_for_each(af_coordination_info->notification_info_list, node) {
            OpenAPI_notification_info_free(node->data);
        }
        OpenAPI_list_free(af_coordination_info->notification_info_list);
        af_coordination_info->notification_info_list = NULL;
    }
    ogs_free(af_coordination_info);
}

cJSON *OpenAPI_af_coordination_info_convertToJSON(OpenAPI_af_coordination_info_t *af_coordination_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (af_coordination_info == NULL) {
        ogs_error("OpenAPI_af_coordination_info_convertToJSON() failed [AfCoordinationInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (af_coordination_info->source_dnai) {
    if (cJSON_AddStringToObject(item, "sourceDnai", af_coordination_info->source_dnai) == NULL) {
        ogs_error("OpenAPI_af_coordination_info_convertToJSON() failed [source_dnai]");
        goto end;
    }
    }

    if (af_coordination_info->source_ue_ipv4_addr) {
    if (cJSON_AddStringToObject(item, "sourceUeIpv4Addr", af_coordination_info->source_ue_ipv4_addr) == NULL) {
        ogs_error("OpenAPI_af_coordination_info_convertToJSON() failed [source_ue_ipv4_addr]");
        goto end;
    }
    }

    if (af_coordination_info->source_ue_ipv6_prefix) {
    if (cJSON_AddStringToObject(item, "sourceUeIpv6Prefix", af_coordination_info->source_ue_ipv6_prefix) == NULL) {
        ogs_error("OpenAPI_af_coordination_info_convertToJSON() failed [source_ue_ipv6_prefix]");
        goto end;
    }
    }

    if (af_coordination_info->notification_info_list) {
    cJSON *notification_info_listList = cJSON_AddArrayToObject(item, "notificationInfoList");
    if (notification_info_listList == NULL) {
        ogs_error("OpenAPI_af_coordination_info_convertToJSON() failed [notification_info_list]");
        goto end;
    }
    OpenAPI_list_for_each(af_coordination_info->notification_info_list, node) {
        cJSON *itemLocal = OpenAPI_notification_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_af_coordination_info_convertToJSON() failed [notification_info_list]");
            goto end;
        }
        cJSON_AddItemToArray(notification_info_listList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_af_coordination_info_t *OpenAPI_af_coordination_info_parseFromJSON(cJSON *af_coordination_infoJSON)
{
    OpenAPI_af_coordination_info_t *af_coordination_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *source_dnai = NULL;
    cJSON *source_ue_ipv4_addr = NULL;
    cJSON *source_ue_ipv6_prefix = NULL;
    cJSON *notification_info_list = NULL;
    OpenAPI_list_t *notification_info_listList = NULL;
    source_dnai = cJSON_GetObjectItemCaseSensitive(af_coordination_infoJSON, "sourceDnai");
    if (source_dnai) {
    if (!cJSON_IsString(source_dnai) && !cJSON_IsNull(source_dnai)) {
        ogs_error("OpenAPI_af_coordination_info_parseFromJSON() failed [source_dnai]");
        goto end;
    }
    }

    source_ue_ipv4_addr = cJSON_GetObjectItemCaseSensitive(af_coordination_infoJSON, "sourceUeIpv4Addr");
    if (source_ue_ipv4_addr) {
    if (!cJSON_IsString(source_ue_ipv4_addr) && !cJSON_IsNull(source_ue_ipv4_addr)) {
        ogs_error("OpenAPI_af_coordination_info_parseFromJSON() failed [source_ue_ipv4_addr]");
        goto end;
    }
    }

    source_ue_ipv6_prefix = cJSON_GetObjectItemCaseSensitive(af_coordination_infoJSON, "sourceUeIpv6Prefix");
    if (source_ue_ipv6_prefix) {
    if (!cJSON_IsString(source_ue_ipv6_prefix) && !cJSON_IsNull(source_ue_ipv6_prefix)) {
        ogs_error("OpenAPI_af_coordination_info_parseFromJSON() failed [source_ue_ipv6_prefix]");
        goto end;
    }
    }

    notification_info_list = cJSON_GetObjectItemCaseSensitive(af_coordination_infoJSON, "notificationInfoList");
    if (notification_info_list) {
        cJSON *notification_info_list_local = NULL;
        if (!cJSON_IsArray(notification_info_list)) {
            ogs_error("OpenAPI_af_coordination_info_parseFromJSON() failed [notification_info_list]");
            goto end;
        }

        notification_info_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(notification_info_list_local, notification_info_list) {
            if (!cJSON_IsObject(notification_info_list_local)) {
                ogs_error("OpenAPI_af_coordination_info_parseFromJSON() failed [notification_info_list]");
                goto end;
            }
            OpenAPI_notification_info_t *notification_info_listItem = OpenAPI_notification_info_parseFromJSON(notification_info_list_local);
            if (!notification_info_listItem) {
                ogs_error("No notification_info_listItem");
                goto end;
            }
            OpenAPI_list_add(notification_info_listList, notification_info_listItem);
        }
    }

    af_coordination_info_local_var = OpenAPI_af_coordination_info_create (
        source_dnai && !cJSON_IsNull(source_dnai) ? ogs_strdup(source_dnai->valuestring) : NULL,
        source_ue_ipv4_addr && !cJSON_IsNull(source_ue_ipv4_addr) ? ogs_strdup(source_ue_ipv4_addr->valuestring) : NULL,
        source_ue_ipv6_prefix && !cJSON_IsNull(source_ue_ipv6_prefix) ? ogs_strdup(source_ue_ipv6_prefix->valuestring) : NULL,
        notification_info_list ? notification_info_listList : NULL
    );

    return af_coordination_info_local_var;
end:
    if (notification_info_listList) {
        OpenAPI_list_for_each(notification_info_listList, node) {
            OpenAPI_notification_info_free(node->data);
        }
        OpenAPI_list_free(notification_info_listList);
        notification_info_listList = NULL;
    }
    return NULL;
}

OpenAPI_af_coordination_info_t *OpenAPI_af_coordination_info_copy(OpenAPI_af_coordination_info_t *dst, OpenAPI_af_coordination_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_af_coordination_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_af_coordination_info_convertToJSON() failed");
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

    OpenAPI_af_coordination_info_free(dst);
    dst = OpenAPI_af_coordination_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

