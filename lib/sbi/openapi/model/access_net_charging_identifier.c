
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "access_net_charging_identifier.h"

OpenAPI_access_net_charging_identifier_t *OpenAPI_access_net_charging_identifier_create(
    int acc_net_cha_id_value,
    OpenAPI_list_t *flows
)
{
    OpenAPI_access_net_charging_identifier_t *access_net_charging_identifier_local_var = ogs_malloc(sizeof(OpenAPI_access_net_charging_identifier_t));
    ogs_assert(access_net_charging_identifier_local_var);

    access_net_charging_identifier_local_var->acc_net_cha_id_value = acc_net_cha_id_value;
    access_net_charging_identifier_local_var->flows = flows;

    return access_net_charging_identifier_local_var;
}

void OpenAPI_access_net_charging_identifier_free(OpenAPI_access_net_charging_identifier_t *access_net_charging_identifier)
{
    if (NULL == access_net_charging_identifier) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(access_net_charging_identifier->flows, node) {
        OpenAPI_flows_free(node->data);
    }
    OpenAPI_list_free(access_net_charging_identifier->flows);
    ogs_free(access_net_charging_identifier);
}

cJSON *OpenAPI_access_net_charging_identifier_convertToJSON(OpenAPI_access_net_charging_identifier_t *access_net_charging_identifier)
{
    cJSON *item = NULL;

    if (access_net_charging_identifier == NULL) {
        ogs_error("OpenAPI_access_net_charging_identifier_convertToJSON() failed [AccessNetChargingIdentifier]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "accNetChaIdValue", access_net_charging_identifier->acc_net_cha_id_value) == NULL) {
        ogs_error("OpenAPI_access_net_charging_identifier_convertToJSON() failed [acc_net_cha_id_value]");
        goto end;
    }

    if (access_net_charging_identifier->flows) {
    cJSON *flowsList = cJSON_AddArrayToObject(item, "flows");
    if (flowsList == NULL) {
        ogs_error("OpenAPI_access_net_charging_identifier_convertToJSON() failed [flows]");
        goto end;
    }

    OpenAPI_lnode_t *flows_node;
    if (access_net_charging_identifier->flows) {
        OpenAPI_list_for_each(access_net_charging_identifier->flows, flows_node) {
            cJSON *itemLocal = OpenAPI_flows_convertToJSON(flows_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_access_net_charging_identifier_convertToJSON() failed [flows]");
                goto end;
            }
            cJSON_AddItemToArray(flowsList, itemLocal);
        }
    }
    }

end:
    return item;
}

OpenAPI_access_net_charging_identifier_t *OpenAPI_access_net_charging_identifier_parseFromJSON(cJSON *access_net_charging_identifierJSON)
{
    OpenAPI_access_net_charging_identifier_t *access_net_charging_identifier_local_var = NULL;
    cJSON *acc_net_cha_id_value = cJSON_GetObjectItemCaseSensitive(access_net_charging_identifierJSON, "accNetChaIdValue");
    if (!acc_net_cha_id_value) {
        ogs_error("OpenAPI_access_net_charging_identifier_parseFromJSON() failed [acc_net_cha_id_value]");
        goto end;
    }

    if (!cJSON_IsNumber(acc_net_cha_id_value)) {
        ogs_error("OpenAPI_access_net_charging_identifier_parseFromJSON() failed [acc_net_cha_id_value]");
        goto end;
    }

    cJSON *flows = cJSON_GetObjectItemCaseSensitive(access_net_charging_identifierJSON, "flows");

    OpenAPI_list_t *flowsList;
    if (flows) {
    cJSON *flows_local_nonprimitive;
    if (!cJSON_IsArray(flows)){
        ogs_error("OpenAPI_access_net_charging_identifier_parseFromJSON() failed [flows]");
        goto end;
    }

    flowsList = OpenAPI_list_create();

    cJSON_ArrayForEach(flows_local_nonprimitive, flows ) {
        if (!cJSON_IsObject(flows_local_nonprimitive)) {
            ogs_error("OpenAPI_access_net_charging_identifier_parseFromJSON() failed [flows]");
            goto end;
        }
        OpenAPI_flows_t *flowsItem = OpenAPI_flows_parseFromJSON(flows_local_nonprimitive);

        if (!flowsItem) {
            ogs_error("No flowsItem");
            OpenAPI_list_free(flowsList);
            goto end;
        }

        OpenAPI_list_add(flowsList, flowsItem);
    }
    }

    access_net_charging_identifier_local_var = OpenAPI_access_net_charging_identifier_create (
        
        acc_net_cha_id_value->valuedouble,
        flows ? flowsList : NULL
    );

    return access_net_charging_identifier_local_var;
end:
    return NULL;
}

OpenAPI_access_net_charging_identifier_t *OpenAPI_access_net_charging_identifier_copy(OpenAPI_access_net_charging_identifier_t *dst, OpenAPI_access_net_charging_identifier_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_access_net_charging_identifier_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_access_net_charging_identifier_convertToJSON() failed");
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

    OpenAPI_access_net_charging_identifier_free(dst);
    dst = OpenAPI_access_net_charging_identifier_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

