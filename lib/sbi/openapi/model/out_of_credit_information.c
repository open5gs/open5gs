
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "out_of_credit_information.h"

OpenAPI_out_of_credit_information_t *OpenAPI_out_of_credit_information_create(
    OpenAPI_final_unit_action_t *fin_unit_act,
    OpenAPI_list_t *flows
)
{
    OpenAPI_out_of_credit_information_t *out_of_credit_information_local_var = ogs_malloc(sizeof(OpenAPI_out_of_credit_information_t));
    ogs_assert(out_of_credit_information_local_var);

    out_of_credit_information_local_var->fin_unit_act = fin_unit_act;
    out_of_credit_information_local_var->flows = flows;

    return out_of_credit_information_local_var;
}

void OpenAPI_out_of_credit_information_free(OpenAPI_out_of_credit_information_t *out_of_credit_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == out_of_credit_information) {
        return;
    }
    if (out_of_credit_information->fin_unit_act) {
        OpenAPI_final_unit_action_free(out_of_credit_information->fin_unit_act);
        out_of_credit_information->fin_unit_act = NULL;
    }
    if (out_of_credit_information->flows) {
        OpenAPI_list_for_each(out_of_credit_information->flows, node) {
            OpenAPI_flows_free(node->data);
        }
        OpenAPI_list_free(out_of_credit_information->flows);
        out_of_credit_information->flows = NULL;
    }
    ogs_free(out_of_credit_information);
}

cJSON *OpenAPI_out_of_credit_information_convertToJSON(OpenAPI_out_of_credit_information_t *out_of_credit_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (out_of_credit_information == NULL) {
        ogs_error("OpenAPI_out_of_credit_information_convertToJSON() failed [OutOfCreditInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!out_of_credit_information->fin_unit_act) {
        ogs_error("OpenAPI_out_of_credit_information_convertToJSON() failed [fin_unit_act]");
        return NULL;
    }
    cJSON *fin_unit_act_local_JSON = OpenAPI_final_unit_action_convertToJSON(out_of_credit_information->fin_unit_act);
    if (fin_unit_act_local_JSON == NULL) {
        ogs_error("OpenAPI_out_of_credit_information_convertToJSON() failed [fin_unit_act]");
        goto end;
    }
    cJSON_AddItemToObject(item, "finUnitAct", fin_unit_act_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_out_of_credit_information_convertToJSON() failed [fin_unit_act]");
        goto end;
    }

    if (out_of_credit_information->flows) {
    cJSON *flowsList = cJSON_AddArrayToObject(item, "flows");
    if (flowsList == NULL) {
        ogs_error("OpenAPI_out_of_credit_information_convertToJSON() failed [flows]");
        goto end;
    }
    OpenAPI_list_for_each(out_of_credit_information->flows, node) {
        cJSON *itemLocal = OpenAPI_flows_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_out_of_credit_information_convertToJSON() failed [flows]");
            goto end;
        }
        cJSON_AddItemToArray(flowsList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_out_of_credit_information_t *OpenAPI_out_of_credit_information_parseFromJSON(cJSON *out_of_credit_informationJSON)
{
    OpenAPI_out_of_credit_information_t *out_of_credit_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *fin_unit_act = NULL;
    OpenAPI_final_unit_action_t *fin_unit_act_local_nonprim = NULL;
    cJSON *flows = NULL;
    OpenAPI_list_t *flowsList = NULL;
    fin_unit_act = cJSON_GetObjectItemCaseSensitive(out_of_credit_informationJSON, "finUnitAct");
    if (!fin_unit_act) {
        ogs_error("OpenAPI_out_of_credit_information_parseFromJSON() failed [fin_unit_act]");
        goto end;
    }
    fin_unit_act_local_nonprim = OpenAPI_final_unit_action_parseFromJSON(fin_unit_act);
    if (!fin_unit_act_local_nonprim) {
        ogs_error("OpenAPI_final_unit_action_parseFromJSON failed [fin_unit_act]");
        goto end;
    }

    flows = cJSON_GetObjectItemCaseSensitive(out_of_credit_informationJSON, "flows");
    if (flows) {
        cJSON *flows_local = NULL;
        if (!cJSON_IsArray(flows)) {
            ogs_error("OpenAPI_out_of_credit_information_parseFromJSON() failed [flows]");
            goto end;
        }

        flowsList = OpenAPI_list_create();

        cJSON_ArrayForEach(flows_local, flows) {
            if (!cJSON_IsObject(flows_local)) {
                ogs_error("OpenAPI_out_of_credit_information_parseFromJSON() failed [flows]");
                goto end;
            }
            OpenAPI_flows_t *flowsItem = OpenAPI_flows_parseFromJSON(flows_local);
            if (!flowsItem) {
                ogs_error("No flowsItem");
                goto end;
            }
            OpenAPI_list_add(flowsList, flowsItem);
        }
    }

    out_of_credit_information_local_var = OpenAPI_out_of_credit_information_create (
        fin_unit_act_local_nonprim,
        flows ? flowsList : NULL
    );

    return out_of_credit_information_local_var;
end:
    if (fin_unit_act_local_nonprim) {
        OpenAPI_final_unit_action_free(fin_unit_act_local_nonprim);
        fin_unit_act_local_nonprim = NULL;
    }
    if (flowsList) {
        OpenAPI_list_for_each(flowsList, node) {
            OpenAPI_flows_free(node->data);
        }
        OpenAPI_list_free(flowsList);
        flowsList = NULL;
    }
    return NULL;
}

OpenAPI_out_of_credit_information_t *OpenAPI_out_of_credit_information_copy(OpenAPI_out_of_credit_information_t *dst, OpenAPI_out_of_credit_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_out_of_credit_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_out_of_credit_information_convertToJSON() failed");
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

    OpenAPI_out_of_credit_information_free(dst);
    dst = OpenAPI_out_of_credit_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

