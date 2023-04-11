
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "qos_flow_setup_item.h"

OpenAPI_qos_flow_setup_item_t *OpenAPI_qos_flow_setup_item_create(
    int qfi,
    char *qos_rules,
    bool is_ebi,
    int ebi,
    char *qos_flow_description,
    OpenAPI_qos_flow_profile_t *qos_flow_profile,
    OpenAPI_qos_flow_access_type_e associated_an_type,
    bool is_default_qos_rule_ind,
    int default_qos_rule_ind
)
{
    OpenAPI_qos_flow_setup_item_t *qos_flow_setup_item_local_var = ogs_malloc(sizeof(OpenAPI_qos_flow_setup_item_t));
    ogs_assert(qos_flow_setup_item_local_var);

    qos_flow_setup_item_local_var->qfi = qfi;
    qos_flow_setup_item_local_var->qos_rules = qos_rules;
    qos_flow_setup_item_local_var->is_ebi = is_ebi;
    qos_flow_setup_item_local_var->ebi = ebi;
    qos_flow_setup_item_local_var->qos_flow_description = qos_flow_description;
    qos_flow_setup_item_local_var->qos_flow_profile = qos_flow_profile;
    qos_flow_setup_item_local_var->associated_an_type = associated_an_type;
    qos_flow_setup_item_local_var->is_default_qos_rule_ind = is_default_qos_rule_ind;
    qos_flow_setup_item_local_var->default_qos_rule_ind = default_qos_rule_ind;

    return qos_flow_setup_item_local_var;
}

void OpenAPI_qos_flow_setup_item_free(OpenAPI_qos_flow_setup_item_t *qos_flow_setup_item)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == qos_flow_setup_item) {
        return;
    }
    if (qos_flow_setup_item->qos_rules) {
        ogs_free(qos_flow_setup_item->qos_rules);
        qos_flow_setup_item->qos_rules = NULL;
    }
    if (qos_flow_setup_item->qos_flow_description) {
        ogs_free(qos_flow_setup_item->qos_flow_description);
        qos_flow_setup_item->qos_flow_description = NULL;
    }
    if (qos_flow_setup_item->qos_flow_profile) {
        OpenAPI_qos_flow_profile_free(qos_flow_setup_item->qos_flow_profile);
        qos_flow_setup_item->qos_flow_profile = NULL;
    }
    ogs_free(qos_flow_setup_item);
}

cJSON *OpenAPI_qos_flow_setup_item_convertToJSON(OpenAPI_qos_flow_setup_item_t *qos_flow_setup_item)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (qos_flow_setup_item == NULL) {
        ogs_error("OpenAPI_qos_flow_setup_item_convertToJSON() failed [QosFlowSetupItem]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "qfi", qos_flow_setup_item->qfi) == NULL) {
        ogs_error("OpenAPI_qos_flow_setup_item_convertToJSON() failed [qfi]");
        goto end;
    }

    if (!qos_flow_setup_item->qos_rules) {
        ogs_error("OpenAPI_qos_flow_setup_item_convertToJSON() failed [qos_rules]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "qosRules", qos_flow_setup_item->qos_rules) == NULL) {
        ogs_error("OpenAPI_qos_flow_setup_item_convertToJSON() failed [qos_rules]");
        goto end;
    }

    if (qos_flow_setup_item->is_ebi) {
    if (cJSON_AddNumberToObject(item, "ebi", qos_flow_setup_item->ebi) == NULL) {
        ogs_error("OpenAPI_qos_flow_setup_item_convertToJSON() failed [ebi]");
        goto end;
    }
    }

    if (qos_flow_setup_item->qos_flow_description) {
    if (cJSON_AddStringToObject(item, "qosFlowDescription", qos_flow_setup_item->qos_flow_description) == NULL) {
        ogs_error("OpenAPI_qos_flow_setup_item_convertToJSON() failed [qos_flow_description]");
        goto end;
    }
    }

    if (qos_flow_setup_item->qos_flow_profile) {
    cJSON *qos_flow_profile_local_JSON = OpenAPI_qos_flow_profile_convertToJSON(qos_flow_setup_item->qos_flow_profile);
    if (qos_flow_profile_local_JSON == NULL) {
        ogs_error("OpenAPI_qos_flow_setup_item_convertToJSON() failed [qos_flow_profile]");
        goto end;
    }
    cJSON_AddItemToObject(item, "qosFlowProfile", qos_flow_profile_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_qos_flow_setup_item_convertToJSON() failed [qos_flow_profile]");
        goto end;
    }
    }

    if (qos_flow_setup_item->associated_an_type != OpenAPI_qos_flow_access_type_NULL) {
    if (cJSON_AddStringToObject(item, "associatedAnType", OpenAPI_qos_flow_access_type_ToString(qos_flow_setup_item->associated_an_type)) == NULL) {
        ogs_error("OpenAPI_qos_flow_setup_item_convertToJSON() failed [associated_an_type]");
        goto end;
    }
    }

    if (qos_flow_setup_item->is_default_qos_rule_ind) {
    if (cJSON_AddBoolToObject(item, "defaultQosRuleInd", qos_flow_setup_item->default_qos_rule_ind) == NULL) {
        ogs_error("OpenAPI_qos_flow_setup_item_convertToJSON() failed [default_qos_rule_ind]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_qos_flow_setup_item_t *OpenAPI_qos_flow_setup_item_parseFromJSON(cJSON *qos_flow_setup_itemJSON)
{
    OpenAPI_qos_flow_setup_item_t *qos_flow_setup_item_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *qfi = NULL;
    cJSON *qos_rules = NULL;
    cJSON *ebi = NULL;
    cJSON *qos_flow_description = NULL;
    cJSON *qos_flow_profile = NULL;
    OpenAPI_qos_flow_profile_t *qos_flow_profile_local_nonprim = NULL;
    cJSON *associated_an_type = NULL;
    OpenAPI_qos_flow_access_type_e associated_an_typeVariable = 0;
    cJSON *default_qos_rule_ind = NULL;
    qfi = cJSON_GetObjectItemCaseSensitive(qos_flow_setup_itemJSON, "qfi");
    if (!qfi) {
        ogs_error("OpenAPI_qos_flow_setup_item_parseFromJSON() failed [qfi]");
        goto end;
    }
    if (!cJSON_IsNumber(qfi)) {
        ogs_error("OpenAPI_qos_flow_setup_item_parseFromJSON() failed [qfi]");
        goto end;
    }

    qos_rules = cJSON_GetObjectItemCaseSensitive(qos_flow_setup_itemJSON, "qosRules");
    if (!qos_rules) {
        ogs_error("OpenAPI_qos_flow_setup_item_parseFromJSON() failed [qos_rules]");
        goto end;
    }
    if (!cJSON_IsString(qos_rules)) {
        ogs_error("OpenAPI_qos_flow_setup_item_parseFromJSON() failed [qos_rules]");
        goto end;
    }

    ebi = cJSON_GetObjectItemCaseSensitive(qos_flow_setup_itemJSON, "ebi");
    if (ebi) {
    if (!cJSON_IsNumber(ebi)) {
        ogs_error("OpenAPI_qos_flow_setup_item_parseFromJSON() failed [ebi]");
        goto end;
    }
    }

    qos_flow_description = cJSON_GetObjectItemCaseSensitive(qos_flow_setup_itemJSON, "qosFlowDescription");
    if (qos_flow_description) {
    if (!cJSON_IsString(qos_flow_description) && !cJSON_IsNull(qos_flow_description)) {
        ogs_error("OpenAPI_qos_flow_setup_item_parseFromJSON() failed [qos_flow_description]");
        goto end;
    }
    }

    qos_flow_profile = cJSON_GetObjectItemCaseSensitive(qos_flow_setup_itemJSON, "qosFlowProfile");
    if (qos_flow_profile) {
    qos_flow_profile_local_nonprim = OpenAPI_qos_flow_profile_parseFromJSON(qos_flow_profile);
    if (!qos_flow_profile_local_nonprim) {
        ogs_error("OpenAPI_qos_flow_profile_parseFromJSON failed [qos_flow_profile]");
        goto end;
    }
    }

    associated_an_type = cJSON_GetObjectItemCaseSensitive(qos_flow_setup_itemJSON, "associatedAnType");
    if (associated_an_type) {
    if (!cJSON_IsString(associated_an_type)) {
        ogs_error("OpenAPI_qos_flow_setup_item_parseFromJSON() failed [associated_an_type]");
        goto end;
    }
    associated_an_typeVariable = OpenAPI_qos_flow_access_type_FromString(associated_an_type->valuestring);
    }

    default_qos_rule_ind = cJSON_GetObjectItemCaseSensitive(qos_flow_setup_itemJSON, "defaultQosRuleInd");
    if (default_qos_rule_ind) {
    if (!cJSON_IsBool(default_qos_rule_ind)) {
        ogs_error("OpenAPI_qos_flow_setup_item_parseFromJSON() failed [default_qos_rule_ind]");
        goto end;
    }
    }

    qos_flow_setup_item_local_var = OpenAPI_qos_flow_setup_item_create (
        
        qfi->valuedouble,
        ogs_strdup(qos_rules->valuestring),
        ebi ? true : false,
        ebi ? ebi->valuedouble : 0,
        qos_flow_description && !cJSON_IsNull(qos_flow_description) ? ogs_strdup(qos_flow_description->valuestring) : NULL,
        qos_flow_profile ? qos_flow_profile_local_nonprim : NULL,
        associated_an_type ? associated_an_typeVariable : 0,
        default_qos_rule_ind ? true : false,
        default_qos_rule_ind ? default_qos_rule_ind->valueint : 0
    );

    return qos_flow_setup_item_local_var;
end:
    if (qos_flow_profile_local_nonprim) {
        OpenAPI_qos_flow_profile_free(qos_flow_profile_local_nonprim);
        qos_flow_profile_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_qos_flow_setup_item_t *OpenAPI_qos_flow_setup_item_copy(OpenAPI_qos_flow_setup_item_t *dst, OpenAPI_qos_flow_setup_item_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_qos_flow_setup_item_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_qos_flow_setup_item_convertToJSON() failed");
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

    OpenAPI_qos_flow_setup_item_free(dst);
    dst = OpenAPI_qos_flow_setup_item_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

