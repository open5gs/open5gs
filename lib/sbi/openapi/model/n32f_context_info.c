
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "n32f_context_info.h"

OpenAPI_n32f_context_info_t *OpenAPI_n32f_context_info_create(
    char *n32f_context_id
)
{
    OpenAPI_n32f_context_info_t *n32f_context_info_local_var = ogs_malloc(sizeof(OpenAPI_n32f_context_info_t));
    ogs_assert(n32f_context_info_local_var);

    n32f_context_info_local_var->n32f_context_id = n32f_context_id;

    return n32f_context_info_local_var;
}

void OpenAPI_n32f_context_info_free(OpenAPI_n32f_context_info_t *n32f_context_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == n32f_context_info) {
        return;
    }
    if (n32f_context_info->n32f_context_id) {
        ogs_free(n32f_context_info->n32f_context_id);
        n32f_context_info->n32f_context_id = NULL;
    }
    ogs_free(n32f_context_info);
}

cJSON *OpenAPI_n32f_context_info_convertToJSON(OpenAPI_n32f_context_info_t *n32f_context_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (n32f_context_info == NULL) {
        ogs_error("OpenAPI_n32f_context_info_convertToJSON() failed [N32fContextInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!n32f_context_info->n32f_context_id) {
        ogs_error("OpenAPI_n32f_context_info_convertToJSON() failed [n32f_context_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "n32fContextId", n32f_context_info->n32f_context_id) == NULL) {
        ogs_error("OpenAPI_n32f_context_info_convertToJSON() failed [n32f_context_id]");
        goto end;
    }

end:
    return item;
}

OpenAPI_n32f_context_info_t *OpenAPI_n32f_context_info_parseFromJSON(cJSON *n32f_context_infoJSON)
{
    OpenAPI_n32f_context_info_t *n32f_context_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *n32f_context_id = NULL;
    n32f_context_id = cJSON_GetObjectItemCaseSensitive(n32f_context_infoJSON, "n32fContextId");
    if (!n32f_context_id) {
        ogs_error("OpenAPI_n32f_context_info_parseFromJSON() failed [n32f_context_id]");
        goto end;
    }
    if (!cJSON_IsString(n32f_context_id)) {
        ogs_error("OpenAPI_n32f_context_info_parseFromJSON() failed [n32f_context_id]");
        goto end;
    }

    n32f_context_info_local_var = OpenAPI_n32f_context_info_create (
        ogs_strdup(n32f_context_id->valuestring)
    );

    return n32f_context_info_local_var;
end:
    return NULL;
}

OpenAPI_n32f_context_info_t *OpenAPI_n32f_context_info_copy(OpenAPI_n32f_context_info_t *dst, OpenAPI_n32f_context_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_n32f_context_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_n32f_context_info_convertToJSON() failed");
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

    OpenAPI_n32f_context_info_free(dst);
    dst = OpenAPI_n32f_context_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

