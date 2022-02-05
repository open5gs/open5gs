
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "plmn_route_selection_descriptor.h"

OpenAPI_plmn_route_selection_descriptor_t *OpenAPI_plmn_route_selection_descriptor_create(
    OpenAPI_plmn_id_1_t *serving_plmn,
    OpenAPI_list_t *snssai_route_sel_descs
)
{
    OpenAPI_plmn_route_selection_descriptor_t *plmn_route_selection_descriptor_local_var = ogs_malloc(sizeof(OpenAPI_plmn_route_selection_descriptor_t));
    ogs_assert(plmn_route_selection_descriptor_local_var);

    plmn_route_selection_descriptor_local_var->serving_plmn = serving_plmn;
    plmn_route_selection_descriptor_local_var->snssai_route_sel_descs = snssai_route_sel_descs;

    return plmn_route_selection_descriptor_local_var;
}

void OpenAPI_plmn_route_selection_descriptor_free(OpenAPI_plmn_route_selection_descriptor_t *plmn_route_selection_descriptor)
{
    if (NULL == plmn_route_selection_descriptor) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_plmn_id_1_free(plmn_route_selection_descriptor->serving_plmn);
    OpenAPI_list_for_each(plmn_route_selection_descriptor->snssai_route_sel_descs, node) {
        OpenAPI_snssai_route_selection_descriptor_free(node->data);
    }
    OpenAPI_list_free(plmn_route_selection_descriptor->snssai_route_sel_descs);
    ogs_free(plmn_route_selection_descriptor);
}

cJSON *OpenAPI_plmn_route_selection_descriptor_convertToJSON(OpenAPI_plmn_route_selection_descriptor_t *plmn_route_selection_descriptor)
{
    cJSON *item = NULL;

    if (plmn_route_selection_descriptor == NULL) {
        ogs_error("OpenAPI_plmn_route_selection_descriptor_convertToJSON() failed [PlmnRouteSelectionDescriptor]");
        return NULL;
    }

    item = cJSON_CreateObject();
    cJSON *serving_plmn_local_JSON = OpenAPI_plmn_id_1_convertToJSON(plmn_route_selection_descriptor->serving_plmn);
    if (serving_plmn_local_JSON == NULL) {
        ogs_error("OpenAPI_plmn_route_selection_descriptor_convertToJSON() failed [serving_plmn]");
        goto end;
    }
    cJSON_AddItemToObject(item, "servingPlmn", serving_plmn_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_plmn_route_selection_descriptor_convertToJSON() failed [serving_plmn]");
        goto end;
    }

    if (plmn_route_selection_descriptor->snssai_route_sel_descs) {
    cJSON *snssai_route_sel_descsList = cJSON_AddArrayToObject(item, "snssaiRouteSelDescs");
    if (snssai_route_sel_descsList == NULL) {
        ogs_error("OpenAPI_plmn_route_selection_descriptor_convertToJSON() failed [snssai_route_sel_descs]");
        goto end;
    }

    OpenAPI_lnode_t *snssai_route_sel_descs_node;
    if (plmn_route_selection_descriptor->snssai_route_sel_descs) {
        OpenAPI_list_for_each(plmn_route_selection_descriptor->snssai_route_sel_descs, snssai_route_sel_descs_node) {
            cJSON *itemLocal = OpenAPI_snssai_route_selection_descriptor_convertToJSON(snssai_route_sel_descs_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_plmn_route_selection_descriptor_convertToJSON() failed [snssai_route_sel_descs]");
                goto end;
            }
            cJSON_AddItemToArray(snssai_route_sel_descsList, itemLocal);
        }
    }
    }

end:
    return item;
}

OpenAPI_plmn_route_selection_descriptor_t *OpenAPI_plmn_route_selection_descriptor_parseFromJSON(cJSON *plmn_route_selection_descriptorJSON)
{
    OpenAPI_plmn_route_selection_descriptor_t *plmn_route_selection_descriptor_local_var = NULL;
    cJSON *serving_plmn = cJSON_GetObjectItemCaseSensitive(plmn_route_selection_descriptorJSON, "servingPlmn");
    if (!serving_plmn) {
        ogs_error("OpenAPI_plmn_route_selection_descriptor_parseFromJSON() failed [serving_plmn]");
        goto end;
    }

    OpenAPI_plmn_id_1_t *serving_plmn_local_nonprim = NULL;
    serving_plmn_local_nonprim = OpenAPI_plmn_id_1_parseFromJSON(serving_plmn);

    cJSON *snssai_route_sel_descs = cJSON_GetObjectItemCaseSensitive(plmn_route_selection_descriptorJSON, "snssaiRouteSelDescs");

    OpenAPI_list_t *snssai_route_sel_descsList;
    if (snssai_route_sel_descs) {
    cJSON *snssai_route_sel_descs_local_nonprimitive;
    if (!cJSON_IsArray(snssai_route_sel_descs)){
        ogs_error("OpenAPI_plmn_route_selection_descriptor_parseFromJSON() failed [snssai_route_sel_descs]");
        goto end;
    }

    snssai_route_sel_descsList = OpenAPI_list_create();

    cJSON_ArrayForEach(snssai_route_sel_descs_local_nonprimitive, snssai_route_sel_descs ) {
        if (!cJSON_IsObject(snssai_route_sel_descs_local_nonprimitive)) {
            ogs_error("OpenAPI_plmn_route_selection_descriptor_parseFromJSON() failed [snssai_route_sel_descs]");
            goto end;
        }
        OpenAPI_snssai_route_selection_descriptor_t *snssai_route_sel_descsItem = OpenAPI_snssai_route_selection_descriptor_parseFromJSON(snssai_route_sel_descs_local_nonprimitive);

        if (!snssai_route_sel_descsItem) {
            ogs_error("No snssai_route_sel_descsItem");
            OpenAPI_list_free(snssai_route_sel_descsList);
            goto end;
        }

        OpenAPI_list_add(snssai_route_sel_descsList, snssai_route_sel_descsItem);
    }
    }

    plmn_route_selection_descriptor_local_var = OpenAPI_plmn_route_selection_descriptor_create (
        serving_plmn_local_nonprim,
        snssai_route_sel_descs ? snssai_route_sel_descsList : NULL
    );

    return plmn_route_selection_descriptor_local_var;
end:
    return NULL;
}

OpenAPI_plmn_route_selection_descriptor_t *OpenAPI_plmn_route_selection_descriptor_copy(OpenAPI_plmn_route_selection_descriptor_t *dst, OpenAPI_plmn_route_selection_descriptor_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_plmn_route_selection_descriptor_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_plmn_route_selection_descriptor_convertToJSON() failed");
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

    OpenAPI_plmn_route_selection_descriptor_free(dst);
    dst = OpenAPI_plmn_route_selection_descriptor_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

