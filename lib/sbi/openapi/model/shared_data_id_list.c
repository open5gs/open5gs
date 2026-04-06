
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "shared_data_id_list.h"

OpenAPI_shared_data_id_list_t *OpenAPI_shared_data_id_list_create(
    OpenAPI_list_t *shared_data_ids
)
{
    OpenAPI_shared_data_id_list_t *shared_data_id_list_local_var = ogs_malloc(sizeof(OpenAPI_shared_data_id_list_t));
    ogs_assert(shared_data_id_list_local_var);

    shared_data_id_list_local_var->shared_data_ids = shared_data_ids;

    return shared_data_id_list_local_var;
}

void OpenAPI_shared_data_id_list_free(OpenAPI_shared_data_id_list_t *shared_data_id_list)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == shared_data_id_list) {
        return;
    }
    if (shared_data_id_list->shared_data_ids) {
        OpenAPI_list_for_each(shared_data_id_list->shared_data_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(shared_data_id_list->shared_data_ids);
        shared_data_id_list->shared_data_ids = NULL;
    }
    ogs_free(shared_data_id_list);
}

cJSON *OpenAPI_shared_data_id_list_convertToJSON(OpenAPI_shared_data_id_list_t *shared_data_id_list)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (shared_data_id_list == NULL) {
        ogs_error("OpenAPI_shared_data_id_list_convertToJSON() failed [SharedDataIdList]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!shared_data_id_list->shared_data_ids) {
        ogs_error("OpenAPI_shared_data_id_list_convertToJSON() failed [shared_data_ids]");
        return NULL;
    }
    cJSON *shared_data_idsList = cJSON_AddArrayToObject(item, "sharedDataIds");
    if (shared_data_idsList == NULL) {
        ogs_error("OpenAPI_shared_data_id_list_convertToJSON() failed [shared_data_ids]");
        goto end;
    }
    OpenAPI_list_for_each(shared_data_id_list->shared_data_ids, node) {
        if (cJSON_AddStringToObject(shared_data_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_shared_data_id_list_convertToJSON() failed [shared_data_ids]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_shared_data_id_list_t *OpenAPI_shared_data_id_list_parseFromJSON(cJSON *shared_data_id_listJSON)
{
    OpenAPI_shared_data_id_list_t *shared_data_id_list_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *shared_data_ids = NULL;
    OpenAPI_list_t *shared_data_idsList = NULL;
    shared_data_ids = cJSON_GetObjectItemCaseSensitive(shared_data_id_listJSON, "sharedDataIds");
    if (!shared_data_ids) {
        ogs_error("OpenAPI_shared_data_id_list_parseFromJSON() failed [shared_data_ids]");
        goto end;
    }
        cJSON *shared_data_ids_local = NULL;
        if (!cJSON_IsArray(shared_data_ids)) {
            ogs_error("OpenAPI_shared_data_id_list_parseFromJSON() failed [shared_data_ids]");
            goto end;
        }

        shared_data_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(shared_data_ids_local, shared_data_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(shared_data_ids_local)) {
                ogs_error("OpenAPI_shared_data_id_list_parseFromJSON() failed [shared_data_ids]");
                goto end;
            }
            OpenAPI_list_add(shared_data_idsList, ogs_strdup(shared_data_ids_local->valuestring));
        }

    shared_data_id_list_local_var = OpenAPI_shared_data_id_list_create (
        shared_data_idsList
    );

    return shared_data_id_list_local_var;
end:
    if (shared_data_idsList) {
        OpenAPI_list_for_each(shared_data_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(shared_data_idsList);
        shared_data_idsList = NULL;
    }
    return NULL;
}

OpenAPI_shared_data_id_list_t *OpenAPI_shared_data_id_list_copy(OpenAPI_shared_data_id_list_t *dst, OpenAPI_shared_data_id_list_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_shared_data_id_list_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_shared_data_id_list_convertToJSON() failed");
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

    OpenAPI_shared_data_id_list_free(dst);
    dst = OpenAPI_shared_data_id_list_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

