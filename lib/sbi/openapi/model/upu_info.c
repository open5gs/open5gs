
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "upu_info.h"

OpenAPI_upu_info_t *OpenAPI_upu_info_create(
    OpenAPI_list_t *upu_data_list,
    int upu_reg_ind,
    int upu_ack_ind,
    char *upu_mac_iausf,
    char *counter_upu,
    char *provisioning_time
)
{
    OpenAPI_upu_info_t *upu_info_local_var = ogs_malloc(sizeof(OpenAPI_upu_info_t));
    ogs_assert(upu_info_local_var);

    upu_info_local_var->upu_data_list = upu_data_list;
    upu_info_local_var->upu_reg_ind = upu_reg_ind;
    upu_info_local_var->upu_ack_ind = upu_ack_ind;
    upu_info_local_var->upu_mac_iausf = upu_mac_iausf;
    upu_info_local_var->counter_upu = counter_upu;
    upu_info_local_var->provisioning_time = provisioning_time;

    return upu_info_local_var;
}

void OpenAPI_upu_info_free(OpenAPI_upu_info_t *upu_info)
{
    if (NULL == upu_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(upu_info->upu_data_list, node) {
        OpenAPI_upu_data_1_free(node->data);
    }
    OpenAPI_list_free(upu_info->upu_data_list);
    ogs_free(upu_info->upu_mac_iausf);
    ogs_free(upu_info->counter_upu);
    ogs_free(upu_info->provisioning_time);
    ogs_free(upu_info);
}

cJSON *OpenAPI_upu_info_convertToJSON(OpenAPI_upu_info_t *upu_info)
{
    cJSON *item = NULL;

    if (upu_info == NULL) {
        ogs_error("OpenAPI_upu_info_convertToJSON() failed [UpuInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    cJSON *upu_data_listList = cJSON_AddArrayToObject(item, "upuDataList");
    if (upu_data_listList == NULL) {
        ogs_error("OpenAPI_upu_info_convertToJSON() failed [upu_data_list]");
        goto end;
    }

    OpenAPI_lnode_t *upu_data_list_node;
    if (upu_info->upu_data_list) {
        OpenAPI_list_for_each(upu_info->upu_data_list, upu_data_list_node) {
            cJSON *itemLocal = OpenAPI_upu_data_1_convertToJSON(upu_data_list_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_upu_info_convertToJSON() failed [upu_data_list]");
                goto end;
            }
            cJSON_AddItemToArray(upu_data_listList, itemLocal);
        }
    }

    if (cJSON_AddBoolToObject(item, "upuRegInd", upu_info->upu_reg_ind) == NULL) {
        ogs_error("OpenAPI_upu_info_convertToJSON() failed [upu_reg_ind]");
        goto end;
    }

    if (cJSON_AddBoolToObject(item, "upuAckInd", upu_info->upu_ack_ind) == NULL) {
        ogs_error("OpenAPI_upu_info_convertToJSON() failed [upu_ack_ind]");
        goto end;
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

    if (cJSON_AddStringToObject(item, "provisioningTime", upu_info->provisioning_time) == NULL) {
        ogs_error("OpenAPI_upu_info_convertToJSON() failed [provisioning_time]");
        goto end;
    }

end:
    return item;
}

OpenAPI_upu_info_t *OpenAPI_upu_info_parseFromJSON(cJSON *upu_infoJSON)
{
    OpenAPI_upu_info_t *upu_info_local_var = NULL;
    cJSON *upu_data_list = cJSON_GetObjectItemCaseSensitive(upu_infoJSON, "upuDataList");
    if (!upu_data_list) {
        ogs_error("OpenAPI_upu_info_parseFromJSON() failed [upu_data_list]");
        goto end;
    }

    OpenAPI_list_t *upu_data_listList;
    cJSON *upu_data_list_local_nonprimitive;
    if (!cJSON_IsArray(upu_data_list)){
        ogs_error("OpenAPI_upu_info_parseFromJSON() failed [upu_data_list]");
        goto end;
    }

    upu_data_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(upu_data_list_local_nonprimitive, upu_data_list ) {
        if (!cJSON_IsObject(upu_data_list_local_nonprimitive)) {
            ogs_error("OpenAPI_upu_info_parseFromJSON() failed [upu_data_list]");
            goto end;
        }
        OpenAPI_upu_data_1_t *upu_data_listItem = OpenAPI_upu_data_1_parseFromJSON(upu_data_list_local_nonprimitive);

        if (!upu_data_listItem) {
            ogs_error("No upu_data_listItem");
            OpenAPI_list_free(upu_data_listList);
            goto end;
        }

        OpenAPI_list_add(upu_data_listList, upu_data_listItem);
    }

    cJSON *upu_reg_ind = cJSON_GetObjectItemCaseSensitive(upu_infoJSON, "upuRegInd");
    if (!upu_reg_ind) {
        ogs_error("OpenAPI_upu_info_parseFromJSON() failed [upu_reg_ind]");
        goto end;
    }

    if (!cJSON_IsBool(upu_reg_ind)) {
        ogs_error("OpenAPI_upu_info_parseFromJSON() failed [upu_reg_ind]");
        goto end;
    }

    cJSON *upu_ack_ind = cJSON_GetObjectItemCaseSensitive(upu_infoJSON, "upuAckInd");
    if (!upu_ack_ind) {
        ogs_error("OpenAPI_upu_info_parseFromJSON() failed [upu_ack_ind]");
        goto end;
    }

    if (!cJSON_IsBool(upu_ack_ind)) {
        ogs_error("OpenAPI_upu_info_parseFromJSON() failed [upu_ack_ind]");
        goto end;
    }

    cJSON *upu_mac_iausf = cJSON_GetObjectItemCaseSensitive(upu_infoJSON, "upuMacIausf");

    if (upu_mac_iausf) {
    if (!cJSON_IsString(upu_mac_iausf)) {
        ogs_error("OpenAPI_upu_info_parseFromJSON() failed [upu_mac_iausf]");
        goto end;
    }
    }

    cJSON *counter_upu = cJSON_GetObjectItemCaseSensitive(upu_infoJSON, "counterUpu");

    if (counter_upu) {
    if (!cJSON_IsString(counter_upu)) {
        ogs_error("OpenAPI_upu_info_parseFromJSON() failed [counter_upu]");
        goto end;
    }
    }

    cJSON *provisioning_time = cJSON_GetObjectItemCaseSensitive(upu_infoJSON, "provisioningTime");
    if (!provisioning_time) {
        ogs_error("OpenAPI_upu_info_parseFromJSON() failed [provisioning_time]");
        goto end;
    }

    if (!cJSON_IsString(provisioning_time)) {
        ogs_error("OpenAPI_upu_info_parseFromJSON() failed [provisioning_time]");
        goto end;
    }

    upu_info_local_var = OpenAPI_upu_info_create (
        upu_data_listList,
        
        upu_reg_ind->valueint,
        
        upu_ack_ind->valueint,
        upu_mac_iausf ? ogs_strdup(upu_mac_iausf->valuestring) : NULL,
        counter_upu ? ogs_strdup(counter_upu->valuestring) : NULL,
        ogs_strdup(provisioning_time->valuestring)
    );

    return upu_info_local_var;
end:
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

