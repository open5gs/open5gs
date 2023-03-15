
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dnf_unit.h"

OpenAPI_dnf_unit_t *OpenAPI_dnf_unit_create(
    OpenAPI_list_t *dnf_unit
)
{
    OpenAPI_dnf_unit_t *dnf_unit_local_var = ogs_malloc(sizeof(OpenAPI_dnf_unit_t));
    ogs_assert(dnf_unit_local_var);

    dnf_unit_local_var->dnf_unit = dnf_unit;

    return dnf_unit_local_var;
}

void OpenAPI_dnf_unit_free(OpenAPI_dnf_unit_t *dnf_unit)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == dnf_unit) {
        return;
    }
    if (dnf_unit->dnf_unit) {
        OpenAPI_list_for_each(dnf_unit->dnf_unit, node) {
            OpenAPI_atom_free(node->data);
        }
        OpenAPI_list_free(dnf_unit->dnf_unit);
        dnf_unit->dnf_unit = NULL;
    }
    ogs_free(dnf_unit);
}

cJSON *OpenAPI_dnf_unit_convertToJSON(OpenAPI_dnf_unit_t *dnf_unit)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (dnf_unit == NULL) {
        ogs_error("OpenAPI_dnf_unit_convertToJSON() failed [DnfUnit]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!dnf_unit->dnf_unit) {
        ogs_error("OpenAPI_dnf_unit_convertToJSON() failed [dnf_unit]");
        return NULL;
    }
    cJSON *dnf_unitList = cJSON_AddArrayToObject(item, "dnfUnit");
    if (dnf_unitList == NULL) {
        ogs_error("OpenAPI_dnf_unit_convertToJSON() failed [dnf_unit]");
        goto end;
    }
    OpenAPI_list_for_each(dnf_unit->dnf_unit, node) {
        cJSON *itemLocal = OpenAPI_atom_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_dnf_unit_convertToJSON() failed [dnf_unit]");
            goto end;
        }
        cJSON_AddItemToArray(dnf_unitList, itemLocal);
    }

end:
    return item;
}

OpenAPI_dnf_unit_t *OpenAPI_dnf_unit_parseFromJSON(cJSON *dnf_unitJSON)
{
    OpenAPI_dnf_unit_t *dnf_unit_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *dnf_unit = NULL;
    OpenAPI_list_t *dnf_unitList = NULL;
    dnf_unit = cJSON_GetObjectItemCaseSensitive(dnf_unitJSON, "dnfUnit");
    if (!dnf_unit) {
        ogs_error("OpenAPI_dnf_unit_parseFromJSON() failed [dnf_unit]");
        goto end;
    }
        cJSON *dnf_unit_local = NULL;
        if (!cJSON_IsArray(dnf_unit)) {
            ogs_error("OpenAPI_dnf_unit_parseFromJSON() failed [dnf_unit]");
            goto end;
        }

        dnf_unitList = OpenAPI_list_create();

        cJSON_ArrayForEach(dnf_unit_local, dnf_unit) {
            if (!cJSON_IsObject(dnf_unit_local)) {
                ogs_error("OpenAPI_dnf_unit_parseFromJSON() failed [dnf_unit]");
                goto end;
            }
            OpenAPI_atom_t *dnf_unitItem = OpenAPI_atom_parseFromJSON(dnf_unit_local);
            if (!dnf_unitItem) {
                ogs_error("No dnf_unitItem");
                goto end;
            }
            OpenAPI_list_add(dnf_unitList, dnf_unitItem);
        }

    dnf_unit_local_var = OpenAPI_dnf_unit_create (
        dnf_unitList
    );

    return dnf_unit_local_var;
end:
    if (dnf_unitList) {
        OpenAPI_list_for_each(dnf_unitList, node) {
            OpenAPI_atom_free(node->data);
        }
        OpenAPI_list_free(dnf_unitList);
        dnf_unitList = NULL;
    }
    return NULL;
}

OpenAPI_dnf_unit_t *OpenAPI_dnf_unit_copy(OpenAPI_dnf_unit_t *dst, OpenAPI_dnf_unit_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_dnf_unit_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_dnf_unit_convertToJSON() failed");
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

    OpenAPI_dnf_unit_free(dst);
    dst = OpenAPI_dnf_unit_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

