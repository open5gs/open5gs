
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "upu_info.h"

OpenAPI_upu_info_t *OpenAPI_upu_info_create(
    OpenAPI_list_t *upu_data_list,
    bool is_upu_reg_ind,
    int upu_reg_ind,
    bool is_upu_ack_ind,
    int upu_ack_ind,
    char *upu_mac_iausf,
    char *counter_upu,
    char *provisioning_time,
    char *upu_transparent_container
)
{
    OpenAPI_upu_info_t *upu_info_local_var = ogs_malloc(sizeof(OpenAPI_upu_info_t));
    ogs_assert(upu_info_local_var);

    upu_info_local_var->upu_data_list = upu_data_list;
    upu_info_local_var->is_upu_reg_ind = is_upu_reg_ind;
    upu_info_local_var->upu_reg_ind = upu_reg_ind;
    upu_info_local_var->is_upu_ack_ind = is_upu_ack_ind;
    upu_info_local_var->upu_ack_ind = upu_ack_ind;
    upu_info_local_var->upu_mac_iausf = upu_mac_iausf;
    upu_info_local_var->counter_upu = counter_upu;
    upu_info_local_var->provisioning_time = provisioning_time;
    upu_info_local_var->upu_transparent_container = upu_transparent_container;

    return upu_info_local_var;
}

void OpenAPI_upu_info_free(OpenAPI_upu_info_t *upu_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == upu_info) {
        return;
    }
    if (upu_info->upu_data_list) {
        OpenAPI_list_for_each(upu_info->upu_data_list, node) {
            OpenAPI_upu_data_free(node->data);
        }
        OpenAPI_list_free(upu_info->upu_data_list);
        upu_info->upu_data_list = NULL;
    }
    if (upu_info->upu_mac_iausf) {
        ogs_free(upu_info->upu_mac_iausf);
        upu_info->upu_mac_iausf = NULL;
    }
    if (upu_info->counter_upu) {
        ogs_free(upu_info->counter_upu);
        upu_info->counter_upu = NULL;
    }
    if (upu_info->provisioning_time) {
        ogs_free(upu_info->provisioning_time);
        upu_info->provisioning_time = NULL;
    }
    if (upu_info->upu_transparent_container) {
        ogs_free(upu_info->upu_transparent_container);
        upu_info->upu_transparent_container = NULL;
    }
    ogs_free(upu_info);
}

cJSON *OpenAPI_upu_info_convertToJSON(OpenAPI_upu_info_t *upu_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (upu_info == NULL) {
        ogs_error("OpenAPI_upu_info_convertToJSON() failed [UpuInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (upu_info->upu_data_list) {
    cJSON *upu_data_listList = cJSON_AddArrayToObject(item, "upuDataList");
    if (upu_data_listList == NULL) {
        ogs_error("OpenAPI_upu_info_convertToJSON() failed [upu_data_list]");
        goto end;
    }
    OpenAPI_list_for_each(upu_info->upu_data_list, node) {
        cJSON *itemLocal = OpenAPI_upu_data_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_upu_info_convertToJSON() failed [upu_data_list]");
            goto end;
        }
        cJSON_AddItemToArray(upu_data_listList, itemLocal);
    }
    }

    if (upu_info->is_upu_reg_ind) {
    if (cJSON_AddBoolToObject(item, "upuRegInd", upu_info->upu_reg_ind) == NULL) {
        ogs_error("OpenAPI_upu_info_convertToJSON() failed [upu_reg_ind]");
        goto end;
    }
    }

    if (upu_info->is_upu_ack_ind) {
    if (cJSON_AddBoolToObject(item, "upuAckInd", upu_info->upu_ack_ind) == NULL) {
        ogs_error("OpenAPI_upu_info_convertToJSON() failed [upu_ack_ind]");
        goto end;
    }
    }

    if (upu_info->upu_mac_iausf) {
    if (cJSON_AddStringToObject(item, "upuMacIausf", upu_info->upu_mac_iausf) == NULL) {
        ogs_error("OpenAPI_upu_info_convertToJSON() failed [upu_mac_iausf]");
        goto end;
    }
    }

    if (upu_info->counter_upu) {
    if (cJSON_AddStringToObject(item, "counterUpu", upu_info->counter_upu) == NULL) {
        ogs_error("OpenAPI_upu_info_convertToJSON() failed [counter_upu]");
        goto end;
    }
    }

    if (!upu_info->provisioning_time) {
        ogs_error("OpenAPI_upu_info_convertToJSON() failed [provisioning_time]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "provisioningTime", upu_info->provisioning_time) == NULL) {
        ogs_error("OpenAPI_upu_info_convertToJSON() failed [provisioning_time]");
        goto end;
    }

    if (upu_info->upu_transparent_container) {
    if (cJSON_AddStringToObject(item, "upuTransparentContainer", upu_info->upu_transparent_container) == NULL) {
        ogs_error("OpenAPI_upu_info_convertToJSON() failed [upu_transparent_container]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_upu_info_t *OpenAPI_upu_info_parseFromJSON(cJSON *upu_infoJSON)
{
    OpenAPI_upu_info_t *upu_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *upu_data_list = NULL;
    OpenAPI_list_t *upu_data_listList = NULL;
    cJSON *upu_reg_ind = NULL;
    cJSON *upu_ack_ind = NULL;
    cJSON *upu_mac_iausf = NULL;
    cJSON *counter_upu = NULL;
    cJSON *provisioning_time = NULL;
    cJSON *upu_transparent_container = NULL;
    upu_data_list = cJSON_GetObjectItemCaseSensitive(upu_infoJSON, "upuDataList");
    if (upu_data_list) {
        cJSON *upu_data_list_local = NULL;
        if (!cJSON_IsArray(upu_data_list)) {
            ogs_error("OpenAPI_upu_info_parseFromJSON() failed [upu_data_list]");
            goto end;
        }

        upu_data_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(upu_data_list_local, upu_data_list) {
            if (!cJSON_IsObject(upu_data_list_local)) {
                ogs_error("OpenAPI_upu_info_parseFromJSON() failed [upu_data_list]");
                goto end;
            }
            OpenAPI_upu_data_t *upu_data_listItem = OpenAPI_upu_data_parseFromJSON(upu_data_list_local);
            if (!upu_data_listItem) {
                ogs_error("No upu_data_listItem");
                goto end;
            }
            OpenAPI_list_add(upu_data_listList, upu_data_listItem);
        }
    }

    upu_reg_ind = cJSON_GetObjectItemCaseSensitive(upu_infoJSON, "upuRegInd");
    if (upu_reg_ind) {
    if (!cJSON_IsBool(upu_reg_ind)) {
        ogs_error("OpenAPI_upu_info_parseFromJSON() failed [upu_reg_ind]");
        goto end;
    }
    }

    upu_ack_ind = cJSON_GetObjectItemCaseSensitive(upu_infoJSON, "upuAckInd");
    if (upu_ack_ind) {
    if (!cJSON_IsBool(upu_ack_ind)) {
        ogs_error("OpenAPI_upu_info_parseFromJSON() failed [upu_ack_ind]");
        goto end;
    }
    }

    upu_mac_iausf = cJSON_GetObjectItemCaseSensitive(upu_infoJSON, "upuMacIausf");
    if (upu_mac_iausf) {
    if (!cJSON_IsString(upu_mac_iausf) && !cJSON_IsNull(upu_mac_iausf)) {
        ogs_error("OpenAPI_upu_info_parseFromJSON() failed [upu_mac_iausf]");
        goto end;
    }
    }

    counter_upu = cJSON_GetObjectItemCaseSensitive(upu_infoJSON, "counterUpu");
    if (counter_upu) {
    if (!cJSON_IsString(counter_upu) && !cJSON_IsNull(counter_upu)) {
        ogs_error("OpenAPI_upu_info_parseFromJSON() failed [counter_upu]");
        goto end;
    }
    }

    provisioning_time = cJSON_GetObjectItemCaseSensitive(upu_infoJSON, "provisioningTime");
    if (!provisioning_time) {
        ogs_error("OpenAPI_upu_info_parseFromJSON() failed [provisioning_time]");
        goto end;
    }
    if (!cJSON_IsString(provisioning_time) && !cJSON_IsNull(provisioning_time)) {
        ogs_error("OpenAPI_upu_info_parseFromJSON() failed [provisioning_time]");
        goto end;
    }

    upu_transparent_container = cJSON_GetObjectItemCaseSensitive(upu_infoJSON, "upuTransparentContainer");
    if (upu_transparent_container) {
    if (!cJSON_IsString(upu_transparent_container) && !cJSON_IsNull(upu_transparent_container)) {
        ogs_error("OpenAPI_upu_info_parseFromJSON() failed [upu_transparent_container]");
        goto end;
    }
    }

    upu_info_local_var = OpenAPI_upu_info_create (
        upu_data_list ? upu_data_listList : NULL,
        upu_reg_ind ? true : false,
        upu_reg_ind ? upu_reg_ind->valueint : 0,
        upu_ack_ind ? true : false,
        upu_ack_ind ? upu_ack_ind->valueint : 0,
        upu_mac_iausf && !cJSON_IsNull(upu_mac_iausf) ? ogs_strdup(upu_mac_iausf->valuestring) : NULL,
        counter_upu && !cJSON_IsNull(counter_upu) ? ogs_strdup(counter_upu->valuestring) : NULL,
        ogs_strdup(provisioning_time->valuestring),
        upu_transparent_container && !cJSON_IsNull(upu_transparent_container) ? ogs_strdup(upu_transparent_container->valuestring) : NULL
    );

    return upu_info_local_var;
end:
    if (upu_data_listList) {
        OpenAPI_list_for_each(upu_data_listList, node) {
            OpenAPI_upu_data_free(node->data);
        }
        OpenAPI_list_free(upu_data_listList);
        upu_data_listList = NULL;
    }
    return NULL;
}

OpenAPI_upu_info_t *OpenAPI_upu_info_copy(OpenAPI_upu_info_t *dst, OpenAPI_upu_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_upu_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_upu_info_convertToJSON() failed");
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

    OpenAPI_upu_info_free(dst);
    dst = OpenAPI_upu_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

