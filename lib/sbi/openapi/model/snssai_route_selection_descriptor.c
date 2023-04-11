
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "snssai_route_selection_descriptor.h"

OpenAPI_snssai_route_selection_descriptor_t *OpenAPI_snssai_route_selection_descriptor_create(
    OpenAPI_snssai_t *snssai,
    OpenAPI_list_t *dnn_route_sel_descs
)
{
    OpenAPI_snssai_route_selection_descriptor_t *snssai_route_selection_descriptor_local_var = ogs_malloc(sizeof(OpenAPI_snssai_route_selection_descriptor_t));
    ogs_assert(snssai_route_selection_descriptor_local_var);

    snssai_route_selection_descriptor_local_var->snssai = snssai;
    snssai_route_selection_descriptor_local_var->dnn_route_sel_descs = dnn_route_sel_descs;

    return snssai_route_selection_descriptor_local_var;
}

void OpenAPI_snssai_route_selection_descriptor_free(OpenAPI_snssai_route_selection_descriptor_t *snssai_route_selection_descriptor)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == snssai_route_selection_descriptor) {
        return;
    }
    if (snssai_route_selection_descriptor->snssai) {
        OpenAPI_snssai_free(snssai_route_selection_descriptor->snssai);
        snssai_route_selection_descriptor->snssai = NULL;
    }
    if (snssai_route_selection_descriptor->dnn_route_sel_descs) {
        OpenAPI_list_for_each(snssai_route_selection_descriptor->dnn_route_sel_descs, node) {
            OpenAPI_dnn_route_selection_descriptor_free(node->data);
        }
        OpenAPI_list_free(snssai_route_selection_descriptor->dnn_route_sel_descs);
        snssai_route_selection_descriptor->dnn_route_sel_descs = NULL;
    }
    ogs_free(snssai_route_selection_descriptor);
}

cJSON *OpenAPI_snssai_route_selection_descriptor_convertToJSON(OpenAPI_snssai_route_selection_descriptor_t *snssai_route_selection_descriptor)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (snssai_route_selection_descriptor == NULL) {
        ogs_error("OpenAPI_snssai_route_selection_descriptor_convertToJSON() failed [SnssaiRouteSelectionDescriptor]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!snssai_route_selection_descriptor->snssai) {
        ogs_error("OpenAPI_snssai_route_selection_descriptor_convertToJSON() failed [snssai]");
        return NULL;
    }
    cJSON *snssai_local_JSON = OpenAPI_snssai_convertToJSON(snssai_route_selection_descriptor->snssai);
    if (snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_snssai_route_selection_descriptor_convertToJSON() failed [snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "snssai", snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_snssai_route_selection_descriptor_convertToJSON() failed [snssai]");
        goto end;
    }

    if (snssai_route_selection_descriptor->dnn_route_sel_descs) {
    cJSON *dnn_route_sel_descsList = cJSON_AddArrayToObject(item, "dnnRouteSelDescs");
    if (dnn_route_sel_descsList == NULL) {
        ogs_error("OpenAPI_snssai_route_selection_descriptor_convertToJSON() failed [dnn_route_sel_descs]");
        goto end;
    }
    OpenAPI_list_for_each(snssai_route_selection_descriptor->dnn_route_sel_descs, node) {
        cJSON *itemLocal = OpenAPI_dnn_route_selection_descriptor_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_snssai_route_selection_descriptor_convertToJSON() failed [dnn_route_sel_descs]");
            goto end;
        }
        cJSON_AddItemToArray(dnn_route_sel_descsList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_snssai_route_selection_descriptor_t *OpenAPI_snssai_route_selection_descriptor_parseFromJSON(cJSON *snssai_route_selection_descriptorJSON)
{
    OpenAPI_snssai_route_selection_descriptor_t *snssai_route_selection_descriptor_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *snssai = NULL;
    OpenAPI_snssai_t *snssai_local_nonprim = NULL;
    cJSON *dnn_route_sel_descs = NULL;
    OpenAPI_list_t *dnn_route_sel_descsList = NULL;
    snssai = cJSON_GetObjectItemCaseSensitive(snssai_route_selection_descriptorJSON, "snssai");
    if (!snssai) {
        ogs_error("OpenAPI_snssai_route_selection_descriptor_parseFromJSON() failed [snssai]");
        goto end;
    }
    snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(snssai);
    if (!snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [snssai]");
        goto end;
    }

    dnn_route_sel_descs = cJSON_GetObjectItemCaseSensitive(snssai_route_selection_descriptorJSON, "dnnRouteSelDescs");
    if (dnn_route_sel_descs) {
        cJSON *dnn_route_sel_descs_local = NULL;
        if (!cJSON_IsArray(dnn_route_sel_descs)) {
            ogs_error("OpenAPI_snssai_route_selection_descriptor_parseFromJSON() failed [dnn_route_sel_descs]");
            goto end;
        }

        dnn_route_sel_descsList = OpenAPI_list_create();

        cJSON_ArrayForEach(dnn_route_sel_descs_local, dnn_route_sel_descs) {
            if (!cJSON_IsObject(dnn_route_sel_descs_local)) {
                ogs_error("OpenAPI_snssai_route_selection_descriptor_parseFromJSON() failed [dnn_route_sel_descs]");
                goto end;
            }
            OpenAPI_dnn_route_selection_descriptor_t *dnn_route_sel_descsItem = OpenAPI_dnn_route_selection_descriptor_parseFromJSON(dnn_route_sel_descs_local);
            if (!dnn_route_sel_descsItem) {
                ogs_error("No dnn_route_sel_descsItem");
                goto end;
            }
            OpenAPI_list_add(dnn_route_sel_descsList, dnn_route_sel_descsItem);
        }
    }

    snssai_route_selection_descriptor_local_var = OpenAPI_snssai_route_selection_descriptor_create (
        snssai_local_nonprim,
        dnn_route_sel_descs ? dnn_route_sel_descsList : NULL
    );

    return snssai_route_selection_descriptor_local_var;
end:
    if (snssai_local_nonprim) {
        OpenAPI_snssai_free(snssai_local_nonprim);
        snssai_local_nonprim = NULL;
    }
    if (dnn_route_sel_descsList) {
        OpenAPI_list_for_each(dnn_route_sel_descsList, node) {
            OpenAPI_dnn_route_selection_descriptor_free(node->data);
        }
        OpenAPI_list_free(dnn_route_sel_descsList);
        dnn_route_sel_descsList = NULL;
    }
    return NULL;
}

OpenAPI_snssai_route_selection_descriptor_t *OpenAPI_snssai_route_selection_descriptor_copy(OpenAPI_snssai_route_selection_descriptor_t *dst, OpenAPI_snssai_route_selection_descriptor_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_snssai_route_selection_descriptor_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_snssai_route_selection_descriptor_convertToJSON() failed");
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

    OpenAPI_snssai_route_selection_descriptor_free(dst);
    dst = OpenAPI_snssai_route_selection_descriptor_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

