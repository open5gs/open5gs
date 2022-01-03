
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "flows.h"

OpenAPI_flows_t *OpenAPI_flows_create(
    OpenAPI_list_t *cont_vers,
    OpenAPI_list_t *f_nums,
    int med_comp_n
)
{
    OpenAPI_flows_t *flows_local_var = ogs_malloc(sizeof(OpenAPI_flows_t));
    ogs_assert(flows_local_var);

    flows_local_var->cont_vers = cont_vers;
    flows_local_var->f_nums = f_nums;
    flows_local_var->med_comp_n = med_comp_n;

    return flows_local_var;
}

void OpenAPI_flows_free(OpenAPI_flows_t *flows)
{
    if (NULL == flows) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(flows->cont_vers, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(flows->cont_vers);
    OpenAPI_list_for_each(flows->f_nums, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(flows->f_nums);
    ogs_free(flows);
}

cJSON *OpenAPI_flows_convertToJSON(OpenAPI_flows_t *flows)
{
    cJSON *item = NULL;

    if (flows == NULL) {
        ogs_error("OpenAPI_flows_convertToJSON() failed [Flows]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (flows->cont_vers) {
    cJSON *cont_vers = cJSON_AddArrayToObject(item, "contVers");
    if (cont_vers == NULL) {
        ogs_error("OpenAPI_flows_convertToJSON() failed [cont_vers]");
        goto end;
    }

    OpenAPI_lnode_t *cont_vers_node;
    OpenAPI_list_for_each(flows->cont_vers, cont_vers_node)  {
    if (cJSON_AddNumberToObject(cont_vers, "", *(double *)cont_vers_node->data) == NULL) {
        ogs_error("OpenAPI_flows_convertToJSON() failed [cont_vers]");
        goto end;
    }
                    }
    }

    if (flows->f_nums) {
    cJSON *f_nums = cJSON_AddArrayToObject(item, "fNums");
    if (f_nums == NULL) {
        ogs_error("OpenAPI_flows_convertToJSON() failed [f_nums]");
        goto end;
    }

    OpenAPI_lnode_t *f_nums_node;
    OpenAPI_list_for_each(flows->f_nums, f_nums_node)  {
    if (cJSON_AddNumberToObject(f_nums, "", *(double *)f_nums_node->data) == NULL) {
        ogs_error("OpenAPI_flows_convertToJSON() failed [f_nums]");
        goto end;
    }
                    }
    }

    if (cJSON_AddNumberToObject(item, "medCompN", flows->med_comp_n) == NULL) {
        ogs_error("OpenAPI_flows_convertToJSON() failed [med_comp_n]");
        goto end;
    }

end:
    return item;
}

OpenAPI_flows_t *OpenAPI_flows_parseFromJSON(cJSON *flowsJSON)
{
    OpenAPI_flows_t *flows_local_var = NULL;
    cJSON *cont_vers = cJSON_GetObjectItemCaseSensitive(flowsJSON, "contVers");

    OpenAPI_list_t *cont_versList;
    if (cont_vers) {
    cJSON *cont_vers_local;
    if (!cJSON_IsArray(cont_vers)) {
        ogs_error("OpenAPI_flows_parseFromJSON() failed [cont_vers]");
        goto end;
    }
    cont_versList = OpenAPI_list_create();

    cJSON_ArrayForEach(cont_vers_local, cont_vers) {
    if (!cJSON_IsNumber(cont_vers_local)) {
        ogs_error("OpenAPI_flows_parseFromJSON() failed [cont_vers]");
        goto end;
    }
    OpenAPI_list_add(cont_versList , &cont_vers_local->valuedouble);
    }
    }

    cJSON *f_nums = cJSON_GetObjectItemCaseSensitive(flowsJSON, "fNums");

    OpenAPI_list_t *f_numsList;
    if (f_nums) {
    cJSON *f_nums_local;
    if (!cJSON_IsArray(f_nums)) {
        ogs_error("OpenAPI_flows_parseFromJSON() failed [f_nums]");
        goto end;
    }
    f_numsList = OpenAPI_list_create();

    cJSON_ArrayForEach(f_nums_local, f_nums) {
    if (!cJSON_IsNumber(f_nums_local)) {
        ogs_error("OpenAPI_flows_parseFromJSON() failed [f_nums]");
        goto end;
    }
    OpenAPI_list_add(f_numsList , &f_nums_local->valuedouble);
    }
    }

    cJSON *med_comp_n = cJSON_GetObjectItemCaseSensitive(flowsJSON, "medCompN");
    if (!med_comp_n) {
        ogs_error("OpenAPI_flows_parseFromJSON() failed [med_comp_n]");
        goto end;
    }

    if (!cJSON_IsNumber(med_comp_n)) {
        ogs_error("OpenAPI_flows_parseFromJSON() failed [med_comp_n]");
        goto end;
    }

    flows_local_var = OpenAPI_flows_create (
        cont_vers ? cont_versList : NULL,
        f_nums ? f_numsList : NULL,
        
        med_comp_n->valuedouble
    );

    return flows_local_var;
end:
    return NULL;
}

OpenAPI_flows_t *OpenAPI_flows_copy(OpenAPI_flows_t *dst, OpenAPI_flows_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_flows_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_flows_convertToJSON() failed");
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

    OpenAPI_flows_free(dst);
    dst = OpenAPI_flows_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

