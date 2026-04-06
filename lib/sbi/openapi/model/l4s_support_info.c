
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "l4s_support_info.h"

OpenAPI_l4s_support_info_t *OpenAPI_l4s_support_info_create(
    OpenAPI_list_t *ref_pcc_rule_ids,
    OpenAPI_l4s_notif_type_e notif_type
)
{
    OpenAPI_l4s_support_info_t *l4s_support_info_local_var = ogs_malloc(sizeof(OpenAPI_l4s_support_info_t));
    ogs_assert(l4s_support_info_local_var);

    l4s_support_info_local_var->ref_pcc_rule_ids = ref_pcc_rule_ids;
    l4s_support_info_local_var->notif_type = notif_type;

    return l4s_support_info_local_var;
}

void OpenAPI_l4s_support_info_free(OpenAPI_l4s_support_info_t *l4s_support_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == l4s_support_info) {
        return;
    }
    if (l4s_support_info->ref_pcc_rule_ids) {
        OpenAPI_list_for_each(l4s_support_info->ref_pcc_rule_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(l4s_support_info->ref_pcc_rule_ids);
        l4s_support_info->ref_pcc_rule_ids = NULL;
    }
    ogs_free(l4s_support_info);
}

cJSON *OpenAPI_l4s_support_info_convertToJSON(OpenAPI_l4s_support_info_t *l4s_support_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (l4s_support_info == NULL) {
        ogs_error("OpenAPI_l4s_support_info_convertToJSON() failed [L4sSupportInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!l4s_support_info->ref_pcc_rule_ids) {
        ogs_error("OpenAPI_l4s_support_info_convertToJSON() failed [ref_pcc_rule_ids]");
        return NULL;
    }
    cJSON *ref_pcc_rule_idsList = cJSON_AddArrayToObject(item, "refPccRuleIds");
    if (ref_pcc_rule_idsList == NULL) {
        ogs_error("OpenAPI_l4s_support_info_convertToJSON() failed [ref_pcc_rule_ids]");
        goto end;
    }
    OpenAPI_list_for_each(l4s_support_info->ref_pcc_rule_ids, node) {
        if (cJSON_AddStringToObject(ref_pcc_rule_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_l4s_support_info_convertToJSON() failed [ref_pcc_rule_ids]");
            goto end;
        }
    }

    if (l4s_support_info->notif_type == OpenAPI_l4s_notif_type_NULL) {
        ogs_error("OpenAPI_l4s_support_info_convertToJSON() failed [notif_type]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "notifType", OpenAPI_l4s_notif_type_ToString(l4s_support_info->notif_type)) == NULL) {
        ogs_error("OpenAPI_l4s_support_info_convertToJSON() failed [notif_type]");
        goto end;
    }

end:
    return item;
}

OpenAPI_l4s_support_info_t *OpenAPI_l4s_support_info_parseFromJSON(cJSON *l4s_support_infoJSON)
{
    OpenAPI_l4s_support_info_t *l4s_support_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ref_pcc_rule_ids = NULL;
    OpenAPI_list_t *ref_pcc_rule_idsList = NULL;
    cJSON *notif_type = NULL;
    OpenAPI_l4s_notif_type_e notif_typeVariable = 0;
    ref_pcc_rule_ids = cJSON_GetObjectItemCaseSensitive(l4s_support_infoJSON, "refPccRuleIds");
    if (!ref_pcc_rule_ids) {
        ogs_error("OpenAPI_l4s_support_info_parseFromJSON() failed [ref_pcc_rule_ids]");
        goto end;
    }
        cJSON *ref_pcc_rule_ids_local = NULL;
        if (!cJSON_IsArray(ref_pcc_rule_ids)) {
            ogs_error("OpenAPI_l4s_support_info_parseFromJSON() failed [ref_pcc_rule_ids]");
            goto end;
        }

        ref_pcc_rule_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(ref_pcc_rule_ids_local, ref_pcc_rule_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(ref_pcc_rule_ids_local)) {
                ogs_error("OpenAPI_l4s_support_info_parseFromJSON() failed [ref_pcc_rule_ids]");
                goto end;
            }
            OpenAPI_list_add(ref_pcc_rule_idsList, ogs_strdup(ref_pcc_rule_ids_local->valuestring));
        }

    notif_type = cJSON_GetObjectItemCaseSensitive(l4s_support_infoJSON, "notifType");
    if (!notif_type) {
        ogs_error("OpenAPI_l4s_support_info_parseFromJSON() failed [notif_type]");
        goto end;
    }
    if (!cJSON_IsString(notif_type)) {
        ogs_error("OpenAPI_l4s_support_info_parseFromJSON() failed [notif_type]");
        goto end;
    }
    notif_typeVariable = OpenAPI_l4s_notif_type_FromString(notif_type->valuestring);

    l4s_support_info_local_var = OpenAPI_l4s_support_info_create (
        ref_pcc_rule_idsList,
        notif_typeVariable
    );

    return l4s_support_info_local_var;
end:
    if (ref_pcc_rule_idsList) {
        OpenAPI_list_for_each(ref_pcc_rule_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ref_pcc_rule_idsList);
        ref_pcc_rule_idsList = NULL;
    }
    return NULL;
}

OpenAPI_l4s_support_info_t *OpenAPI_l4s_support_info_copy(OpenAPI_l4s_support_info_t *dst, OpenAPI_l4s_support_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_l4s_support_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_l4s_support_info_convertToJSON() failed");
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

    OpenAPI_l4s_support_info_free(dst);
    dst = OpenAPI_l4s_support_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

