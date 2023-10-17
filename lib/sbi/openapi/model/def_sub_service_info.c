
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "def_sub_service_info.h"

OpenAPI_def_sub_service_info_t *OpenAPI_def_sub_service_info_create(
    OpenAPI_list_t *versions,
    char *supported_features
)
{
    OpenAPI_def_sub_service_info_t *def_sub_service_info_local_var = ogs_malloc(sizeof(OpenAPI_def_sub_service_info_t));
    ogs_assert(def_sub_service_info_local_var);

    def_sub_service_info_local_var->versions = versions;
    def_sub_service_info_local_var->supported_features = supported_features;

    return def_sub_service_info_local_var;
}

void OpenAPI_def_sub_service_info_free(OpenAPI_def_sub_service_info_t *def_sub_service_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == def_sub_service_info) {
        return;
    }
    if (def_sub_service_info->versions) {
        OpenAPI_list_for_each(def_sub_service_info->versions, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(def_sub_service_info->versions);
        def_sub_service_info->versions = NULL;
    }
    if (def_sub_service_info->supported_features) {
        ogs_free(def_sub_service_info->supported_features);
        def_sub_service_info->supported_features = NULL;
    }
    ogs_free(def_sub_service_info);
}

cJSON *OpenAPI_def_sub_service_info_convertToJSON(OpenAPI_def_sub_service_info_t *def_sub_service_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (def_sub_service_info == NULL) {
        ogs_error("OpenAPI_def_sub_service_info_convertToJSON() failed [DefSubServiceInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (def_sub_service_info->versions) {
    cJSON *versionsList = cJSON_AddArrayToObject(item, "versions");
    if (versionsList == NULL) {
        ogs_error("OpenAPI_def_sub_service_info_convertToJSON() failed [versions]");
        goto end;
    }
    OpenAPI_list_for_each(def_sub_service_info->versions, node) {
        if (cJSON_AddStringToObject(versionsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_def_sub_service_info_convertToJSON() failed [versions]");
            goto end;
        }
    }
    }

    if (def_sub_service_info->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", def_sub_service_info->supported_features) == NULL) {
        ogs_error("OpenAPI_def_sub_service_info_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_def_sub_service_info_t *OpenAPI_def_sub_service_info_parseFromJSON(cJSON *def_sub_service_infoJSON)
{
    OpenAPI_def_sub_service_info_t *def_sub_service_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *versions = NULL;
    OpenAPI_list_t *versionsList = NULL;
    cJSON *supported_features = NULL;
    versions = cJSON_GetObjectItemCaseSensitive(def_sub_service_infoJSON, "versions");
    if (versions) {
        cJSON *versions_local = NULL;
        if (!cJSON_IsArray(versions)) {
            ogs_error("OpenAPI_def_sub_service_info_parseFromJSON() failed [versions]");
            goto end;
        }

        versionsList = OpenAPI_list_create();

        cJSON_ArrayForEach(versions_local, versions) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(versions_local)) {
                ogs_error("OpenAPI_def_sub_service_info_parseFromJSON() failed [versions]");
                goto end;
            }
            OpenAPI_list_add(versionsList, ogs_strdup(versions_local->valuestring));
        }
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(def_sub_service_infoJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_def_sub_service_info_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    def_sub_service_info_local_var = OpenAPI_def_sub_service_info_create (
        versions ? versionsList : NULL,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL
    );

    return def_sub_service_info_local_var;
end:
    if (versionsList) {
        OpenAPI_list_for_each(versionsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(versionsList);
        versionsList = NULL;
    }
    return NULL;
}

OpenAPI_def_sub_service_info_t *OpenAPI_def_sub_service_info_copy(OpenAPI_def_sub_service_info_t *dst, OpenAPI_def_sub_service_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_def_sub_service_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_def_sub_service_info_convertToJSON() failed");
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

    OpenAPI_def_sub_service_info_free(dst);
    dst = OpenAPI_def_sub_service_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

