
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "complex_query.h"

OpenAPI_complex_query_t *OpenAPI_complex_query_create(
    OpenAPI_list_t *cnf_units,
    OpenAPI_list_t *dnf_units
)
{
    OpenAPI_complex_query_t *complex_query_local_var = ogs_malloc(sizeof(OpenAPI_complex_query_t));
    ogs_assert(complex_query_local_var);

    complex_query_local_var->cnf_units = cnf_units;
    complex_query_local_var->dnf_units = dnf_units;

    return complex_query_local_var;
}

void OpenAPI_complex_query_free(OpenAPI_complex_query_t *complex_query)
{
    if (NULL == complex_query) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(complex_query->cnf_units, node) {
        OpenAPI_cnf_unit_free(node->data);
    }
    OpenAPI_list_free(complex_query->cnf_units);
    OpenAPI_list_for_each(complex_query->dnf_units, node) {
        OpenAPI_dnf_unit_free(node->data);
    }
    OpenAPI_list_free(complex_query->dnf_units);
    ogs_free(complex_query);
}

cJSON *OpenAPI_complex_query_convertToJSON(OpenAPI_complex_query_t *complex_query)
{
    cJSON *item = NULL;

    if (complex_query == NULL) {
        ogs_error("OpenAPI_complex_query_convertToJSON() failed [ComplexQuery]");
        return NULL;
    }

    item = cJSON_CreateObject();
    cJSON *cnf_unitsList = cJSON_AddArrayToObject(item, "cnfUnits");
    if (cnf_unitsList == NULL) {
        ogs_error("OpenAPI_complex_query_convertToJSON() failed [cnf_units]");
        goto end;
    }

    OpenAPI_lnode_t *cnf_units_node;
    if (complex_query->cnf_units) {
        OpenAPI_list_for_each(complex_query->cnf_units, cnf_units_node) {
            cJSON *itemLocal = OpenAPI_cnf_unit_convertToJSON(cnf_units_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_complex_query_convertToJSON() failed [cnf_units]");
                goto end;
            }
            cJSON_AddItemToArray(cnf_unitsList, itemLocal);
        }
    }

    cJSON *dnf_unitsList = cJSON_AddArrayToObject(item, "dnfUnits");
    if (dnf_unitsList == NULL) {
        ogs_error("OpenAPI_complex_query_convertToJSON() failed [dnf_units]");
        goto end;
    }

    OpenAPI_lnode_t *dnf_units_node;
    if (complex_query->dnf_units) {
        OpenAPI_list_for_each(complex_query->dnf_units, dnf_units_node) {
            cJSON *itemLocal = OpenAPI_dnf_unit_convertToJSON(dnf_units_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_complex_query_convertToJSON() failed [dnf_units]");
                goto end;
            }
            cJSON_AddItemToArray(dnf_unitsList, itemLocal);
        }
    }

end:
    return item;
}

OpenAPI_complex_query_t *OpenAPI_complex_query_parseFromJSON(cJSON *complex_queryJSON)
{
    OpenAPI_complex_query_t *complex_query_local_var = NULL;
    cJSON *cnf_units = cJSON_GetObjectItemCaseSensitive(complex_queryJSON, "cnfUnits");
    if (!cnf_units) {
        ogs_error("OpenAPI_complex_query_parseFromJSON() failed [cnf_units]");
        goto end;
    }

    OpenAPI_list_t *cnf_unitsList;
    cJSON *cnf_units_local_nonprimitive;
    if (!cJSON_IsArray(cnf_units)){
        ogs_error("OpenAPI_complex_query_parseFromJSON() failed [cnf_units]");
        goto end;
    }

    cnf_unitsList = OpenAPI_list_create();

    cJSON_ArrayForEach(cnf_units_local_nonprimitive, cnf_units ) {
        if (!cJSON_IsObject(cnf_units_local_nonprimitive)) {
            ogs_error("OpenAPI_complex_query_parseFromJSON() failed [cnf_units]");
            goto end;
        }
        OpenAPI_cnf_unit_t *cnf_unitsItem = OpenAPI_cnf_unit_parseFromJSON(cnf_units_local_nonprimitive);

        if (!cnf_unitsItem) {
            ogs_error("No cnf_unitsItem");
            OpenAPI_list_free(cnf_unitsList);
            goto end;
        }

        OpenAPI_list_add(cnf_unitsList, cnf_unitsItem);
    }

    cJSON *dnf_units = cJSON_GetObjectItemCaseSensitive(complex_queryJSON, "dnfUnits");
    if (!dnf_units) {
        ogs_error("OpenAPI_complex_query_parseFromJSON() failed [dnf_units]");
        goto end;
    }

    OpenAPI_list_t *dnf_unitsList;
    cJSON *dnf_units_local_nonprimitive;
    if (!cJSON_IsArray(dnf_units)){
        ogs_error("OpenAPI_complex_query_parseFromJSON() failed [dnf_units]");
        goto end;
    }

    dnf_unitsList = OpenAPI_list_create();

    cJSON_ArrayForEach(dnf_units_local_nonprimitive, dnf_units ) {
        if (!cJSON_IsObject(dnf_units_local_nonprimitive)) {
            ogs_error("OpenAPI_complex_query_parseFromJSON() failed [dnf_units]");
            goto end;
        }
        OpenAPI_dnf_unit_t *dnf_unitsItem = OpenAPI_dnf_unit_parseFromJSON(dnf_units_local_nonprimitive);

        if (!dnf_unitsItem) {
            ogs_error("No dnf_unitsItem");
            OpenAPI_list_free(dnf_unitsList);
            goto end;
        }

        OpenAPI_list_add(dnf_unitsList, dnf_unitsItem);
    }

    complex_query_local_var = OpenAPI_complex_query_create (
        cnf_unitsList,
        dnf_unitsList
    );

    return complex_query_local_var;
end:
    return NULL;
}

OpenAPI_complex_query_t *OpenAPI_complex_query_copy(OpenAPI_complex_query_t *dst, OpenAPI_complex_query_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_complex_query_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_complex_query_convertToJSON() failed");
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

    OpenAPI_complex_query_free(dst);
    dst = OpenAPI_complex_query_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

