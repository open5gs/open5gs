
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "app_specific_expected_ue_behaviour.h"

OpenAPI_app_specific_expected_ue_behaviour_t *OpenAPI_app_specific_expected_ue_behaviour_create(
    char *af_instance_id,
    int reference_id,
    OpenAPI_list_t* app_specific_expected_ue_behaviour_data
)
{
    OpenAPI_app_specific_expected_ue_behaviour_t *app_specific_expected_ue_behaviour_local_var = ogs_malloc(sizeof(OpenAPI_app_specific_expected_ue_behaviour_t));
    ogs_assert(app_specific_expected_ue_behaviour_local_var);

    app_specific_expected_ue_behaviour_local_var->af_instance_id = af_instance_id;
    app_specific_expected_ue_behaviour_local_var->reference_id = reference_id;
    app_specific_expected_ue_behaviour_local_var->app_specific_expected_ue_behaviour_data = app_specific_expected_ue_behaviour_data;

    return app_specific_expected_ue_behaviour_local_var;
}

void OpenAPI_app_specific_expected_ue_behaviour_free(OpenAPI_app_specific_expected_ue_behaviour_t *app_specific_expected_ue_behaviour)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == app_specific_expected_ue_behaviour) {
        return;
    }
    if (app_specific_expected_ue_behaviour->af_instance_id) {
        ogs_free(app_specific_expected_ue_behaviour->af_instance_id);
        app_specific_expected_ue_behaviour->af_instance_id = NULL;
    }
    if (app_specific_expected_ue_behaviour->app_specific_expected_ue_behaviour_data) {
        OpenAPI_list_for_each(app_specific_expected_ue_behaviour->app_specific_expected_ue_behaviour_data, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_app_specific_expected_ue_behaviour_data_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(app_specific_expected_ue_behaviour->app_specific_expected_ue_behaviour_data);
        app_specific_expected_ue_behaviour->app_specific_expected_ue_behaviour_data = NULL;
    }
    ogs_free(app_specific_expected_ue_behaviour);
}

cJSON *OpenAPI_app_specific_expected_ue_behaviour_convertToJSON(OpenAPI_app_specific_expected_ue_behaviour_t *app_specific_expected_ue_behaviour)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (app_specific_expected_ue_behaviour == NULL) {
        ogs_error("OpenAPI_app_specific_expected_ue_behaviour_convertToJSON() failed [AppSpecificExpectedUeBehaviour]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!app_specific_expected_ue_behaviour->af_instance_id) {
        ogs_error("OpenAPI_app_specific_expected_ue_behaviour_convertToJSON() failed [af_instance_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "afInstanceId", app_specific_expected_ue_behaviour->af_instance_id) == NULL) {
        ogs_error("OpenAPI_app_specific_expected_ue_behaviour_convertToJSON() failed [af_instance_id]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "referenceId", app_specific_expected_ue_behaviour->reference_id) == NULL) {
        ogs_error("OpenAPI_app_specific_expected_ue_behaviour_convertToJSON() failed [reference_id]");
        goto end;
    }

    if (!app_specific_expected_ue_behaviour->app_specific_expected_ue_behaviour_data) {
        ogs_error("OpenAPI_app_specific_expected_ue_behaviour_convertToJSON() failed [app_specific_expected_ue_behaviour_data]");
        return NULL;
    }
    cJSON *app_specific_expected_ue_behaviour_data = cJSON_AddObjectToObject(item, "appSpecificExpectedUeBehaviourData");
    if (app_specific_expected_ue_behaviour_data == NULL) {
        ogs_error("OpenAPI_app_specific_expected_ue_behaviour_convertToJSON() failed [app_specific_expected_ue_behaviour_data]");
        goto end;
    }
    cJSON *localMapObject = app_specific_expected_ue_behaviour_data;
    if (app_specific_expected_ue_behaviour->app_specific_expected_ue_behaviour_data) {
        OpenAPI_list_for_each(app_specific_expected_ue_behaviour->app_specific_expected_ue_behaviour_data, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_app_specific_expected_ue_behaviour_convertToJSON() failed [app_specific_expected_ue_behaviour_data]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_app_specific_expected_ue_behaviour_convertToJSON() failed [app_specific_expected_ue_behaviour_data]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_app_specific_expected_ue_behaviour_data_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_app_specific_expected_ue_behaviour_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }

end:
    return item;
}

OpenAPI_app_specific_expected_ue_behaviour_t *OpenAPI_app_specific_expected_ue_behaviour_parseFromJSON(cJSON *app_specific_expected_ue_behaviourJSON)
{
    OpenAPI_app_specific_expected_ue_behaviour_t *app_specific_expected_ue_behaviour_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *af_instance_id = NULL;
    cJSON *reference_id = NULL;
    cJSON *app_specific_expected_ue_behaviour_data = NULL;
    OpenAPI_list_t *app_specific_expected_ue_behaviour_dataList = NULL;
    af_instance_id = cJSON_GetObjectItemCaseSensitive(app_specific_expected_ue_behaviourJSON, "afInstanceId");
    if (!af_instance_id) {
        ogs_error("OpenAPI_app_specific_expected_ue_behaviour_parseFromJSON() failed [af_instance_id]");
        goto end;
    }
    if (!cJSON_IsString(af_instance_id)) {
        ogs_error("OpenAPI_app_specific_expected_ue_behaviour_parseFromJSON() failed [af_instance_id]");
        goto end;
    }

    reference_id = cJSON_GetObjectItemCaseSensitive(app_specific_expected_ue_behaviourJSON, "referenceId");
    if (!reference_id) {
        ogs_error("OpenAPI_app_specific_expected_ue_behaviour_parseFromJSON() failed [reference_id]");
        goto end;
    }
    if (!cJSON_IsNumber(reference_id)) {
        ogs_error("OpenAPI_app_specific_expected_ue_behaviour_parseFromJSON() failed [reference_id]");
        goto end;
    }

    app_specific_expected_ue_behaviour_data = cJSON_GetObjectItemCaseSensitive(app_specific_expected_ue_behaviourJSON, "appSpecificExpectedUeBehaviourData");
    if (!app_specific_expected_ue_behaviour_data) {
        ogs_error("OpenAPI_app_specific_expected_ue_behaviour_parseFromJSON() failed [app_specific_expected_ue_behaviour_data]");
        goto end;
    }
        cJSON *app_specific_expected_ue_behaviour_data_local_map = NULL;
        if (!cJSON_IsObject(app_specific_expected_ue_behaviour_data) && !cJSON_IsNull(app_specific_expected_ue_behaviour_data)) {
            ogs_error("OpenAPI_app_specific_expected_ue_behaviour_parseFromJSON() failed [app_specific_expected_ue_behaviour_data]");
            goto end;
        }
        if (cJSON_IsObject(app_specific_expected_ue_behaviour_data)) {
            app_specific_expected_ue_behaviour_dataList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(app_specific_expected_ue_behaviour_data_local_map, app_specific_expected_ue_behaviour_data) {
                cJSON *localMapObject = app_specific_expected_ue_behaviour_data_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_app_specific_expected_ue_behaviour_data_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_app_specific_expected_ue_behaviour_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(app_specific_expected_ue_behaviour_dataList, localMapKeyPair);
            }
        }

    app_specific_expected_ue_behaviour_local_var = OpenAPI_app_specific_expected_ue_behaviour_create (
        ogs_strdup(af_instance_id->valuestring),
        
        reference_id->valuedouble,
        app_specific_expected_ue_behaviour_dataList
    );

    return app_specific_expected_ue_behaviour_local_var;
end:
    if (app_specific_expected_ue_behaviour_dataList) {
        OpenAPI_list_for_each(app_specific_expected_ue_behaviour_dataList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_app_specific_expected_ue_behaviour_data_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(app_specific_expected_ue_behaviour_dataList);
        app_specific_expected_ue_behaviour_dataList = NULL;
    }
    return NULL;
}

OpenAPI_app_specific_expected_ue_behaviour_t *OpenAPI_app_specific_expected_ue_behaviour_copy(OpenAPI_app_specific_expected_ue_behaviour_t *dst, OpenAPI_app_specific_expected_ue_behaviour_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_app_specific_expected_ue_behaviour_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_app_specific_expected_ue_behaviour_convertToJSON() failed");
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

    OpenAPI_app_specific_expected_ue_behaviour_free(dst);
    dst = OpenAPI_app_specific_expected_ue_behaviour_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

