
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dnf.h"

OpenAPI_dnf_t *OpenAPI_dnf_create(
    OpenAPI_list_t *dnf_units
    )
{
    OpenAPI_dnf_t *dnf_local_var = OpenAPI_malloc(sizeof(OpenAPI_dnf_t));
    if (!dnf_local_var) {
        return NULL;
    }
    dnf_local_var->dnf_units = dnf_units;

    return dnf_local_var;
}

void OpenAPI_dnf_free(OpenAPI_dnf_t *dnf)
{
    if (NULL == dnf) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(dnf->dnf_units, node) {
        OpenAPI_dnf_unit_free(node->data);
    }
    OpenAPI_list_free(dnf->dnf_units);
    ogs_free(dnf);
}

cJSON *OpenAPI_dnf_convertToJSON(OpenAPI_dnf_t *dnf)
{
    cJSON *item = NULL;

    if (dnf == NULL) {
        ogs_error("OpenAPI_dnf_convertToJSON() failed [Dnf]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!dnf->dnf_units) {
        ogs_error("OpenAPI_dnf_convertToJSON() failed [dnf_units]");
        goto end;
    }
    cJSON *dnf_unitsList = cJSON_AddArrayToObject(item, "dnfUnits");
    if (dnf_unitsList == NULL) {
        ogs_error("OpenAPI_dnf_convertToJSON() failed [dnf_units]");
        goto end;
    }

    OpenAPI_lnode_t *dnf_units_node;
    if (dnf->dnf_units) {
        OpenAPI_list_for_each(dnf->dnf_units, dnf_units_node) {
            cJSON *itemLocal = OpenAPI_dnf_unit_convertToJSON(dnf_units_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_dnf_convertToJSON() failed [dnf_units]");
                goto end;
            }
            cJSON_AddItemToArray(dnf_unitsList, itemLocal);
        }
    }

end:
    return item;
}

OpenAPI_dnf_t *OpenAPI_dnf_parseFromJSON(cJSON *dnfJSON)
{
    OpenAPI_dnf_t *dnf_local_var = NULL;
    cJSON *dnf_units = cJSON_GetObjectItemCaseSensitive(dnfJSON, "dnfUnits");
    if (!dnf_units) {
        ogs_error("OpenAPI_dnf_parseFromJSON() failed [dnf_units]");
        goto end;
    }

    OpenAPI_list_t *dnf_unitsList;

    cJSON *dnf_units_local_nonprimitive;
    if (!cJSON_IsArray(dnf_units)) {
        ogs_error("OpenAPI_dnf_parseFromJSON() failed [dnf_units]");
        goto end;
    }

    dnf_unitsList = OpenAPI_list_create();

    cJSON_ArrayForEach(dnf_units_local_nonprimitive, dnf_units ) {
        if (!cJSON_IsObject(dnf_units_local_nonprimitive)) {
            ogs_error("OpenAPI_dnf_parseFromJSON() failed [dnf_units]");
            goto end;
        }
        OpenAPI_dnf_unit_t *dnf_unitsItem = OpenAPI_dnf_unit_parseFromJSON(dnf_units_local_nonprimitive);

        OpenAPI_list_add(dnf_unitsList, dnf_unitsItem);
    }

    dnf_local_var = OpenAPI_dnf_create (
        dnf_unitsList
        );

    return dnf_local_var;
end:
    return NULL;
}

