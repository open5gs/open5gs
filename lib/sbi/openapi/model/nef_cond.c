
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nef_cond.h"

OpenAPI_nef_cond_t *OpenAPI_nef_cond_create(
    OpenAPI_list_t *event_ids
    )
{
    OpenAPI_nef_cond_t *nef_cond_local_var = OpenAPI_malloc(sizeof(OpenAPI_nef_cond_t));
    if (!nef_cond_local_var) {
        return NULL;
    }
    nef_cond_local_var->event_ids = event_ids;

    return nef_cond_local_var;
}

void OpenAPI_nef_cond_free(OpenAPI_nef_cond_t *nef_cond)
{
    if (NULL == nef_cond) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(nef_cond->event_ids, node) {
        OpenAPI_event_id_free(node->data);
    }
    OpenAPI_list_free(nef_cond->event_ids);
    ogs_free(nef_cond);
}

cJSON *OpenAPI_nef_cond_convertToJSON(OpenAPI_nef_cond_t *nef_cond)
{
    cJSON *item = NULL;

    if (nef_cond == NULL) {
        ogs_error("OpenAPI_nef_cond_convertToJSON() failed [NefCond]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (nef_cond->event_ids) {
        cJSON *event_idsList = cJSON_AddArrayToObject(item, "eventIds");
        if (event_idsList == NULL) {
            ogs_error("OpenAPI_nef_cond_convertToJSON() failed [event_ids]");
            goto end;
        }

        OpenAPI_lnode_t *event_ids_node;
        if (nef_cond->event_ids) {
            OpenAPI_list_for_each(nef_cond->event_ids, event_ids_node) {
                cJSON *itemLocal = OpenAPI_event_id_convertToJSON(event_ids_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_nef_cond_convertToJSON() failed [event_ids]");
                    goto end;
                }
                cJSON_AddItemToArray(event_idsList, itemLocal);
            }
        }
    }

end:
    return item;
}

OpenAPI_nef_cond_t *OpenAPI_nef_cond_parseFromJSON(cJSON *nef_condJSON)
{
    OpenAPI_nef_cond_t *nef_cond_local_var = NULL;
    cJSON *event_ids = cJSON_GetObjectItemCaseSensitive(nef_condJSON, "eventIds");

    OpenAPI_list_t *event_idsList;
    if (event_ids) {
        cJSON *event_ids_local_nonprimitive;
        if (!cJSON_IsArray(event_ids)) {
            ogs_error("OpenAPI_nef_cond_parseFromJSON() failed [event_ids]");
            goto end;
        }

        event_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(event_ids_local_nonprimitive, event_ids ) {
            if (!cJSON_IsObject(event_ids_local_nonprimitive)) {
                ogs_error("OpenAPI_nef_cond_parseFromJSON() failed [event_ids]");
                goto end;
            }
            OpenAPI_event_id_t *event_idsItem = OpenAPI_event_id_parseFromJSON(event_ids_local_nonprimitive);

            OpenAPI_list_add(event_idsList, event_idsItem);
        }
    }

    nef_cond_local_var = OpenAPI_nef_cond_create (
        event_ids ? event_idsList : NULL
        );

    return nef_cond_local_var;
end:
    return NULL;
}

OpenAPI_nef_cond_t *OpenAPI_nef_cond_copy(OpenAPI_nef_cond_t *dst, OpenAPI_nef_cond_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nef_cond_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nef_cond_convertToJSON() failed");
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

    OpenAPI_nef_cond_free(dst);
    dst = OpenAPI_nef_cond_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

