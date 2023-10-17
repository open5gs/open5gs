
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "app_descriptor_1.h"

OpenAPI_app_descriptor_1_t *OpenAPI_app_descriptor_1_create(
    char *os_id,
    OpenAPI_list_t* app_ids
)
{
    OpenAPI_app_descriptor_1_t *app_descriptor_1_local_var = ogs_malloc(sizeof(OpenAPI_app_descriptor_1_t));
    ogs_assert(app_descriptor_1_local_var);

    app_descriptor_1_local_var->os_id = os_id;
    app_descriptor_1_local_var->app_ids = app_ids;

    return app_descriptor_1_local_var;
}

void OpenAPI_app_descriptor_1_free(OpenAPI_app_descriptor_1_t *app_descriptor_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == app_descriptor_1) {
        return;
    }
    if (app_descriptor_1->os_id) {
        ogs_free(app_descriptor_1->os_id);
        app_descriptor_1->os_id = NULL;
    }
    if (app_descriptor_1->app_ids) {
        OpenAPI_list_for_each(app_descriptor_1->app_ids, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(app_descriptor_1->app_ids);
        app_descriptor_1->app_ids = NULL;
    }
    ogs_free(app_descriptor_1);
}

cJSON *OpenAPI_app_descriptor_1_convertToJSON(OpenAPI_app_descriptor_1_t *app_descriptor_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (app_descriptor_1 == NULL) {
        ogs_error("OpenAPI_app_descriptor_1_convertToJSON() failed [AppDescriptor_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!app_descriptor_1->os_id) {
        ogs_error("OpenAPI_app_descriptor_1_convertToJSON() failed [os_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "osId", app_descriptor_1->os_id) == NULL) {
        ogs_error("OpenAPI_app_descriptor_1_convertToJSON() failed [os_id]");
        goto end;
    }

    if (!app_descriptor_1->app_ids) {
        ogs_error("OpenAPI_app_descriptor_1_convertToJSON() failed [app_ids]");
        return NULL;
    }
    cJSON *app_ids = cJSON_AddObjectToObject(item, "appIds");
    if (app_ids == NULL) {
        ogs_error("OpenAPI_app_descriptor_1_convertToJSON() failed [app_ids]");
        goto end;
    }
    cJSON *localMapObject = app_ids;
    if (app_descriptor_1->app_ids) {
        OpenAPI_list_for_each(app_descriptor_1->app_ids, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_app_descriptor_1_convertToJSON() failed [app_ids]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_app_descriptor_1_convertToJSON() failed [app_ids]");
                goto end;
            }
            if (cJSON_AddStringToObject(localMapObject, localKeyValue->key, (char*)localKeyValue->value) == NULL) {
                ogs_error("OpenAPI_app_descriptor_1_convertToJSON() failed [inner]");
                goto end;
            }
        }
    }

end:
    return item;
}

OpenAPI_app_descriptor_1_t *OpenAPI_app_descriptor_1_parseFromJSON(cJSON *app_descriptor_1JSON)
{
    OpenAPI_app_descriptor_1_t *app_descriptor_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *os_id = NULL;
    cJSON *app_ids = NULL;
    OpenAPI_list_t *app_idsList = NULL;
    os_id = cJSON_GetObjectItemCaseSensitive(app_descriptor_1JSON, "osId");
    if (!os_id) {
        ogs_error("OpenAPI_app_descriptor_1_parseFromJSON() failed [os_id]");
        goto end;
    }
    if (!cJSON_IsString(os_id)) {
        ogs_error("OpenAPI_app_descriptor_1_parseFromJSON() failed [os_id]");
        goto end;
    }

    app_ids = cJSON_GetObjectItemCaseSensitive(app_descriptor_1JSON, "appIds");
    if (!app_ids) {
        ogs_error("OpenAPI_app_descriptor_1_parseFromJSON() failed [app_ids]");
        goto end;
    }
        cJSON *app_ids_local_map = NULL;
        if (!cJSON_IsObject(app_ids) && !cJSON_IsNull(app_ids)) {
            ogs_error("OpenAPI_app_descriptor_1_parseFromJSON() failed [app_ids]");
            goto end;
        }
        if (cJSON_IsObject(app_ids)) {
            app_idsList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(app_ids_local_map, app_ids) {
                cJSON *localMapObject = app_ids_local_map;
                double *localDouble = NULL;
                int *localInt = NULL;
                if (!cJSON_IsString(localMapObject)) {
                    ogs_error("OpenAPI_app_descriptor_1_parseFromJSON() failed [inner]");
                    goto end;
                }
                localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), ogs_strdup(localMapObject->valuestring));
                OpenAPI_list_add(app_idsList, localMapKeyPair);
            }
        }

    app_descriptor_1_local_var = OpenAPI_app_descriptor_1_create (
        ogs_strdup(os_id->valuestring),
        app_idsList
    );

    return app_descriptor_1_local_var;
end:
    if (app_idsList) {
        OpenAPI_list_for_each(app_idsList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(app_idsList);
        app_idsList = NULL;
    }
    return NULL;
}

OpenAPI_app_descriptor_1_t *OpenAPI_app_descriptor_1_copy(OpenAPI_app_descriptor_1_t *dst, OpenAPI_app_descriptor_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_app_descriptor_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_app_descriptor_1_convertToJSON() failed");
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

    OpenAPI_app_descriptor_1_free(dst);
    dst = OpenAPI_app_descriptor_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

