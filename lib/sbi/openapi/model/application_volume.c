
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "application_volume.h"

OpenAPI_application_volume_t *OpenAPI_application_volume_create(
    char *app_id,
    long app_volume
)
{
    OpenAPI_application_volume_t *application_volume_local_var = ogs_malloc(sizeof(OpenAPI_application_volume_t));
    ogs_assert(application_volume_local_var);

    application_volume_local_var->app_id = app_id;
    application_volume_local_var->app_volume = app_volume;

    return application_volume_local_var;
}

void OpenAPI_application_volume_free(OpenAPI_application_volume_t *application_volume)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == application_volume) {
        return;
    }
    if (application_volume->app_id) {
        ogs_free(application_volume->app_id);
        application_volume->app_id = NULL;
    }
    ogs_free(application_volume);
}

cJSON *OpenAPI_application_volume_convertToJSON(OpenAPI_application_volume_t *application_volume)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (application_volume == NULL) {
        ogs_error("OpenAPI_application_volume_convertToJSON() failed [ApplicationVolume]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!application_volume->app_id) {
        ogs_error("OpenAPI_application_volume_convertToJSON() failed [app_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "appId", application_volume->app_id) == NULL) {
        ogs_error("OpenAPI_application_volume_convertToJSON() failed [app_id]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "appVolume", application_volume->app_volume) == NULL) {
        ogs_error("OpenAPI_application_volume_convertToJSON() failed [app_volume]");
        goto end;
    }

end:
    return item;
}

OpenAPI_application_volume_t *OpenAPI_application_volume_parseFromJSON(cJSON *application_volumeJSON)
{
    OpenAPI_application_volume_t *application_volume_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *app_id = NULL;
    cJSON *app_volume = NULL;
    app_id = cJSON_GetObjectItemCaseSensitive(application_volumeJSON, "appId");
    if (!app_id) {
        ogs_error("OpenAPI_application_volume_parseFromJSON() failed [app_id]");
        goto end;
    }
    if (!cJSON_IsString(app_id)) {
        ogs_error("OpenAPI_application_volume_parseFromJSON() failed [app_id]");
        goto end;
    }

    app_volume = cJSON_GetObjectItemCaseSensitive(application_volumeJSON, "appVolume");
    if (!app_volume) {
        ogs_error("OpenAPI_application_volume_parseFromJSON() failed [app_volume]");
        goto end;
    }
    if (!cJSON_IsNumber(app_volume)) {
        ogs_error("OpenAPI_application_volume_parseFromJSON() failed [app_volume]");
        goto end;
    }

    application_volume_local_var = OpenAPI_application_volume_create (
        ogs_strdup(app_id->valuestring),
        
        app_volume->valuedouble
    );

    return application_volume_local_var;
end:
    return NULL;
}

OpenAPI_application_volume_t *OpenAPI_application_volume_copy(OpenAPI_application_volume_t *dst, OpenAPI_application_volume_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_application_volume_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_application_volume_convertToJSON() failed");
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

    OpenAPI_application_volume_free(dst);
    dst = OpenAPI_application_volume_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

