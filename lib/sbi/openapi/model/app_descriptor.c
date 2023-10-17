
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "app_descriptor.h"

OpenAPI_app_descriptor_t *OpenAPI_app_descriptor_create(
    char *os_id,
    char *app_id
)
{
    OpenAPI_app_descriptor_t *app_descriptor_local_var = ogs_malloc(sizeof(OpenAPI_app_descriptor_t));
    ogs_assert(app_descriptor_local_var);

    app_descriptor_local_var->os_id = os_id;
    app_descriptor_local_var->app_id = app_id;

    return app_descriptor_local_var;
}

void OpenAPI_app_descriptor_free(OpenAPI_app_descriptor_t *app_descriptor)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == app_descriptor) {
        return;
    }
    if (app_descriptor->os_id) {
        ogs_free(app_descriptor->os_id);
        app_descriptor->os_id = NULL;
    }
    if (app_descriptor->app_id) {
        ogs_free(app_descriptor->app_id);
        app_descriptor->app_id = NULL;
    }
    ogs_free(app_descriptor);
}

cJSON *OpenAPI_app_descriptor_convertToJSON(OpenAPI_app_descriptor_t *app_descriptor)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (app_descriptor == NULL) {
        ogs_error("OpenAPI_app_descriptor_convertToJSON() failed [AppDescriptor]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (app_descriptor->os_id) {
    if (cJSON_AddStringToObject(item, "osId", app_descriptor->os_id) == NULL) {
        ogs_error("OpenAPI_app_descriptor_convertToJSON() failed [os_id]");
        goto end;
    }
    }

    if (app_descriptor->app_id) {
    if (cJSON_AddStringToObject(item, "appId", app_descriptor->app_id) == NULL) {
        ogs_error("OpenAPI_app_descriptor_convertToJSON() failed [app_id]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_app_descriptor_t *OpenAPI_app_descriptor_parseFromJSON(cJSON *app_descriptorJSON)
{
    OpenAPI_app_descriptor_t *app_descriptor_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *os_id = NULL;
    cJSON *app_id = NULL;
    os_id = cJSON_GetObjectItemCaseSensitive(app_descriptorJSON, "osId");
    if (os_id) {
    if (!cJSON_IsString(os_id) && !cJSON_IsNull(os_id)) {
        ogs_error("OpenAPI_app_descriptor_parseFromJSON() failed [os_id]");
        goto end;
    }
    }

    app_id = cJSON_GetObjectItemCaseSensitive(app_descriptorJSON, "appId");
    if (app_id) {
    if (!cJSON_IsString(app_id) && !cJSON_IsNull(app_id)) {
        ogs_error("OpenAPI_app_descriptor_parseFromJSON() failed [app_id]");
        goto end;
    }
    }

    app_descriptor_local_var = OpenAPI_app_descriptor_create (
        os_id && !cJSON_IsNull(os_id) ? ogs_strdup(os_id->valuestring) : NULL,
        app_id && !cJSON_IsNull(app_id) ? ogs_strdup(app_id->valuestring) : NULL
    );

    return app_descriptor_local_var;
end:
    return NULL;
}

OpenAPI_app_descriptor_t *OpenAPI_app_descriptor_copy(OpenAPI_app_descriptor_t *dst, OpenAPI_app_descriptor_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_app_descriptor_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_app_descriptor_convertToJSON() failed");
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

    OpenAPI_app_descriptor_free(dst);
    dst = OpenAPI_app_descriptor_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

