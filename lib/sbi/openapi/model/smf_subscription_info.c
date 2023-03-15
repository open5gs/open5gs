
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "smf_subscription_info.h"

OpenAPI_smf_subscription_info_t *OpenAPI_smf_subscription_info_create(
    OpenAPI_list_t *smf_subscription_list
)
{
    OpenAPI_smf_subscription_info_t *smf_subscription_info_local_var = ogs_malloc(sizeof(OpenAPI_smf_subscription_info_t));
    ogs_assert(smf_subscription_info_local_var);

    smf_subscription_info_local_var->smf_subscription_list = smf_subscription_list;

    return smf_subscription_info_local_var;
}

void OpenAPI_smf_subscription_info_free(OpenAPI_smf_subscription_info_t *smf_subscription_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == smf_subscription_info) {
        return;
    }
    if (smf_subscription_info->smf_subscription_list) {
        OpenAPI_list_for_each(smf_subscription_info->smf_subscription_list, node) {
            OpenAPI_smf_subscription_item_free(node->data);
        }
        OpenAPI_list_free(smf_subscription_info->smf_subscription_list);
        smf_subscription_info->smf_subscription_list = NULL;
    }
    ogs_free(smf_subscription_info);
}

cJSON *OpenAPI_smf_subscription_info_convertToJSON(OpenAPI_smf_subscription_info_t *smf_subscription_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (smf_subscription_info == NULL) {
        ogs_error("OpenAPI_smf_subscription_info_convertToJSON() failed [SmfSubscriptionInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!smf_subscription_info->smf_subscription_list) {
        ogs_error("OpenAPI_smf_subscription_info_convertToJSON() failed [smf_subscription_list]");
        return NULL;
    }
    cJSON *smf_subscription_listList = cJSON_AddArrayToObject(item, "smfSubscriptionList");
    if (smf_subscription_listList == NULL) {
        ogs_error("OpenAPI_smf_subscription_info_convertToJSON() failed [smf_subscription_list]");
        goto end;
    }
    OpenAPI_list_for_each(smf_subscription_info->smf_subscription_list, node) {
        cJSON *itemLocal = OpenAPI_smf_subscription_item_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_smf_subscription_info_convertToJSON() failed [smf_subscription_list]");
            goto end;
        }
        cJSON_AddItemToArray(smf_subscription_listList, itemLocal);
    }

end:
    return item;
}

OpenAPI_smf_subscription_info_t *OpenAPI_smf_subscription_info_parseFromJSON(cJSON *smf_subscription_infoJSON)
{
    OpenAPI_smf_subscription_info_t *smf_subscription_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *smf_subscription_list = NULL;
    OpenAPI_list_t *smf_subscription_listList = NULL;
    smf_subscription_list = cJSON_GetObjectItemCaseSensitive(smf_subscription_infoJSON, "smfSubscriptionList");
    if (!smf_subscription_list) {
        ogs_error("OpenAPI_smf_subscription_info_parseFromJSON() failed [smf_subscription_list]");
        goto end;
    }
        cJSON *smf_subscription_list_local = NULL;
        if (!cJSON_IsArray(smf_subscription_list)) {
            ogs_error("OpenAPI_smf_subscription_info_parseFromJSON() failed [smf_subscription_list]");
            goto end;
        }

        smf_subscription_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(smf_subscription_list_local, smf_subscription_list) {
            if (!cJSON_IsObject(smf_subscription_list_local)) {
                ogs_error("OpenAPI_smf_subscription_info_parseFromJSON() failed [smf_subscription_list]");
                goto end;
            }
            OpenAPI_smf_subscription_item_t *smf_subscription_listItem = OpenAPI_smf_subscription_item_parseFromJSON(smf_subscription_list_local);
            if (!smf_subscription_listItem) {
                ogs_error("No smf_subscription_listItem");
                goto end;
            }
            OpenAPI_list_add(smf_subscription_listList, smf_subscription_listItem);
        }

    smf_subscription_info_local_var = OpenAPI_smf_subscription_info_create (
        smf_subscription_listList
    );

    return smf_subscription_info_local_var;
end:
    if (smf_subscription_listList) {
        OpenAPI_list_for_each(smf_subscription_listList, node) {
            OpenAPI_smf_subscription_item_free(node->data);
        }
        OpenAPI_list_free(smf_subscription_listList);
        smf_subscription_listList = NULL;
    }
    return NULL;
}

OpenAPI_smf_subscription_info_t *OpenAPI_smf_subscription_info_copy(OpenAPI_smf_subscription_info_t *dst, OpenAPI_smf_subscription_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_smf_subscription_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_smf_subscription_info_convertToJSON() failed");
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

    OpenAPI_smf_subscription_info_free(dst);
    dst = OpenAPI_smf_subscription_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

