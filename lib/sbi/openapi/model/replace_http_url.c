
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "replace_http_url.h"

OpenAPI_replace_http_url_t *OpenAPI_replace_http_url_create(
    char *replace_http_url,
    bool is_stream_id,
    int stream_id
)
{
    OpenAPI_replace_http_url_t *replace_http_url_local_var = ogs_malloc(sizeof(OpenAPI_replace_http_url_t));
    ogs_assert(replace_http_url_local_var);

    replace_http_url_local_var->replace_http_url = replace_http_url;
    replace_http_url_local_var->is_stream_id = is_stream_id;
    replace_http_url_local_var->stream_id = stream_id;

    return replace_http_url_local_var;
}

void OpenAPI_replace_http_url_free(OpenAPI_replace_http_url_t *replace_http_url)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == replace_http_url) {
        return;
    }
    if (replace_http_url->replace_http_url) {
        ogs_free(replace_http_url->replace_http_url);
        replace_http_url->replace_http_url = NULL;
    }
    ogs_free(replace_http_url);
}

cJSON *OpenAPI_replace_http_url_convertToJSON(OpenAPI_replace_http_url_t *replace_http_url)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (replace_http_url == NULL) {
        ogs_error("OpenAPI_replace_http_url_convertToJSON() failed [ReplaceHttpUrl]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (replace_http_url->replace_http_url) {
    if (cJSON_AddStringToObject(item, "replaceHttpUrl", replace_http_url->replace_http_url) == NULL) {
        ogs_error("OpenAPI_replace_http_url_convertToJSON() failed [replace_http_url]");
        goto end;
    }
    }

    if (replace_http_url->is_stream_id) {
    if (cJSON_AddNumberToObject(item, "streamId", replace_http_url->stream_id) == NULL) {
        ogs_error("OpenAPI_replace_http_url_convertToJSON() failed [stream_id]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_replace_http_url_t *OpenAPI_replace_http_url_parseFromJSON(cJSON *replace_http_urlJSON)
{
    OpenAPI_replace_http_url_t *replace_http_url_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *replace_http_url = NULL;
    cJSON *stream_id = NULL;
    replace_http_url = cJSON_GetObjectItemCaseSensitive(replace_http_urlJSON, "replaceHttpUrl");
    if (replace_http_url) {
    if (!cJSON_IsString(replace_http_url) && !cJSON_IsNull(replace_http_url)) {
        ogs_error("OpenAPI_replace_http_url_parseFromJSON() failed [replace_http_url]");
        goto end;
    }
    }

    stream_id = cJSON_GetObjectItemCaseSensitive(replace_http_urlJSON, "streamId");
    if (stream_id) {
    if (!cJSON_IsNumber(stream_id)) {
        ogs_error("OpenAPI_replace_http_url_parseFromJSON() failed [stream_id]");
        goto end;
    }
    }

    replace_http_url_local_var = OpenAPI_replace_http_url_create (
        replace_http_url && !cJSON_IsNull(replace_http_url) ? ogs_strdup(replace_http_url->valuestring) : NULL,
        stream_id ? true : false,
        stream_id ? stream_id->valuedouble : 0
    );

    return replace_http_url_local_var;
end:
    return NULL;
}

OpenAPI_replace_http_url_t *OpenAPI_replace_http_url_copy(OpenAPI_replace_http_url_t *dst, OpenAPI_replace_http_url_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_replace_http_url_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_replace_http_url_convertToJSON() failed");
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

    OpenAPI_replace_http_url_free(dst);
    dst = OpenAPI_replace_http_url_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

