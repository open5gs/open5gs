
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "stored_search_result.h"

OpenAPI_stored_search_result_t *OpenAPI_stored_search_result_create(
    OpenAPI_list_t *nf_instances
)
{
    OpenAPI_stored_search_result_t *stored_search_result_local_var = ogs_malloc(sizeof(OpenAPI_stored_search_result_t));
    ogs_assert(stored_search_result_local_var);

    stored_search_result_local_var->nf_instances = nf_instances;

    return stored_search_result_local_var;
}

void OpenAPI_stored_search_result_free(OpenAPI_stored_search_result_t *stored_search_result)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == stored_search_result) {
        return;
    }
    if (stored_search_result->nf_instances) {
        OpenAPI_list_for_each(stored_search_result->nf_instances, node) {
            OpenAPI_nf_profile_free(node->data);
        }
        OpenAPI_list_free(stored_search_result->nf_instances);
        stored_search_result->nf_instances = NULL;
    }
    ogs_free(stored_search_result);
}

cJSON *OpenAPI_stored_search_result_convertToJSON(OpenAPI_stored_search_result_t *stored_search_result)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (stored_search_result == NULL) {
        ogs_error("OpenAPI_stored_search_result_convertToJSON() failed [StoredSearchResult]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!stored_search_result->nf_instances) {
        ogs_error("OpenAPI_stored_search_result_convertToJSON() failed [nf_instances]");
        return NULL;
    }
    cJSON *nf_instancesList = cJSON_AddArrayToObject(item, "nfInstances");
    if (nf_instancesList == NULL) {
        ogs_error("OpenAPI_stored_search_result_convertToJSON() failed [nf_instances]");
        goto end;
    }
    OpenAPI_list_for_each(stored_search_result->nf_instances, node) {
        cJSON *itemLocal = OpenAPI_nf_profile_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_stored_search_result_convertToJSON() failed [nf_instances]");
            goto end;
        }
        cJSON_AddItemToArray(nf_instancesList, itemLocal);
    }

end:
    return item;
}

OpenAPI_stored_search_result_t *OpenAPI_stored_search_result_parseFromJSON(cJSON *stored_search_resultJSON)
{
    OpenAPI_stored_search_result_t *stored_search_result_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *nf_instances = NULL;
    OpenAPI_list_t *nf_instancesList = NULL;
    nf_instances = cJSON_GetObjectItemCaseSensitive(stored_search_resultJSON, "nfInstances");
    if (!nf_instances) {
        ogs_error("OpenAPI_stored_search_result_parseFromJSON() failed [nf_instances]");
        goto end;
    }
        cJSON *nf_instances_local = NULL;
        if (!cJSON_IsArray(nf_instances)) {
            ogs_error("OpenAPI_stored_search_result_parseFromJSON() failed [nf_instances]");
            goto end;
        }

        nf_instancesList = OpenAPI_list_create();

        cJSON_ArrayForEach(nf_instances_local, nf_instances) {
            if (!cJSON_IsObject(nf_instances_local)) {
                ogs_error("OpenAPI_stored_search_result_parseFromJSON() failed [nf_instances]");
                goto end;
            }
            OpenAPI_nf_profile_t *nf_instancesItem = OpenAPI_nf_profile_parseFromJSON(nf_instances_local);
            if (!nf_instancesItem) {
                ogs_error("No nf_instancesItem");
                goto end;
            }
            OpenAPI_list_add(nf_instancesList, nf_instancesItem);
        }

    stored_search_result_local_var = OpenAPI_stored_search_result_create (
        nf_instancesList
    );

    return stored_search_result_local_var;
end:
    if (nf_instancesList) {
        OpenAPI_list_for_each(nf_instancesList, node) {
            OpenAPI_nf_profile_free(node->data);
        }
        OpenAPI_list_free(nf_instancesList);
        nf_instancesList = NULL;
    }
    return NULL;
}

OpenAPI_stored_search_result_t *OpenAPI_stored_search_result_copy(OpenAPI_stored_search_result_t *dst, OpenAPI_stored_search_result_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_stored_search_result_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_stored_search_result_convertToJSON() failed");
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

    OpenAPI_stored_search_result_free(dst);
    dst = OpenAPI_stored_search_result_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

