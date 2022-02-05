
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "guami_list_cond.h"

OpenAPI_guami_list_cond_t *OpenAPI_guami_list_cond_create(
    OpenAPI_list_t *guami_list
)
{
    OpenAPI_guami_list_cond_t *guami_list_cond_local_var = ogs_malloc(sizeof(OpenAPI_guami_list_cond_t));
    ogs_assert(guami_list_cond_local_var);

    guami_list_cond_local_var->guami_list = guami_list;

    return guami_list_cond_local_var;
}

void OpenAPI_guami_list_cond_free(OpenAPI_guami_list_cond_t *guami_list_cond)
{
    if (NULL == guami_list_cond) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(guami_list_cond->guami_list, node) {
        OpenAPI_guami_free(node->data);
    }
    OpenAPI_list_free(guami_list_cond->guami_list);
    ogs_free(guami_list_cond);
}

cJSON *OpenAPI_guami_list_cond_convertToJSON(OpenAPI_guami_list_cond_t *guami_list_cond)
{
    cJSON *item = NULL;

    if (guami_list_cond == NULL) {
        ogs_error("OpenAPI_guami_list_cond_convertToJSON() failed [GuamiListCond]");
        return NULL;
    }

    item = cJSON_CreateObject();
    cJSON *guami_listList = cJSON_AddArrayToObject(item, "guamiList");
    if (guami_listList == NULL) {
        ogs_error("OpenAPI_guami_list_cond_convertToJSON() failed [guami_list]");
        goto end;
    }

    OpenAPI_lnode_t *guami_list_node;
    if (guami_list_cond->guami_list) {
        OpenAPI_list_for_each(guami_list_cond->guami_list, guami_list_node) {
            cJSON *itemLocal = OpenAPI_guami_convertToJSON(guami_list_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_guami_list_cond_convertToJSON() failed [guami_list]");
                goto end;
            }
            cJSON_AddItemToArray(guami_listList, itemLocal);
        }
    }

end:
    return item;
}

OpenAPI_guami_list_cond_t *OpenAPI_guami_list_cond_parseFromJSON(cJSON *guami_list_condJSON)
{
    OpenAPI_guami_list_cond_t *guami_list_cond_local_var = NULL;
    cJSON *guami_list = cJSON_GetObjectItemCaseSensitive(guami_list_condJSON, "guamiList");
    if (!guami_list) {
        ogs_error("OpenAPI_guami_list_cond_parseFromJSON() failed [guami_list]");
        goto end;
    }

    OpenAPI_list_t *guami_listList;
    cJSON *guami_list_local_nonprimitive;
    if (!cJSON_IsArray(guami_list)){
        ogs_error("OpenAPI_guami_list_cond_parseFromJSON() failed [guami_list]");
        goto end;
    }

    guami_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(guami_list_local_nonprimitive, guami_list ) {
        if (!cJSON_IsObject(guami_list_local_nonprimitive)) {
            ogs_error("OpenAPI_guami_list_cond_parseFromJSON() failed [guami_list]");
            goto end;
        }
        OpenAPI_guami_t *guami_listItem = OpenAPI_guami_parseFromJSON(guami_list_local_nonprimitive);

        if (!guami_listItem) {
            ogs_error("No guami_listItem");
            OpenAPI_list_free(guami_listList);
            goto end;
        }

        OpenAPI_list_add(guami_listList, guami_listItem);
    }

    guami_list_cond_local_var = OpenAPI_guami_list_cond_create (
        guami_listList
    );

    return guami_list_cond_local_var;
end:
    return NULL;
}

OpenAPI_guami_list_cond_t *OpenAPI_guami_list_cond_copy(OpenAPI_guami_list_cond_t *dst, OpenAPI_guami_list_cond_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_guami_list_cond_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_guami_list_cond_convertToJSON() failed");
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

    OpenAPI_guami_list_cond_free(dst);
    dst = OpenAPI_guami_list_cond_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

