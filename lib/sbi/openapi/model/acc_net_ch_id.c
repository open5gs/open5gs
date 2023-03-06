
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "acc_net_ch_id.h"

OpenAPI_acc_net_ch_id_t *OpenAPI_acc_net_ch_id_create(
    bool is_acc_net_cha_id_value,
    int acc_net_cha_id_value,
    char *acc_net_charg_id,
    OpenAPI_list_t *ref_pcc_rule_ids,
    bool is_session_ch_scope,
    int session_ch_scope
)
{
    OpenAPI_acc_net_ch_id_t *acc_net_ch_id_local_var = ogs_malloc(sizeof(OpenAPI_acc_net_ch_id_t));
    ogs_assert(acc_net_ch_id_local_var);

    acc_net_ch_id_local_var->is_acc_net_cha_id_value = is_acc_net_cha_id_value;
    acc_net_ch_id_local_var->acc_net_cha_id_value = acc_net_cha_id_value;
    acc_net_ch_id_local_var->acc_net_charg_id = acc_net_charg_id;
    acc_net_ch_id_local_var->ref_pcc_rule_ids = ref_pcc_rule_ids;
    acc_net_ch_id_local_var->is_session_ch_scope = is_session_ch_scope;
    acc_net_ch_id_local_var->session_ch_scope = session_ch_scope;

    return acc_net_ch_id_local_var;
}

void OpenAPI_acc_net_ch_id_free(OpenAPI_acc_net_ch_id_t *acc_net_ch_id)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == acc_net_ch_id) {
        return;
    }
    if (acc_net_ch_id->acc_net_charg_id) {
        ogs_free(acc_net_ch_id->acc_net_charg_id);
        acc_net_ch_id->acc_net_charg_id = NULL;
    }
    if (acc_net_ch_id->ref_pcc_rule_ids) {
        OpenAPI_list_for_each(acc_net_ch_id->ref_pcc_rule_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(acc_net_ch_id->ref_pcc_rule_ids);
        acc_net_ch_id->ref_pcc_rule_ids = NULL;
    }
    ogs_free(acc_net_ch_id);
}

cJSON *OpenAPI_acc_net_ch_id_convertToJSON(OpenAPI_acc_net_ch_id_t *acc_net_ch_id)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (acc_net_ch_id == NULL) {
        ogs_error("OpenAPI_acc_net_ch_id_convertToJSON() failed [AccNetChId]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (acc_net_ch_id->is_acc_net_cha_id_value) {
    if (cJSON_AddNumberToObject(item, "accNetChaIdValue", acc_net_ch_id->acc_net_cha_id_value) == NULL) {
        ogs_error("OpenAPI_acc_net_ch_id_convertToJSON() failed [acc_net_cha_id_value]");
        goto end;
    }
    }

    if (acc_net_ch_id->acc_net_charg_id) {
    if (cJSON_AddStringToObject(item, "accNetChargId", acc_net_ch_id->acc_net_charg_id) == NULL) {
        ogs_error("OpenAPI_acc_net_ch_id_convertToJSON() failed [acc_net_charg_id]");
        goto end;
    }
    }

    if (acc_net_ch_id->ref_pcc_rule_ids) {
    cJSON *ref_pcc_rule_idsList = cJSON_AddArrayToObject(item, "refPccRuleIds");
    if (ref_pcc_rule_idsList == NULL) {
        ogs_error("OpenAPI_acc_net_ch_id_convertToJSON() failed [ref_pcc_rule_ids]");
        goto end;
    }
    OpenAPI_list_for_each(acc_net_ch_id->ref_pcc_rule_ids, node) {
        if (cJSON_AddStringToObject(ref_pcc_rule_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_acc_net_ch_id_convertToJSON() failed [ref_pcc_rule_ids]");
            goto end;
        }
    }
    }

    if (acc_net_ch_id->is_session_ch_scope) {
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
    OpenAPI_lnode_t *node = NULL;
    cJSON *acc_net_cha_id_value = NULL;
    cJSON *acc_net_charg_id = NULL;
    cJSON *ref_pcc_rule_ids = NULL;
    OpenAPI_list_t *ref_pcc_rule_idsList = NULL;
    cJSON *session_ch_scope = NULL;
    acc_net_cha_id_value = cJSON_GetObjectItemCaseSensitive(acc_net_ch_idJSON, "accNetChaIdValue");
    if (acc_net_cha_id_value) {
    if (!cJSON_IsNumber(acc_net_cha_id_value)) {
        ogs_error("OpenAPI_acc_net_ch_id_parseFromJSON() failed [acc_net_cha_id_value]");
        goto end;
    }
    }

    acc_net_charg_id = cJSON_GetObjectItemCaseSensitive(acc_net_ch_idJSON, "accNetChargId");
    if (acc_net_charg_id) {
    if (!cJSON_IsString(acc_net_charg_id) && !cJSON_IsNull(acc_net_charg_id)) {
        ogs_error("OpenAPI_acc_net_ch_id_parseFromJSON() failed [acc_net_charg_id]");
        goto end;
    }
    }

    ref_pcc_rule_ids = cJSON_GetObjectItemCaseSensitive(acc_net_ch_idJSON, "refPccRuleIds");
    if (ref_pcc_rule_ids) {
        cJSON *ref_pcc_rule_ids_local = NULL;
        if (!cJSON_IsArray(ref_pcc_rule_ids)) {
            ogs_error("OpenAPI_acc_net_ch_id_parseFromJSON() failed [ref_pcc_rule_ids]");
            goto end;
        }

        ref_pcc_rule_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(ref_pcc_rule_ids_local, ref_pcc_rule_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(ref_pcc_rule_ids_local)) {
                ogs_error("OpenAPI_acc_net_ch_id_parseFromJSON() failed [ref_pcc_rule_ids]");
                goto end;
            }
            OpenAPI_list_add(ref_pcc_rule_idsList, ogs_strdup(ref_pcc_rule_ids_local->valuestring));
        }
    }

    session_ch_scope = cJSON_GetObjectItemCaseSensitive(acc_net_ch_idJSON, "sessionChScope");
    if (session_ch_scope) {
    if (!cJSON_IsBool(session_ch_scope)) {
        ogs_error("OpenAPI_acc_net_ch_id_parseFromJSON() failed [session_ch_scope]");
        goto end;
    }
    }

    acc_net_ch_id_local_var = OpenAPI_acc_net_ch_id_create (
        acc_net_cha_id_value ? true : false,
        acc_net_cha_id_value ? acc_net_cha_id_value->valuedouble : 0,
        acc_net_charg_id && !cJSON_IsNull(acc_net_charg_id) ? ogs_strdup(acc_net_charg_id->valuestring) : NULL,
        ref_pcc_rule_ids ? ref_pcc_rule_idsList : NULL,
        session_ch_scope ? true : false,
        session_ch_scope ? session_ch_scope->valueint : 0
    );

    return acc_net_ch_id_local_var;
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

