
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "shared_scope.h"

OpenAPI_shared_scope_t *OpenAPI_shared_scope_create(
    OpenAPI_list_t *nf_set_id_list
)
{
    OpenAPI_shared_scope_t *shared_scope_local_var = ogs_malloc(sizeof(OpenAPI_shared_scope_t));
    ogs_assert(shared_scope_local_var);

    shared_scope_local_var->nf_set_id_list = nf_set_id_list;

    return shared_scope_local_var;
}

void OpenAPI_shared_scope_free(OpenAPI_shared_scope_t *shared_scope)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == shared_scope) {
        return;
    }
    if (shared_scope->nf_set_id_list) {
        OpenAPI_list_for_each(shared_scope->nf_set_id_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(shared_scope->nf_set_id_list);
        shared_scope->nf_set_id_list = NULL;
    }
    ogs_free(shared_scope);
}

cJSON *OpenAPI_shared_scope_convertToJSON(OpenAPI_shared_scope_t *shared_scope)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (shared_scope == NULL) {
        ogs_error("OpenAPI_shared_scope_convertToJSON() failed [SharedScope]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (shared_scope->nf_set_id_list) {
    cJSON *nf_set_id_listList = cJSON_AddArrayToObject(item, "nfSetIdList");
    if (nf_set_id_listList == NULL) {
        ogs_error("OpenAPI_shared_scope_convertToJSON() failed [nf_set_id_list]");
        goto end;
    }
    OpenAPI_list_for_each(shared_scope->nf_set_id_list, node) {
        if (cJSON_AddStringToObject(nf_set_id_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_shared_scope_convertToJSON() failed [nf_set_id_list]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_shared_scope_t *OpenAPI_shared_scope_parseFromJSON(cJSON *shared_scopeJSON)
{
    OpenAPI_shared_scope_t *shared_scope_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *nf_set_id_list = NULL;
    OpenAPI_list_t *nf_set_id_listList = NULL;
    nf_set_id_list = cJSON_GetObjectItemCaseSensitive(shared_scopeJSON, "nfSetIdList");
    if (nf_set_id_list) {
        cJSON *nf_set_id_list_local = NULL;
        if (!cJSON_IsArray(nf_set_id_list)) {
            ogs_error("OpenAPI_shared_scope_parseFromJSON() failed [nf_set_id_list]");
            goto end;
        }

        nf_set_id_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(nf_set_id_list_local, nf_set_id_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(nf_set_id_list_local)) {
                ogs_error("OpenAPI_shared_scope_parseFromJSON() failed [nf_set_id_list]");
                goto end;
            }
            OpenAPI_list_add(nf_set_id_listList, ogs_strdup(nf_set_id_list_local->valuestring));
        }
    }

    shared_scope_local_var = OpenAPI_shared_scope_create (
        nf_set_id_list ? nf_set_id_listList : NULL
    );

    return shared_scope_local_var;
end:
    if (nf_set_id_listList) {
        OpenAPI_list_for_each(nf_set_id_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(nf_set_id_listList);
        nf_set_id_listList = NULL;
    }
    return NULL;
}

OpenAPI_shared_scope_t *OpenAPI_shared_scope_copy(OpenAPI_shared_scope_t *dst, OpenAPI_shared_scope_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_shared_scope_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_shared_scope_convertToJSON() failed");
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

    OpenAPI_shared_scope_free(dst);
    dst = OpenAPI_shared_scope_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

