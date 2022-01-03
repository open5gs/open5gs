
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ref_to_binary_data.h"

OpenAPI_ref_to_binary_data_t *OpenAPI_ref_to_binary_data_create(
    char *content_id
)
{
    OpenAPI_ref_to_binary_data_t *ref_to_binary_data_local_var = ogs_malloc(sizeof(OpenAPI_ref_to_binary_data_t));
    ogs_assert(ref_to_binary_data_local_var);

    ref_to_binary_data_local_var->content_id = content_id;

    return ref_to_binary_data_local_var;
}

void OpenAPI_ref_to_binary_data_free(OpenAPI_ref_to_binary_data_t *ref_to_binary_data)
{
    if (NULL == ref_to_binary_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(ref_to_binary_data->content_id);
    ogs_free(ref_to_binary_data);
}

cJSON *OpenAPI_ref_to_binary_data_convertToJSON(OpenAPI_ref_to_binary_data_t *ref_to_binary_data)
{
    cJSON *item = NULL;

    if (ref_to_binary_data == NULL) {
        ogs_error("OpenAPI_ref_to_binary_data_convertToJSON() failed [RefToBinaryData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddStringToObject(item, "contentId", ref_to_binary_data->content_id) == NULL) {
        ogs_error("OpenAPI_ref_to_binary_data_convertToJSON() failed [content_id]");
        goto end;
    }

end:
    return item;
}

OpenAPI_ref_to_binary_data_t *OpenAPI_ref_to_binary_data_parseFromJSON(cJSON *ref_to_binary_dataJSON)
{
    OpenAPI_ref_to_binary_data_t *ref_to_binary_data_local_var = NULL;
    cJSON *content_id = cJSON_GetObjectItemCaseSensitive(ref_to_binary_dataJSON, "contentId");
    if (!content_id) {
        ogs_error("OpenAPI_ref_to_binary_data_parseFromJSON() failed [content_id]");
        goto end;
    }

    if (!cJSON_IsString(content_id)) {
        ogs_error("OpenAPI_ref_to_binary_data_parseFromJSON() failed [content_id]");
        goto end;
    }

    ref_to_binary_data_local_var = OpenAPI_ref_to_binary_data_create (
        ogs_strdup(content_id->valuestring)
    );

    return ref_to_binary_data_local_var;
end:
    return NULL;
}

OpenAPI_ref_to_binary_data_t *OpenAPI_ref_to_binary_data_copy(OpenAPI_ref_to_binary_data_t *dst, OpenAPI_ref_to_binary_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ref_to_binary_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ref_to_binary_data_convertToJSON() failed");
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

    OpenAPI_ref_to_binary_data_free(dst);
    dst = OpenAPI_ref_to_binary_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

