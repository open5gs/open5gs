
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mb_upf_info.h"

OpenAPI_mb_upf_info_t *OpenAPI_mb_upf_info_create(
    OpenAPI_list_t *s_nssai_mb_upf_info_list,
    OpenAPI_list_t *mb_smf_serving_area,
    OpenAPI_list_t *interface_mb_upf_info_list,
    OpenAPI_list_t *tai_list,
    OpenAPI_list_t *tai_range_list,
    bool is_priority,
    int priority,
    char *supported_pfcp_features
)
{
    OpenAPI_mb_upf_info_t *mb_upf_info_local_var = ogs_malloc(sizeof(OpenAPI_mb_upf_info_t));
    ogs_assert(mb_upf_info_local_var);

    mb_upf_info_local_var->s_nssai_mb_upf_info_list = s_nssai_mb_upf_info_list;
    mb_upf_info_local_var->mb_smf_serving_area = mb_smf_serving_area;
    mb_upf_info_local_var->interface_mb_upf_info_list = interface_mb_upf_info_list;
    mb_upf_info_local_var->tai_list = tai_list;
    mb_upf_info_local_var->tai_range_list = tai_range_list;
    mb_upf_info_local_var->is_priority = is_priority;
    mb_upf_info_local_var->priority = priority;
    mb_upf_info_local_var->supported_pfcp_features = supported_pfcp_features;

    return mb_upf_info_local_var;
}

void OpenAPI_mb_upf_info_free(OpenAPI_mb_upf_info_t *mb_upf_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == mb_upf_info) {
        return;
    }
    if (mb_upf_info->s_nssai_mb_upf_info_list) {
        OpenAPI_list_for_each(mb_upf_info->s_nssai_mb_upf_info_list, node) {
            OpenAPI_snssai_upf_info_item_free(node->data);
        }
        OpenAPI_list_free(mb_upf_info->s_nssai_mb_upf_info_list);
        mb_upf_info->s_nssai_mb_upf_info_list = NULL;
    }
    if (mb_upf_info->mb_smf_serving_area) {
        OpenAPI_list_for_each(mb_upf_info->mb_smf_serving_area, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(mb_upf_info->mb_smf_serving_area);
        mb_upf_info->mb_smf_serving_area = NULL;
    }
    if (mb_upf_info->interface_mb_upf_info_list) {
        OpenAPI_list_for_each(mb_upf_info->interface_mb_upf_info_list, node) {
            OpenAPI_interface_upf_info_item_free(node->data);
        }
        OpenAPI_list_free(mb_upf_info->interface_mb_upf_info_list);
        mb_upf_info->interface_mb_upf_info_list = NULL;
    }
    if (mb_upf_info->tai_list) {
        OpenAPI_list_for_each(mb_upf_info->tai_list, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(mb_upf_info->tai_list);
        mb_upf_info->tai_list = NULL;
    }
    if (mb_upf_info->tai_range_list) {
        OpenAPI_list_for_each(mb_upf_info->tai_range_list, node) {
            OpenAPI_tai_range_free(node->data);
        }
        OpenAPI_list_free(mb_upf_info->tai_range_list);
        mb_upf_info->tai_range_list = NULL;
    }
    if (mb_upf_info->supported_pfcp_features) {
        ogs_free(mb_upf_info->supported_pfcp_features);
        mb_upf_info->supported_pfcp_features = NULL;
    }
    ogs_free(mb_upf_info);
}

cJSON *OpenAPI_mb_upf_info_convertToJSON(OpenAPI_mb_upf_info_t *mb_upf_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (mb_upf_info == NULL) {
        ogs_error("OpenAPI_mb_upf_info_convertToJSON() failed [MbUpfInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!mb_upf_info->s_nssai_mb_upf_info_list) {
        ogs_error("OpenAPI_mb_upf_info_convertToJSON() failed [s_nssai_mb_upf_info_list]");
        return NULL;
    }
    cJSON *s_nssai_mb_upf_info_listList = cJSON_AddArrayToObject(item, "sNssaiMbUpfInfoList");
    if (s_nssai_mb_upf_info_listList == NULL) {
        ogs_error("OpenAPI_mb_upf_info_convertToJSON() failed [s_nssai_mb_upf_info_list]");
        goto end;
    }
    OpenAPI_list_for_each(mb_upf_info->s_nssai_mb_upf_info_list, node) {
        cJSON *itemLocal = OpenAPI_snssai_upf_info_item_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_mb_upf_info_convertToJSON() failed [s_nssai_mb_upf_info_list]");
            goto end;
        }
        cJSON_AddItemToArray(s_nssai_mb_upf_info_listList, itemLocal);
    }

    if (mb_upf_info->mb_smf_serving_area) {
    cJSON *mb_smf_serving_areaList = cJSON_AddArrayToObject(item, "mbSmfServingArea");
    if (mb_smf_serving_areaList == NULL) {
        ogs_error("OpenAPI_mb_upf_info_convertToJSON() failed [mb_smf_serving_area]");
        goto end;
    }
    OpenAPI_list_for_each(mb_upf_info->mb_smf_serving_area, node) {
        if (cJSON_AddStringToObject(mb_smf_serving_areaList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_mb_upf_info_convertToJSON() failed [mb_smf_serving_area]");
            goto end;
        }
    }
    }

    if (mb_upf_info->interface_mb_upf_info_list) {
    cJSON *interface_mb_upf_info_listList = cJSON_AddArrayToObject(item, "interfaceMbUpfInfoList");
    if (interface_mb_upf_info_listList == NULL) {
        ogs_error("OpenAPI_mb_upf_info_convertToJSON() failed [interface_mb_upf_info_list]");
        goto end;
    }
    OpenAPI_list_for_each(mb_upf_info->interface_mb_upf_info_list, node) {
        cJSON *itemLocal = OpenAPI_interface_upf_info_item_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_mb_upf_info_convertToJSON() failed [interface_mb_upf_info_list]");
            goto end;
        }
        cJSON_AddItemToArray(interface_mb_upf_info_listList, itemLocal);
    }
    }

    if (mb_upf_info->tai_list) {
    cJSON *tai_listList = cJSON_AddArrayToObject(item, "taiList");
    if (tai_listList == NULL) {
        ogs_error("OpenAPI_mb_upf_info_convertToJSON() failed [tai_list]");
        goto end;
    }
    OpenAPI_list_for_each(mb_upf_info->tai_list, node) {
        cJSON *itemLocal = OpenAPI_tai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_mb_upf_info_convertToJSON() failed [tai_list]");
            goto end;
        }
        cJSON_AddItemToArray(tai_listList, itemLocal);
    }
    }

    if (mb_upf_info->tai_range_list) {
    cJSON *tai_range_listList = cJSON_AddArrayToObject(item, "taiRangeList");
    if (tai_range_listList == NULL) {
        ogs_error("OpenAPI_mb_upf_info_convertToJSON() failed [tai_range_list]");
        goto end;
    }
    OpenAPI_list_for_each(mb_upf_info->tai_range_list, node) {
        cJSON *itemLocal = OpenAPI_tai_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_mb_upf_info_convertToJSON() failed [tai_range_list]");
            goto end;
        }
        cJSON_AddItemToArray(tai_range_listList, itemLocal);
    }
    }

    if (mb_upf_info->is_priority) {
    if (cJSON_AddNumberToObject(item, "priority", mb_upf_info->priority) == NULL) {
        ogs_error("OpenAPI_mb_upf_info_convertToJSON() failed [priority]");
        goto end;
    }
    }

    if (mb_upf_info->supported_pfcp_features) {
    if (cJSON_AddStringToObject(item, "supportedPfcpFeatures", mb_upf_info->supported_pfcp_features) == NULL) {
        ogs_error("OpenAPI_mb_upf_info_convertToJSON() failed [supported_pfcp_features]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_mb_upf_info_t *OpenAPI_mb_upf_info_parseFromJSON(cJSON *mb_upf_infoJSON)
{
    OpenAPI_mb_upf_info_t *mb_upf_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *s_nssai_mb_upf_info_list = NULL;
    OpenAPI_list_t *s_nssai_mb_upf_info_listList = NULL;
    cJSON *mb_smf_serving_area = NULL;
    OpenAPI_list_t *mb_smf_serving_areaList = NULL;
    cJSON *interface_mb_upf_info_list = NULL;
    OpenAPI_list_t *interface_mb_upf_info_listList = NULL;
    cJSON *tai_list = NULL;
    OpenAPI_list_t *tai_listList = NULL;
    cJSON *tai_range_list = NULL;
    OpenAPI_list_t *tai_range_listList = NULL;
    cJSON *priority = NULL;
    cJSON *supported_pfcp_features = NULL;
    s_nssai_mb_upf_info_list = cJSON_GetObjectItemCaseSensitive(mb_upf_infoJSON, "sNssaiMbUpfInfoList");
    if (!s_nssai_mb_upf_info_list) {
        ogs_error("OpenAPI_mb_upf_info_parseFromJSON() failed [s_nssai_mb_upf_info_list]");
        goto end;
    }
        cJSON *s_nssai_mb_upf_info_list_local = NULL;
        if (!cJSON_IsArray(s_nssai_mb_upf_info_list)) {
            ogs_error("OpenAPI_mb_upf_info_parseFromJSON() failed [s_nssai_mb_upf_info_list]");
            goto end;
        }

        s_nssai_mb_upf_info_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(s_nssai_mb_upf_info_list_local, s_nssai_mb_upf_info_list) {
            if (!cJSON_IsObject(s_nssai_mb_upf_info_list_local)) {
                ogs_error("OpenAPI_mb_upf_info_parseFromJSON() failed [s_nssai_mb_upf_info_list]");
                goto end;
            }
            OpenAPI_snssai_upf_info_item_t *s_nssai_mb_upf_info_listItem = OpenAPI_snssai_upf_info_item_parseFromJSON(s_nssai_mb_upf_info_list_local);
            if (!s_nssai_mb_upf_info_listItem) {
                ogs_error("No s_nssai_mb_upf_info_listItem");
                goto end;
            }
            OpenAPI_list_add(s_nssai_mb_upf_info_listList, s_nssai_mb_upf_info_listItem);
        }

    mb_smf_serving_area = cJSON_GetObjectItemCaseSensitive(mb_upf_infoJSON, "mbSmfServingArea");
    if (mb_smf_serving_area) {
        cJSON *mb_smf_serving_area_local = NULL;
        if (!cJSON_IsArray(mb_smf_serving_area)) {
            ogs_error("OpenAPI_mb_upf_info_parseFromJSON() failed [mb_smf_serving_area]");
            goto end;
        }

        mb_smf_serving_areaList = OpenAPI_list_create();

        cJSON_ArrayForEach(mb_smf_serving_area_local, mb_smf_serving_area) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(mb_smf_serving_area_local)) {
                ogs_error("OpenAPI_mb_upf_info_parseFromJSON() failed [mb_smf_serving_area]");
                goto end;
            }
            OpenAPI_list_add(mb_smf_serving_areaList, ogs_strdup(mb_smf_serving_area_local->valuestring));
        }
    }

    interface_mb_upf_info_list = cJSON_GetObjectItemCaseSensitive(mb_upf_infoJSON, "interfaceMbUpfInfoList");
    if (interface_mb_upf_info_list) {
        cJSON *interface_mb_upf_info_list_local = NULL;
        if (!cJSON_IsArray(interface_mb_upf_info_list)) {
            ogs_error("OpenAPI_mb_upf_info_parseFromJSON() failed [interface_mb_upf_info_list]");
            goto end;
        }

        interface_mb_upf_info_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(interface_mb_upf_info_list_local, interface_mb_upf_info_list) {
            if (!cJSON_IsObject(interface_mb_upf_info_list_local)) {
                ogs_error("OpenAPI_mb_upf_info_parseFromJSON() failed [interface_mb_upf_info_list]");
                goto end;
            }
            OpenAPI_interface_upf_info_item_t *interface_mb_upf_info_listItem = OpenAPI_interface_upf_info_item_parseFromJSON(interface_mb_upf_info_list_local);
            if (!interface_mb_upf_info_listItem) {
                ogs_error("No interface_mb_upf_info_listItem");
                goto end;
            }
            OpenAPI_list_add(interface_mb_upf_info_listList, interface_mb_upf_info_listItem);
        }
    }

    tai_list = cJSON_GetObjectItemCaseSensitive(mb_upf_infoJSON, "taiList");
    if (tai_list) {
        cJSON *tai_list_local = NULL;
        if (!cJSON_IsArray(tai_list)) {
            ogs_error("OpenAPI_mb_upf_info_parseFromJSON() failed [tai_list]");
            goto end;
        }

        tai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tai_list_local, tai_list) {
            if (!cJSON_IsObject(tai_list_local)) {
                ogs_error("OpenAPI_mb_upf_info_parseFromJSON() failed [tai_list]");
                goto end;
            }
            OpenAPI_tai_t *tai_listItem = OpenAPI_tai_parseFromJSON(tai_list_local);
            if (!tai_listItem) {
                ogs_error("No tai_listItem");
                goto end;
            }
            OpenAPI_list_add(tai_listList, tai_listItem);
        }
    }

    tai_range_list = cJSON_GetObjectItemCaseSensitive(mb_upf_infoJSON, "taiRangeList");
    if (tai_range_list) {
        cJSON *tai_range_list_local = NULL;
        if (!cJSON_IsArray(tai_range_list)) {
            ogs_error("OpenAPI_mb_upf_info_parseFromJSON() failed [tai_range_list]");
            goto end;
        }

        tai_range_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tai_range_list_local, tai_range_list) {
            if (!cJSON_IsObject(tai_range_list_local)) {
                ogs_error("OpenAPI_mb_upf_info_parseFromJSON() failed [tai_range_list]");
                goto end;
            }
            OpenAPI_tai_range_t *tai_range_listItem = OpenAPI_tai_range_parseFromJSON(tai_range_list_local);
            if (!tai_range_listItem) {
                ogs_error("No tai_range_listItem");
                goto end;
            }
            OpenAPI_list_add(tai_range_listList, tai_range_listItem);
        }
    }

    priority = cJSON_GetObjectItemCaseSensitive(mb_upf_infoJSON, "priority");
    if (priority) {
    if (!cJSON_IsNumber(priority)) {
        ogs_error("OpenAPI_mb_upf_info_parseFromJSON() failed [priority]");
        goto end;
    }
    }

    supported_pfcp_features = cJSON_GetObjectItemCaseSensitive(mb_upf_infoJSON, "supportedPfcpFeatures");
    if (supported_pfcp_features) {
    if (!cJSON_IsString(supported_pfcp_features) && !cJSON_IsNull(supported_pfcp_features)) {
        ogs_error("OpenAPI_mb_upf_info_parseFromJSON() failed [supported_pfcp_features]");
        goto end;
    }
    }

    mb_upf_info_local_var = OpenAPI_mb_upf_info_create (
        s_nssai_mb_upf_info_listList,
        mb_smf_serving_area ? mb_smf_serving_areaList : NULL,
        interface_mb_upf_info_list ? interface_mb_upf_info_listList : NULL,
        tai_list ? tai_listList : NULL,
        tai_range_list ? tai_range_listList : NULL,
        priority ? true : false,
        priority ? priority->valuedouble : 0,
        supported_pfcp_features && !cJSON_IsNull(supported_pfcp_features) ? ogs_strdup(supported_pfcp_features->valuestring) : NULL
    );

    return mb_upf_info_local_var;
end:
    if (s_nssai_mb_upf_info_listList) {
        OpenAPI_list_for_each(s_nssai_mb_upf_info_listList, node) {
            OpenAPI_snssai_upf_info_item_free(node->data);
        }
        OpenAPI_list_free(s_nssai_mb_upf_info_listList);
        s_nssai_mb_upf_info_listList = NULL;
    }
    if (mb_smf_serving_areaList) {
        OpenAPI_list_for_each(mb_smf_serving_areaList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(mb_smf_serving_areaList);
        mb_smf_serving_areaList = NULL;
    }
    if (interface_mb_upf_info_listList) {
        OpenAPI_list_for_each(interface_mb_upf_info_listList, node) {
            OpenAPI_interface_upf_info_item_free(node->data);
        }
        OpenAPI_list_free(interface_mb_upf_info_listList);
        interface_mb_upf_info_listList = NULL;
    }
    if (tai_listList) {
        OpenAPI_list_for_each(tai_listList, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(tai_listList);
        tai_listList = NULL;
    }
    if (tai_range_listList) {
        OpenAPI_list_for_each(tai_range_listList, node) {
            OpenAPI_tai_range_free(node->data);
        }
        OpenAPI_list_free(tai_range_listList);
        tai_range_listList = NULL;
    }
    return NULL;
}

OpenAPI_mb_upf_info_t *OpenAPI_mb_upf_info_copy(OpenAPI_mb_upf_info_t *dst, OpenAPI_mb_upf_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_mb_upf_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_mb_upf_info_convertToJSON() failed");
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

    OpenAPI_mb_upf_info_free(dst);
    dst = OpenAPI_mb_upf_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

