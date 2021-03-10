
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nwdaf_cond.h"

OpenAPI_nwdaf_cond_t *OpenAPI_nwdaf_cond_create(
    OpenAPI_list_t *analytics_ids
    )
{
    OpenAPI_nwdaf_cond_t *nwdaf_cond_local_var = OpenAPI_malloc(sizeof(OpenAPI_nwdaf_cond_t));
    if (!nwdaf_cond_local_var) {
        return NULL;
    }
    nwdaf_cond_local_var->analytics_ids = analytics_ids;

    return nwdaf_cond_local_var;
}

void OpenAPI_nwdaf_cond_free(OpenAPI_nwdaf_cond_t *nwdaf_cond)
{
    if (NULL == nwdaf_cond) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(nwdaf_cond->analytics_ids, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(nwdaf_cond->analytics_ids);
    ogs_free(nwdaf_cond);
}

cJSON *OpenAPI_nwdaf_cond_convertToJSON(OpenAPI_nwdaf_cond_t *nwdaf_cond)
{
    cJSON *item = NULL;

    if (nwdaf_cond == NULL) {
        ogs_error("OpenAPI_nwdaf_cond_convertToJSON() failed [NwdafCond]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (nwdaf_cond->analytics_ids) {
        cJSON *analytics_ids = cJSON_AddArrayToObject(item, "analyticsIds");
        if (analytics_ids == NULL) {
            ogs_error("OpenAPI_nwdaf_cond_convertToJSON() failed [analytics_ids]");
            goto end;
        }

        OpenAPI_lnode_t *analytics_ids_node;
        OpenAPI_list_for_each(nwdaf_cond->analytics_ids, analytics_ids_node)  {
            if (cJSON_AddStringToObject(analytics_ids, "", (char*)analytics_ids_node->data) == NULL) {
                ogs_error("OpenAPI_nwdaf_cond_convertToJSON() failed [analytics_ids]");
                goto end;
            }
        }
    }

end:
    return item;
}

OpenAPI_nwdaf_cond_t *OpenAPI_nwdaf_cond_parseFromJSON(cJSON *nwdaf_condJSON)
{
    OpenAPI_nwdaf_cond_t *nwdaf_cond_local_var = NULL;
    cJSON *analytics_ids = cJSON_GetObjectItemCaseSensitive(nwdaf_condJSON, "analyticsIds");

    OpenAPI_list_t *analytics_idsList;
    if (analytics_ids) {
        cJSON *analytics_ids_local;
        if (!cJSON_IsArray(analytics_ids)) {
            ogs_error("OpenAPI_nwdaf_cond_parseFromJSON() failed [analytics_ids]");
            goto end;
        }
        analytics_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(analytics_ids_local, analytics_ids) {
            if (!cJSON_IsString(analytics_ids_local)) {
                ogs_error("OpenAPI_nwdaf_cond_parseFromJSON() failed [analytics_ids]");
                goto end;
            }
            OpenAPI_list_add(analytics_idsList, ogs_strdup(analytics_ids_local->valuestring));
        }
    }

    nwdaf_cond_local_var = OpenAPI_nwdaf_cond_create (
        analytics_ids ? analytics_idsList : NULL
        );

    return nwdaf_cond_local_var;
end:
    return NULL;
}

OpenAPI_nwdaf_cond_t *OpenAPI_nwdaf_cond_copy(OpenAPI_nwdaf_cond_t *dst, OpenAPI_nwdaf_cond_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nwdaf_cond_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nwdaf_cond_convertToJSON() failed");
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

    OpenAPI_nwdaf_cond_free(dst);
    dst = OpenAPI_nwdaf_cond_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

