
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "slice_scope_per_plmn_1.h"

OpenAPI_slice_scope_per_plmn_1_t *OpenAPI_slice_scope_per_plmn_1_create(
    OpenAPI_plmn_id_t *plmn,
    OpenAPI_list_t *slice_scope
)
{
    OpenAPI_slice_scope_per_plmn_1_t *slice_scope_per_plmn_1_local_var = ogs_malloc(sizeof(OpenAPI_slice_scope_per_plmn_1_t));
    ogs_assert(slice_scope_per_plmn_1_local_var);

    slice_scope_per_plmn_1_local_var->plmn = plmn;
    slice_scope_per_plmn_1_local_var->slice_scope = slice_scope;

    return slice_scope_per_plmn_1_local_var;
}

void OpenAPI_slice_scope_per_plmn_1_free(OpenAPI_slice_scope_per_plmn_1_t *slice_scope_per_plmn_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == slice_scope_per_plmn_1) {
        return;
    }
    if (slice_scope_per_plmn_1->plmn) {
        OpenAPI_plmn_id_free(slice_scope_per_plmn_1->plmn);
        slice_scope_per_plmn_1->plmn = NULL;
    }
    if (slice_scope_per_plmn_1->slice_scope) {
        OpenAPI_list_for_each(slice_scope_per_plmn_1->slice_scope, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(slice_scope_per_plmn_1->slice_scope);
        slice_scope_per_plmn_1->slice_scope = NULL;
    }
    ogs_free(slice_scope_per_plmn_1);
}

cJSON *OpenAPI_slice_scope_per_plmn_1_convertToJSON(OpenAPI_slice_scope_per_plmn_1_t *slice_scope_per_plmn_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (slice_scope_per_plmn_1 == NULL) {
        ogs_error("OpenAPI_slice_scope_per_plmn_1_convertToJSON() failed [SliceScopePerPlmn_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!slice_scope_per_plmn_1->plmn) {
        ogs_error("OpenAPI_slice_scope_per_plmn_1_convertToJSON() failed [plmn]");
        return NULL;
    }
    cJSON *plmn_local_JSON = OpenAPI_plmn_id_convertToJSON(slice_scope_per_plmn_1->plmn);
    if (plmn_local_JSON == NULL) {
        ogs_error("OpenAPI_slice_scope_per_plmn_1_convertToJSON() failed [plmn]");
        goto end;
    }
    cJSON_AddItemToObject(item, "plmn", plmn_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_slice_scope_per_plmn_1_convertToJSON() failed [plmn]");
        goto end;
    }

    if (!slice_scope_per_plmn_1->slice_scope) {
        ogs_error("OpenAPI_slice_scope_per_plmn_1_convertToJSON() failed [slice_scope]");
        return NULL;
    }
    cJSON *slice_scopeList = cJSON_AddArrayToObject(item, "sliceScope");
    if (slice_scopeList == NULL) {
        ogs_error("OpenAPI_slice_scope_per_plmn_1_convertToJSON() failed [slice_scope]");
        goto end;
    }
    OpenAPI_list_for_each(slice_scope_per_plmn_1->slice_scope, node) {
        cJSON *itemLocal = OpenAPI_snssai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_slice_scope_per_plmn_1_convertToJSON() failed [slice_scope]");
            goto end;
        }
        cJSON_AddItemToArray(slice_scopeList, itemLocal);
    }

end:
    return item;
}

OpenAPI_slice_scope_per_plmn_1_t *OpenAPI_slice_scope_per_plmn_1_parseFromJSON(cJSON *slice_scope_per_plmn_1JSON)
{
    OpenAPI_slice_scope_per_plmn_1_t *slice_scope_per_plmn_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *plmn = NULL;
    OpenAPI_plmn_id_t *plmn_local_nonprim = NULL;
    cJSON *slice_scope = NULL;
    OpenAPI_list_t *slice_scopeList = NULL;
    plmn = cJSON_GetObjectItemCaseSensitive(slice_scope_per_plmn_1JSON, "plmn");
    if (!plmn) {
        ogs_error("OpenAPI_slice_scope_per_plmn_1_parseFromJSON() failed [plmn]");
        goto end;
    }
    plmn_local_nonprim = OpenAPI_plmn_id_parseFromJSON(plmn);
    if (!plmn_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_parseFromJSON failed [plmn]");
        goto end;
    }

    slice_scope = cJSON_GetObjectItemCaseSensitive(slice_scope_per_plmn_1JSON, "sliceScope");
    if (!slice_scope) {
        ogs_error("OpenAPI_slice_scope_per_plmn_1_parseFromJSON() failed [slice_scope]");
        goto end;
    }
        cJSON *slice_scope_local = NULL;
        if (!cJSON_IsArray(slice_scope)) {
            ogs_error("OpenAPI_slice_scope_per_plmn_1_parseFromJSON() failed [slice_scope]");
            goto end;
        }

        slice_scopeList = OpenAPI_list_create();

        cJSON_ArrayForEach(slice_scope_local, slice_scope) {
            if (!cJSON_IsObject(slice_scope_local)) {
                ogs_error("OpenAPI_slice_scope_per_plmn_1_parseFromJSON() failed [slice_scope]");
                goto end;
            }
            OpenAPI_snssai_t *slice_scopeItem = OpenAPI_snssai_parseFromJSON(slice_scope_local);
            if (!slice_scopeItem) {
                ogs_error("No slice_scopeItem");
                goto end;
            }
            OpenAPI_list_add(slice_scopeList, slice_scopeItem);
        }

    slice_scope_per_plmn_1_local_var = OpenAPI_slice_scope_per_plmn_1_create (
        plmn_local_nonprim,
        slice_scopeList
    );

    return slice_scope_per_plmn_1_local_var;
end:
    if (plmn_local_nonprim) {
        OpenAPI_plmn_id_free(plmn_local_nonprim);
        plmn_local_nonprim = NULL;
    }
    if (slice_scopeList) {
        OpenAPI_list_for_each(slice_scopeList, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(slice_scopeList);
        slice_scopeList = NULL;
    }
    return NULL;
}

OpenAPI_slice_scope_per_plmn_1_t *OpenAPI_slice_scope_per_plmn_1_copy(OpenAPI_slice_scope_per_plmn_1_t *dst, OpenAPI_slice_scope_per_plmn_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_slice_scope_per_plmn_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_slice_scope_per_plmn_1_convertToJSON() failed");
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

    OpenAPI_slice_scope_per_plmn_1_free(dst);
    dst = OpenAPI_slice_scope_per_plmn_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

