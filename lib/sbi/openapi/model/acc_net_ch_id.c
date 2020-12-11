
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "acc_net_ch_id.h"

OpenAPI_acc_net_ch_id_t *OpenAPI_acc_net_ch_id_create(
    int acc_net_cha_id_value,
    OpenAPI_list_t *ref_pcc_rule_ids,
    int session_ch_scope
    )
{
    OpenAPI_acc_net_ch_id_t *acc_net_ch_id_local_var = OpenAPI_malloc(sizeof(OpenAPI_acc_net_ch_id_t));
    if (!acc_net_ch_id_local_var) {
        return NULL;
    }
    acc_net_ch_id_local_var->acc_net_cha_id_value = acc_net_cha_id_value;
    acc_net_ch_id_local_var->ref_pcc_rule_ids = ref_pcc_rule_ids;
    acc_net_ch_id_local_var->session_ch_scope = session_ch_scope;

    return acc_net_ch_id_local_var;
}

void OpenAPI_acc_net_ch_id_free(OpenAPI_acc_net_ch_id_t *acc_net_ch_id)
{
    if (NULL == acc_net_ch_id) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(acc_net_ch_id->ref_pcc_rule_ids, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(acc_net_ch_id->ref_pcc_rule_ids);
    ogs_free(acc_net_ch_id);
}

cJSON *OpenAPI_acc_net_ch_id_convertToJSON(OpenAPI_acc_net_ch_id_t *acc_net_ch_id)
{
    cJSON *item = NULL;

    if (acc_net_ch_id == NULL) {
        ogs_error("OpenAPI_acc_net_ch_id_convertToJSON() failed [AccNetChId]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!acc_net_ch_id->acc_net_cha_id_value) {
        ogs_error("OpenAPI_acc_net_ch_id_convertToJSON() failed [acc_net_cha_id_value]");
        goto end;
    }
    if (cJSON_AddNumberToObject(item, "accNetChaIdValue", acc_net_ch_id->acc_net_cha_id_value) == NULL) {
        ogs_error("OpenAPI_acc_net_ch_id_convertToJSON() failed [acc_net_cha_id_value]");
        goto end;
    }

    if (acc_net_ch_id->ref_pcc_rule_ids) {
        cJSON *ref_pcc_rule_ids = cJSON_AddArrayToObject(item, "refPccRuleIds");
        if (ref_pcc_rule_ids == NULL) {
            ogs_error("OpenAPI_acc_net_ch_id_convertToJSON() failed [ref_pcc_rule_ids]");
            goto end;
        }

        OpenAPI_lnode_t *ref_pcc_rule_ids_node;
        OpenAPI_list_for_each(acc_net_ch_id->ref_pcc_rule_ids, ref_pcc_rule_ids_node)  {
            if (cJSON_AddStringToObject(ref_pcc_rule_ids, "", (char*)ref_pcc_rule_ids_node->data) == NULL) {
                ogs_error("OpenAPI_acc_net_ch_id_convertToJSON() failed [ref_pcc_rule_ids]");
                goto end;
            }
        }
    }

    if (acc_net_ch_id->session_ch_scope) {
        if (cJSON_AddBoolToObject(item, "sessionChScope", acc_net_ch_id->session_ch_scope) == NULL) {
            ogs_error("OpenAPI_acc_net_ch_id_convertToJSON() failed [session_ch_scope]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_acc_net_ch_id_t *OpenAPI_acc_net_ch_id_parseFromJSON(cJSON *acc_net_ch_idJSON)
{
    OpenAPI_acc_net_ch_id_t *acc_net_ch_id_local_var = NULL;
    cJSON *acc_net_cha_id_value = cJSON_GetObjectItemCaseSensitive(acc_net_ch_idJSON, "accNetChaIdValue");
    if (!acc_net_cha_id_value) {
        ogs_error("OpenAPI_acc_net_ch_id_parseFromJSON() failed [acc_net_cha_id_value]");
        goto end;
    }


    if (!cJSON_IsNumber(acc_net_cha_id_value)) {
        ogs_error("OpenAPI_acc_net_ch_id_parseFromJSON() failed [acc_net_cha_id_value]");
        goto end;
    }

    cJSON *ref_pcc_rule_ids = cJSON_GetObjectItemCaseSensitive(acc_net_ch_idJSON, "refPccRuleIds");

    OpenAPI_list_t *ref_pcc_rule_idsList;
    if (ref_pcc_rule_ids) {
        cJSON *ref_pcc_rule_ids_local;
        if (!cJSON_IsArray(ref_pcc_rule_ids)) {
            ogs_error("OpenAPI_acc_net_ch_id_parseFromJSON() failed [ref_pcc_rule_ids]");
            goto end;
        }
        ref_pcc_rule_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(ref_pcc_rule_ids_local, ref_pcc_rule_ids) {
            if (!cJSON_IsString(ref_pcc_rule_ids_local)) {
                ogs_error("OpenAPI_acc_net_ch_id_parseFromJSON() failed [ref_pcc_rule_ids]");
                goto end;
            }
            OpenAPI_list_add(ref_pcc_rule_idsList, ogs_strdup(ref_pcc_rule_ids_local->valuestring));
        }
    }

    cJSON *session_ch_scope = cJSON_GetObjectItemCaseSensitive(acc_net_ch_idJSON, "sessionChScope");

    if (session_ch_scope) {
        if (!cJSON_IsBool(session_ch_scope)) {
            ogs_error("OpenAPI_acc_net_ch_id_parseFromJSON() failed [session_ch_scope]");
            goto end;
        }
    }

    acc_net_ch_id_local_var = OpenAPI_acc_net_ch_id_create (
        acc_net_cha_id_value->valuedouble,
        ref_pcc_rule_ids ? ref_pcc_rule_idsList : NULL,
        session_ch_scope ? session_ch_scope->valueint : 0
        );

    return acc_net_ch_id_local_var;
end:
    return NULL;
}

OpenAPI_acc_net_ch_id_t *OpenAPI_acc_net_ch_id_copy(OpenAPI_acc_net_ch_id_t *dst, OpenAPI_acc_net_ch_id_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_acc_net_ch_id_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_acc_net_ch_id_convertToJSON() failed");
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

    OpenAPI_acc_net_ch_id_free(dst);
    dst = OpenAPI_acc_net_ch_id_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

