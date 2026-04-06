
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "smsf_info.h"

OpenAPI_smsf_info_t *OpenAPI_smsf_info_create(
    bool is_roaming_ue_ind,
    int roaming_ue_ind,
    OpenAPI_list_t *remote_plmn_range_list
)
{
    OpenAPI_smsf_info_t *smsf_info_local_var = ogs_malloc(sizeof(OpenAPI_smsf_info_t));
    ogs_assert(smsf_info_local_var);

    smsf_info_local_var->is_roaming_ue_ind = is_roaming_ue_ind;
    smsf_info_local_var->roaming_ue_ind = roaming_ue_ind;
    smsf_info_local_var->remote_plmn_range_list = remote_plmn_range_list;

    return smsf_info_local_var;
}

void OpenAPI_smsf_info_free(OpenAPI_smsf_info_t *smsf_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == smsf_info) {
        return;
    }
    if (smsf_info->remote_plmn_range_list) {
        OpenAPI_list_for_each(smsf_info->remote_plmn_range_list, node) {
            OpenAPI_plmn_range_free(node->data);
        }
        OpenAPI_list_free(smsf_info->remote_plmn_range_list);
        smsf_info->remote_plmn_range_list = NULL;
    }
    ogs_free(smsf_info);
}

cJSON *OpenAPI_smsf_info_convertToJSON(OpenAPI_smsf_info_t *smsf_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (smsf_info == NULL) {
        ogs_error("OpenAPI_smsf_info_convertToJSON() failed [SmsfInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (smsf_info->is_roaming_ue_ind) {
    if (cJSON_AddBoolToObject(item, "roamingUeInd", smsf_info->roaming_ue_ind) == NULL) {
        ogs_error("OpenAPI_smsf_info_convertToJSON() failed [roaming_ue_ind]");
        goto end;
    }
    }

    if (smsf_info->remote_plmn_range_list) {
    cJSON *remote_plmn_range_listList = cJSON_AddArrayToObject(item, "remotePlmnRangeList");
    if (remote_plmn_range_listList == NULL) {
        ogs_error("OpenAPI_smsf_info_convertToJSON() failed [remote_plmn_range_list]");
        goto end;
    }
    OpenAPI_list_for_each(smsf_info->remote_plmn_range_list, node) {
        cJSON *itemLocal = OpenAPI_plmn_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_smsf_info_convertToJSON() failed [remote_plmn_range_list]");
            goto end;
        }
        cJSON_AddItemToArray(remote_plmn_range_listList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_smsf_info_t *OpenAPI_smsf_info_parseFromJSON(cJSON *smsf_infoJSON)
{
    OpenAPI_smsf_info_t *smsf_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *roaming_ue_ind = NULL;
    cJSON *remote_plmn_range_list = NULL;
    OpenAPI_list_t *remote_plmn_range_listList = NULL;
    roaming_ue_ind = cJSON_GetObjectItemCaseSensitive(smsf_infoJSON, "roamingUeInd");
    if (roaming_ue_ind) {
    if (!cJSON_IsBool(roaming_ue_ind)) {
        ogs_error("OpenAPI_smsf_info_parseFromJSON() failed [roaming_ue_ind]");
        goto end;
    }
    }

    remote_plmn_range_list = cJSON_GetObjectItemCaseSensitive(smsf_infoJSON, "remotePlmnRangeList");
    if (remote_plmn_range_list) {
        cJSON *remote_plmn_range_list_local = NULL;
        if (!cJSON_IsArray(remote_plmn_range_list)) {
            ogs_error("OpenAPI_smsf_info_parseFromJSON() failed [remote_plmn_range_list]");
            goto end;
        }

        remote_plmn_range_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(remote_plmn_range_list_local, remote_plmn_range_list) {
            if (!cJSON_IsObject(remote_plmn_range_list_local)) {
                ogs_error("OpenAPI_smsf_info_parseFromJSON() failed [remote_plmn_range_list]");
                goto end;
            }
            OpenAPI_plmn_range_t *remote_plmn_range_listItem = OpenAPI_plmn_range_parseFromJSON(remote_plmn_range_list_local);
            if (!remote_plmn_range_listItem) {
                ogs_error("No remote_plmn_range_listItem");
                goto end;
            }
            OpenAPI_list_add(remote_plmn_range_listList, remote_plmn_range_listItem);
        }
    }

    smsf_info_local_var = OpenAPI_smsf_info_create (
        roaming_ue_ind ? true : false,
        roaming_ue_ind ? roaming_ue_ind->valueint : 0,
        remote_plmn_range_list ? remote_plmn_range_listList : NULL
    );

    return smsf_info_local_var;
end:
    if (remote_plmn_range_listList) {
        OpenAPI_list_for_each(remote_plmn_range_listList, node) {
            OpenAPI_plmn_range_free(node->data);
        }
        OpenAPI_list_free(remote_plmn_range_listList);
        remote_plmn_range_listList = NULL;
    }
    return NULL;
}

OpenAPI_smsf_info_t *OpenAPI_smsf_info_copy(OpenAPI_smsf_info_t *dst, OpenAPI_smsf_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_smsf_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_smsf_info_convertToJSON() failed");
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

    OpenAPI_smsf_info_free(dst);
    dst = OpenAPI_smsf_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

