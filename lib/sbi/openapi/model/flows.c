
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
    OpenAPI_lnode_t *node = NULL;

    if (NULL == flows) {
        return;
    }
    if (flows->cont_vers) {
        OpenAPI_list_for_each(flows->cont_vers, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(flows->cont_vers);
        flows->cont_vers = NULL;
    }
    if (flows->f_nums) {
        OpenAPI_list_for_each(flows->f_nums, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(flows->f_nums);
        flows->f_nums = NULL;
    }
    ogs_free(flows);
}

cJSON *OpenAPI_flows_convertToJSON(OpenAPI_flows_t *flows)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (flows == NULL) {
        ogs_error("OpenAPI_flows_convertToJSON() failed [Flows]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (flows->cont_vers) {
    cJSON *cont_versList = cJSON_AddArrayToObject(item, "contVers");
    if (cont_versList == NULL) {
        ogs_error("OpenAPI_flows_convertToJSON() failed [cont_vers]");
        goto end;
    }
    OpenAPI_list_for_each(flows->cont_vers, node) {
        if (node->data == NULL) {
            ogs_error("OpenAPI_flows_convertToJSON() failed [cont_vers]");
            goto end;
        }
        if (cJSON_AddNumberToObject(cont_versList, "", *(double *)node->data) == NULL) {
            ogs_error("OpenAPI_flows_convertToJSON() failed [cont_vers]");
            goto end;
        }
    }
    }

    if (flows->f_nums) {
    cJSON *f_numsList = cJSON_AddArrayToObject(item, "fNums");
    if (f_numsList == NULL) {
        ogs_error("OpenAPI_flows_convertToJSON() failed [f_nums]");
        goto end;
    }
    OpenAPI_list_for_each(flows->f_nums, node) {
        if (node->data == NULL) {
            ogs_error("OpenAPI_flows_convertToJSON() failed [f_nums]");
            goto end;
        }
        if (cJSON_AddNumberToObject(f_numsList, "", *(double *)node->data) == NULL) {
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
    OpenAPI_lnode_t *node = NULL;
    cJSON *cont_vers = NULL;
    OpenAPI_list_t *cont_versList = NULL;
    cJSON *f_nums = NULL;
    OpenAPI_list_t *f_numsList = NULL;
    cJSON *med_comp_n = NULL;
    cont_vers = cJSON_GetObjectItemCaseSensitive(flowsJSON, "contVers");
    if (cont_vers) {
        cJSON *cont_vers_local = NULL;
        if (!cJSON_IsArray(cont_vers)) {
            ogs_error("OpenAPI_flows_parseFromJSON() failed [cont_vers]");
            goto end;
        }

        cont_versList = OpenAPI_list_create();

        cJSON_ArrayForEach(cont_vers_local, cont_vers) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsNumber(cont_vers_local)) {
                ogs_error("OpenAPI_flows_parseFromJSON() failed [cont_vers]");
                goto end;
            }
            localDouble = (double *)ogs_calloc(1, sizeof(double));
            if (!localDouble) {
                ogs_error("OpenAPI_flows_parseFromJSON() failed [cont_vers]");
                goto end;
            }
            *localDouble = cont_vers_local->valuedouble;
            OpenAPI_list_add(cont_versList, localDouble);
        }
    }

    f_nums = cJSON_GetObjectItemCaseSensitive(flowsJSON, "fNums");
    if (f_nums) {
        cJSON *f_nums_local = NULL;
        if (!cJSON_IsArray(f_nums)) {
            ogs_error("OpenAPI_flows_parseFromJSON() failed [f_nums]");
            goto end;
        }

        f_numsList = OpenAPI_list_create();

        cJSON_ArrayForEach(f_nums_local, f_nums) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsNumber(f_nums_local)) {
                ogs_error("OpenAPI_flows_parseFromJSON() failed [f_nums]");
                goto end;
            }
            localDouble = (double *)ogs_calloc(1, sizeof(double));
            if (!localDouble) {
                ogs_error("OpenAPI_flows_parseFromJSON() failed [f_nums]");
                goto end;
            }
            *localDouble = f_nums_local->valuedouble;
            OpenAPI_list_add(f_numsList, localDouble);
        }
    }

    med_comp_n = cJSON_GetObjectItemCaseSensitive(flowsJSON, "medCompN");
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
    if (cont_versList) {
        OpenAPI_list_for_each(cont_versList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(cont_versList);
        cont_versList = NULL;
    }
    if (f_numsList) {
        OpenAPI_list_for_each(f_numsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(f_numsList);
        f_numsList = NULL;
    }
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

