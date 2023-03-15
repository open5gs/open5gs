
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "app_detection_info.h"

OpenAPI_app_detection_info_t *OpenAPI_app_detection_info_create(
    char *app_id,
    char *instance_id,
    OpenAPI_list_t *sdf_descriptions
)
{
    OpenAPI_app_detection_info_t *app_detection_info_local_var = ogs_malloc(sizeof(OpenAPI_app_detection_info_t));
    ogs_assert(app_detection_info_local_var);

    app_detection_info_local_var->app_id = app_id;
    app_detection_info_local_var->instance_id = instance_id;
    app_detection_info_local_var->sdf_descriptions = sdf_descriptions;

    return app_detection_info_local_var;
}

void OpenAPI_app_detection_info_free(OpenAPI_app_detection_info_t *app_detection_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == app_detection_info) {
        return;
    }
    if (app_detection_info->app_id) {
        ogs_free(app_detection_info->app_id);
        app_detection_info->app_id = NULL;
    }
    if (app_detection_info->instance_id) {
        ogs_free(app_detection_info->instance_id);
        app_detection_info->instance_id = NULL;
    }
    if (app_detection_info->sdf_descriptions) {
        OpenAPI_list_for_each(app_detection_info->sdf_descriptions, node) {
            OpenAPI_flow_information_free(node->data);
        }
        OpenAPI_list_free(app_detection_info->sdf_descriptions);
        app_detection_info->sdf_descriptions = NULL;
    }
    ogs_free(app_detection_info);
}

cJSON *OpenAPI_app_detection_info_convertToJSON(OpenAPI_app_detection_info_t *app_detection_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (app_detection_info == NULL) {
        ogs_error("OpenAPI_app_detection_info_convertToJSON() failed [AppDetectionInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!app_detection_info->app_id) {
        ogs_error("OpenAPI_app_detection_info_convertToJSON() failed [app_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "appId", app_detection_info->app_id) == NULL) {
        ogs_error("OpenAPI_app_detection_info_convertToJSON() failed [app_id]");
        goto end;
    }

    if (app_detection_info->instance_id) {
    if (cJSON_AddStringToObject(item, "instanceId", app_detection_info->instance_id) == NULL) {
        ogs_error("OpenAPI_app_detection_info_convertToJSON() failed [instance_id]");
        goto end;
    }
    }

    if (app_detection_info->sdf_descriptions) {
    cJSON *sdf_descriptionsList = cJSON_AddArrayToObject(item, "sdfDescriptions");
    if (sdf_descriptionsList == NULL) {
        ogs_error("OpenAPI_app_detection_info_convertToJSON() failed [sdf_descriptions]");
        goto end;
    }
    OpenAPI_list_for_each(app_detection_info->sdf_descriptions, node) {
        cJSON *itemLocal = OpenAPI_flow_information_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_app_detection_info_convertToJSON() failed [sdf_descriptions]");
            goto end;
        }
        cJSON_AddItemToArray(sdf_descriptionsList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_app_detection_info_t *OpenAPI_app_detection_info_parseFromJSON(cJSON *app_detection_infoJSON)
{
    OpenAPI_app_detection_info_t *app_detection_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *app_id = NULL;
    cJSON *instance_id = NULL;
    cJSON *sdf_descriptions = NULL;
    OpenAPI_list_t *sdf_descriptionsList = NULL;
    app_id = cJSON_GetObjectItemCaseSensitive(app_detection_infoJSON, "appId");
    if (!app_id) {
        ogs_error("OpenAPI_app_detection_info_parseFromJSON() failed [app_id]");
        goto end;
    }
    if (!cJSON_IsString(app_id)) {
        ogs_error("OpenAPI_app_detection_info_parseFromJSON() failed [app_id]");
        goto end;
    }

    instance_id = cJSON_GetObjectItemCaseSensitive(app_detection_infoJSON, "instanceId");
    if (instance_id) {
    if (!cJSON_IsString(instance_id) && !cJSON_IsNull(instance_id)) {
        ogs_error("OpenAPI_app_detection_info_parseFromJSON() failed [instance_id]");
        goto end;
    }
    }

    sdf_descriptions = cJSON_GetObjectItemCaseSensitive(app_detection_infoJSON, "sdfDescriptions");
    if (sdf_descriptions) {
        cJSON *sdf_descriptions_local = NULL;
        if (!cJSON_IsArray(sdf_descriptions)) {
            ogs_error("OpenAPI_app_detection_info_parseFromJSON() failed [sdf_descriptions]");
            goto end;
        }

        sdf_descriptionsList = OpenAPI_list_create();

        cJSON_ArrayForEach(sdf_descriptions_local, sdf_descriptions) {
            if (!cJSON_IsObject(sdf_descriptions_local)) {
                ogs_error("OpenAPI_app_detection_info_parseFromJSON() failed [sdf_descriptions]");
                goto end;
            }
            OpenAPI_flow_information_t *sdf_descriptionsItem = OpenAPI_flow_information_parseFromJSON(sdf_descriptions_local);
            if (!sdf_descriptionsItem) {
                ogs_error("No sdf_descriptionsItem");
                goto end;
            }
            OpenAPI_list_add(sdf_descriptionsList, sdf_descriptionsItem);
        }
    }

    app_detection_info_local_var = OpenAPI_app_detection_info_create (
        ogs_strdup(app_id->valuestring),
        instance_id && !cJSON_IsNull(instance_id) ? ogs_strdup(instance_id->valuestring) : NULL,
        sdf_descriptions ? sdf_descriptionsList : NULL
    );

    return app_detection_info_local_var;
end:
    if (sdf_descriptionsList) {
        OpenAPI_list_for_each(sdf_descriptionsList, node) {
            OpenAPI_flow_information_free(node->data);
        }
        OpenAPI_list_free(sdf_descriptionsList);
        sdf_descriptionsList = NULL;
    }
    return NULL;
}

OpenAPI_app_detection_info_t *OpenAPI_app_detection_info_copy(OpenAPI_app_detection_info_t *dst, OpenAPI_app_detection_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_app_detection_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_app_detection_info_convertToJSON() failed");
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

    OpenAPI_app_detection_info_free(dst);
    dst = OpenAPI_app_detection_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

