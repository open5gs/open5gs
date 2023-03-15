
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "additional_ee_subs_info.h"

OpenAPI_additional_ee_subs_info_t *OpenAPI_additional_ee_subs_info_create(
    OpenAPI_list_t *amf_subscription_info_list,
    OpenAPI_smf_subscription_info_t *smf_subscription_info,
    OpenAPI_hss_subscription_info_t *hss_subscription_info
)
{
    OpenAPI_additional_ee_subs_info_t *additional_ee_subs_info_local_var = ogs_malloc(sizeof(OpenAPI_additional_ee_subs_info_t));
    ogs_assert(additional_ee_subs_info_local_var);

    additional_ee_subs_info_local_var->amf_subscription_info_list = amf_subscription_info_list;
    additional_ee_subs_info_local_var->smf_subscription_info = smf_subscription_info;
    additional_ee_subs_info_local_var->hss_subscription_info = hss_subscription_info;

    return additional_ee_subs_info_local_var;
}

void OpenAPI_additional_ee_subs_info_free(OpenAPI_additional_ee_subs_info_t *additional_ee_subs_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == additional_ee_subs_info) {
        return;
    }
    if (additional_ee_subs_info->amf_subscription_info_list) {
        OpenAPI_list_for_each(additional_ee_subs_info->amf_subscription_info_list, node) {
            OpenAPI_amf_subscription_info_free(node->data);
        }
        OpenAPI_list_free(additional_ee_subs_info->amf_subscription_info_list);
        additional_ee_subs_info->amf_subscription_info_list = NULL;
    }
    if (additional_ee_subs_info->smf_subscription_info) {
        OpenAPI_smf_subscription_info_free(additional_ee_subs_info->smf_subscription_info);
        additional_ee_subs_info->smf_subscription_info = NULL;
    }
    if (additional_ee_subs_info->hss_subscription_info) {
        OpenAPI_hss_subscription_info_free(additional_ee_subs_info->hss_subscription_info);
        additional_ee_subs_info->hss_subscription_info = NULL;
    }
    ogs_free(additional_ee_subs_info);
}

cJSON *OpenAPI_additional_ee_subs_info_convertToJSON(OpenAPI_additional_ee_subs_info_t *additional_ee_subs_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (additional_ee_subs_info == NULL) {
        ogs_error("OpenAPI_additional_ee_subs_info_convertToJSON() failed [AdditionalEeSubsInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (additional_ee_subs_info->amf_subscription_info_list) {
    cJSON *amf_subscription_info_listList = cJSON_AddArrayToObject(item, "amfSubscriptionInfoList");
    if (amf_subscription_info_listList == NULL) {
        ogs_error("OpenAPI_additional_ee_subs_info_convertToJSON() failed [amf_subscription_info_list]");
        goto end;
    }
    OpenAPI_list_for_each(additional_ee_subs_info->amf_subscription_info_list, node) {
        cJSON *itemLocal = OpenAPI_amf_subscription_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_additional_ee_subs_info_convertToJSON() failed [amf_subscription_info_list]");
            goto end;
        }
        cJSON_AddItemToArray(amf_subscription_info_listList, itemLocal);
    }
    }

    if (additional_ee_subs_info->smf_subscription_info) {
    cJSON *smf_subscription_info_local_JSON = OpenAPI_smf_subscription_info_convertToJSON(additional_ee_subs_info->smf_subscription_info);
    if (smf_subscription_info_local_JSON == NULL) {
        ogs_error("OpenAPI_additional_ee_subs_info_convertToJSON() failed [smf_subscription_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "smfSubscriptionInfo", smf_subscription_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_additional_ee_subs_info_convertToJSON() failed [smf_subscription_info]");
        goto end;
    }
    }

    if (additional_ee_subs_info->hss_subscription_info) {
    cJSON *hss_subscription_info_local_JSON = OpenAPI_hss_subscription_info_convertToJSON(additional_ee_subs_info->hss_subscription_info);
    if (hss_subscription_info_local_JSON == NULL) {
        ogs_error("OpenAPI_additional_ee_subs_info_convertToJSON() failed [hss_subscription_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "hssSubscriptionInfo", hss_subscription_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_additional_ee_subs_info_convertToJSON() failed [hss_subscription_info]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_additional_ee_subs_info_t *OpenAPI_additional_ee_subs_info_parseFromJSON(cJSON *additional_ee_subs_infoJSON)
{
    OpenAPI_additional_ee_subs_info_t *additional_ee_subs_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *amf_subscription_info_list = NULL;
    OpenAPI_list_t *amf_subscription_info_listList = NULL;
    cJSON *smf_subscription_info = NULL;
    OpenAPI_smf_subscription_info_t *smf_subscription_info_local_nonprim = NULL;
    cJSON *hss_subscription_info = NULL;
    OpenAPI_hss_subscription_info_t *hss_subscription_info_local_nonprim = NULL;
    amf_subscription_info_list = cJSON_GetObjectItemCaseSensitive(additional_ee_subs_infoJSON, "amfSubscriptionInfoList");
    if (amf_subscription_info_list) {
        cJSON *amf_subscription_info_list_local = NULL;
        if (!cJSON_IsArray(amf_subscription_info_list)) {
            ogs_error("OpenAPI_additional_ee_subs_info_parseFromJSON() failed [amf_subscription_info_list]");
            goto end;
        }

        amf_subscription_info_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(amf_subscription_info_list_local, amf_subscription_info_list) {
            if (!cJSON_IsObject(amf_subscription_info_list_local)) {
                ogs_error("OpenAPI_additional_ee_subs_info_parseFromJSON() failed [amf_subscription_info_list]");
                goto end;
            }
            OpenAPI_amf_subscription_info_t *amf_subscription_info_listItem = OpenAPI_amf_subscription_info_parseFromJSON(amf_subscription_info_list_local);
            if (!amf_subscription_info_listItem) {
                ogs_error("No amf_subscription_info_listItem");
                goto end;
            }
            OpenAPI_list_add(amf_subscription_info_listList, amf_subscription_info_listItem);
        }
    }

    smf_subscription_info = cJSON_GetObjectItemCaseSensitive(additional_ee_subs_infoJSON, "smfSubscriptionInfo");
    if (smf_subscription_info) {
    smf_subscription_info_local_nonprim = OpenAPI_smf_subscription_info_parseFromJSON(smf_subscription_info);
    if (!smf_subscription_info_local_nonprim) {
        ogs_error("OpenAPI_smf_subscription_info_parseFromJSON failed [smf_subscription_info]");
        goto end;
    }
    }

    hss_subscription_info = cJSON_GetObjectItemCaseSensitive(additional_ee_subs_infoJSON, "hssSubscriptionInfo");
    if (hss_subscription_info) {
    hss_subscription_info_local_nonprim = OpenAPI_hss_subscription_info_parseFromJSON(hss_subscription_info);
    if (!hss_subscription_info_local_nonprim) {
        ogs_error("OpenAPI_hss_subscription_info_parseFromJSON failed [hss_subscription_info]");
        goto end;
    }
    }

    additional_ee_subs_info_local_var = OpenAPI_additional_ee_subs_info_create (
        amf_subscription_info_list ? amf_subscription_info_listList : NULL,
        smf_subscription_info ? smf_subscription_info_local_nonprim : NULL,
        hss_subscription_info ? hss_subscription_info_local_nonprim : NULL
    );

    return additional_ee_subs_info_local_var;
end:
    if (amf_subscription_info_listList) {
        OpenAPI_list_for_each(amf_subscription_info_listList, node) {
            OpenAPI_amf_subscription_info_free(node->data);
        }
        OpenAPI_list_free(amf_subscription_info_listList);
        amf_subscription_info_listList = NULL;
    }
    if (smf_subscription_info_local_nonprim) {
        OpenAPI_smf_subscription_info_free(smf_subscription_info_local_nonprim);
        smf_subscription_info_local_nonprim = NULL;
    }
    if (hss_subscription_info_local_nonprim) {
        OpenAPI_hss_subscription_info_free(hss_subscription_info_local_nonprim);
        hss_subscription_info_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_additional_ee_subs_info_t *OpenAPI_additional_ee_subs_info_copy(OpenAPI_additional_ee_subs_info_t *dst, OpenAPI_additional_ee_subs_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_additional_ee_subs_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_additional_ee_subs_info_convertToJSON() failed");
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

    OpenAPI_additional_ee_subs_info_free(dst);
    dst = OpenAPI_additional_ee_subs_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

