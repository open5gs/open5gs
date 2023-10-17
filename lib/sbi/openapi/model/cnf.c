
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cnf.h"

OpenAPI_cnf_t *OpenAPI_cnf_create(
    OpenAPI_list_t *cnf_units
)
{
    OpenAPI_cnf_t *cnf_local_var = ogs_malloc(sizeof(OpenAPI_cnf_t));
    ogs_assert(cnf_local_var);

    cnf_local_var->cnf_units = cnf_units;

    return cnf_local_var;
}

void OpenAPI_cnf_free(OpenAPI_cnf_t *cnf)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == cnf) {
        return;
    }
    if (cnf->cnf_units) {
        OpenAPI_list_for_each(cnf->cnf_units, node) {
            OpenAPI_cnf_unit_free(node->data);
        }
        OpenAPI_list_free(cnf->cnf_units);
        cnf->cnf_units = NULL;
    }
    ogs_free(cnf);
}

cJSON *OpenAPI_cnf_convertToJSON(OpenAPI_cnf_t *cnf)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (cnf == NULL) {
        ogs_error("OpenAPI_cnf_convertToJSON() failed [Cnf]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!cnf->cnf_units) {
        ogs_error("OpenAPI_cnf_convertToJSON() failed [cnf_units]");
        return NULL;
    }
    cJSON *cnf_unitsList = cJSON_AddArrayToObject(item, "cnfUnits");
    if (cnf_unitsList == NULL) {
        ogs_error("OpenAPI_cnf_convertToJSON() failed [cnf_units]");
        goto end;
    }
    OpenAPI_list_for_each(cnf->cnf_units, node) {
        cJSON *itemLocal = OpenAPI_cnf_unit_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_cnf_convertToJSON() failed [cnf_units]");
            goto end;
        }
        cJSON_AddItemToArray(cnf_unitsList, itemLocal);
    }

end:
    return item;
}

OpenAPI_cnf_t *OpenAPI_cnf_parseFromJSON(cJSON *cnfJSON)
{
    OpenAPI_cnf_t *cnf_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *cnf_units = NULL;
    OpenAPI_list_t *cnf_unitsList = NULL;
    cnf_units = cJSON_GetObjectItemCaseSensitive(cnfJSON, "cnfUnits");
    if (!cnf_units) {
        ogs_error("OpenAPI_cnf_parseFromJSON() failed [cnf_units]");
        goto end;
    }
        cJSON *cnf_units_local = NULL;
        if (!cJSON_IsArray(cnf_units)) {
            ogs_error("OpenAPI_cnf_parseFromJSON() failed [cnf_units]");
            goto end;
        }

        cnf_unitsList = OpenAPI_list_create();

        cJSON_ArrayForEach(cnf_units_local, cnf_units) {
            if (!cJSON_IsObject(cnf_units_local)) {
                ogs_error("OpenAPI_cnf_parseFromJSON() failed [cnf_units]");
                goto end;
            }
            OpenAPI_cnf_unit_t *cnf_unitsItem = OpenAPI_cnf_unit_parseFromJSON(cnf_units_local);
            if (!cnf_unitsItem) {
                ogs_error("No cnf_unitsItem");
                goto end;
            }
            OpenAPI_list_add(cnf_unitsList, cnf_unitsItem);
        }

    cnf_local_var = OpenAPI_cnf_create (
        cnf_unitsList
    );

    return cnf_local_var;
end:
    if (cnf_unitsList) {
        OpenAPI_list_for_each(cnf_unitsList, node) {
            OpenAPI_cnf_unit_free(node->data);
        }
        OpenAPI_list_free(cnf_unitsList);
        cnf_unitsList = NULL;
    }
    return NULL;
}

OpenAPI_cnf_t *OpenAPI_cnf_copy(OpenAPI_cnf_t *dst, OpenAPI_cnf_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_cnf_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_cnf_convertToJSON() failed");
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

    OpenAPI_cnf_free(dst);
    dst = OpenAPI_cnf_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

