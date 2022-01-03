
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "context_info.h"

OpenAPI_context_info_t *OpenAPI_context_info_create(
    OpenAPI_list_t *orig_headers
)
{
    OpenAPI_context_info_t *context_info_local_var = ogs_malloc(sizeof(OpenAPI_context_info_t));
    ogs_assert(context_info_local_var);

    context_info_local_var->orig_headers = orig_headers;

    return context_info_local_var;
}

void OpenAPI_context_info_free(OpenAPI_context_info_t *context_info)
{
    if (NULL == context_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(context_info->orig_headers, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(context_info->orig_headers);
    ogs_free(context_info);
}

cJSON *OpenAPI_context_info_convertToJSON(OpenAPI_context_info_t *context_info)
{
    cJSON *item = NULL;

    if (context_info == NULL) {
        ogs_error("OpenAPI_context_info_convertToJSON() failed [ContextInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (context_info->orig_headers) {
    cJSON *orig_headers = cJSON_AddArrayToObject(item, "origHeaders");
    if (orig_headers == NULL) {
        ogs_error("OpenAPI_context_info_convertToJSON() failed [orig_headers]");
        goto end;
    }

    OpenAPI_lnode_t *orig_headers_node;
    OpenAPI_list_for_each(context_info->orig_headers, orig_headers_node)  {
    if (cJSON_AddStringToObject(orig_headers, "", (char*)orig_headers_node->data) == NULL) {
        ogs_error("OpenAPI_context_info_convertToJSON() failed [orig_headers]");
        goto end;
    }
                    }
    }

end:
    return item;
}

OpenAPI_context_info_t *OpenAPI_context_info_parseFromJSON(cJSON *context_infoJSON)
{
    OpenAPI_context_info_t *context_info_local_var = NULL;
    cJSON *orig_headers = cJSON_GetObjectItemCaseSensitive(context_infoJSON, "origHeaders");

    OpenAPI_list_t *orig_headersList;
    if (orig_headers) {
    cJSON *orig_headers_local;
    if (!cJSON_IsArray(orig_headers)) {
        ogs_error("OpenAPI_context_info_parseFromJSON() failed [orig_headers]");
        goto end;
    }
    orig_headersList = OpenAPI_list_create();

    cJSON_ArrayForEach(orig_headers_local, orig_headers) {
    if (!cJSON_IsString(orig_headers_local)) {
        ogs_error("OpenAPI_context_info_parseFromJSON() failed [orig_headers]");
        goto end;
    }
    OpenAPI_list_add(orig_headersList , ogs_strdup(orig_headers_local->valuestring));
    }
    }

    context_info_local_var = OpenAPI_context_info_create (
        orig_headers ? orig_headersList : NULL
    );

    return context_info_local_var;
end:
    return NULL;
}

OpenAPI_context_info_t *OpenAPI_context_info_copy(OpenAPI_context_info_t *dst, OpenAPI_context_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_context_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_context_info_convertToJSON() failed");
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

    OpenAPI_context_info_free(dst);
    dst = OpenAPI_context_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

