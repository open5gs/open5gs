
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cnf_unit.h"

OpenAPI_cnf_unit_t *OpenAPI_cnf_unit_create(
    OpenAPI_list_t *cnf_unit
)
{
    OpenAPI_cnf_unit_t *cnf_unit_local_var = ogs_malloc(sizeof(OpenAPI_cnf_unit_t));
    ogs_assert(cnf_unit_local_var);

    cnf_unit_local_var->cnf_unit = cnf_unit;

    return cnf_unit_local_var;
}

void OpenAPI_cnf_unit_free(OpenAPI_cnf_unit_t *cnf_unit)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == cnf_unit) {
        return;
    }
    if (cnf_unit->cnf_unit) {
        OpenAPI_list_for_each(cnf_unit->cnf_unit, node) {
            OpenAPI_atom_free(node->data);
        }
        OpenAPI_list_free(cnf_unit->cnf_unit);
        cnf_unit->cnf_unit = NULL;
    }
    ogs_free(cnf_unit);
}

cJSON *OpenAPI_cnf_unit_convertToJSON(OpenAPI_cnf_unit_t *cnf_unit)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (cnf_unit == NULL) {
        ogs_error("OpenAPI_cnf_unit_convertToJSON() failed [CnfUnit]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!cnf_unit->cnf_unit) {
        ogs_error("OpenAPI_cnf_unit_convertToJSON() failed [cnf_unit]");
        return NULL;
    }
    cJSON *cnf_unitList = cJSON_AddArrayToObject(item, "cnfUnit");
    if (cnf_unitList == NULL) {
        ogs_error("OpenAPI_cnf_unit_convertToJSON() failed [cnf_unit]");
        goto end;
    }
    OpenAPI_list_for_each(cnf_unit->cnf_unit, node) {
        cJSON *itemLocal = OpenAPI_atom_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_cnf_unit_convertToJSON() failed [cnf_unit]");
            goto end;
        }
        cJSON_AddItemToArray(cnf_unitList, itemLocal);
    }

end:
    return item;
}

OpenAPI_cnf_unit_t *OpenAPI_cnf_unit_parseFromJSON(cJSON *cnf_unitJSON)
{
    OpenAPI_cnf_unit_t *cnf_unit_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *cnf_unit = NULL;
    OpenAPI_list_t *cnf_unitList = NULL;
    cnf_unit = cJSON_GetObjectItemCaseSensitive(cnf_unitJSON, "cnfUnit");
    if (!cnf_unit) {
        ogs_error("OpenAPI_cnf_unit_parseFromJSON() failed [cnf_unit]");
        goto end;
    }
        cJSON *cnf_unit_local = NULL;
        if (!cJSON_IsArray(cnf_unit)) {
            ogs_error("OpenAPI_cnf_unit_parseFromJSON() failed [cnf_unit]");
            goto end;
        }

        cnf_unitList = OpenAPI_list_create();

        cJSON_ArrayForEach(cnf_unit_local, cnf_unit) {
            if (!cJSON_IsObject(cnf_unit_local)) {
                ogs_error("OpenAPI_cnf_unit_parseFromJSON() failed [cnf_unit]");
                goto end;
            }
            OpenAPI_atom_t *cnf_unitItem = OpenAPI_atom_parseFromJSON(cnf_unit_local);
            if (!cnf_unitItem) {
                ogs_error("No cnf_unitItem");
                goto end;
            }
            OpenAPI_list_add(cnf_unitList, cnf_unitItem);
        }

    cnf_unit_local_var = OpenAPI_cnf_unit_create (
        cnf_unitList
    );

    return cnf_unit_local_var;
end:
    if (cnf_unitList) {
        OpenAPI_list_for_each(cnf_unitList, node) {
            OpenAPI_atom_free(node->data);
        }
        OpenAPI_list_free(cnf_unitList);
        cnf_unitList = NULL;
    }
    return NULL;
}

OpenAPI_cnf_unit_t *OpenAPI_cnf_unit_copy(OpenAPI_cnf_unit_t *dst, OpenAPI_cnf_unit_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_cnf_unit_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_cnf_unit_convertToJSON() failed");
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

    OpenAPI_cnf_unit_free(dst);
    dst = OpenAPI_cnf_unit_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

