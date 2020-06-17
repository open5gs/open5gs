
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "location_info.h"

OpenAPI_location_info_t *OpenAPI_location_info_create(
    char *supi,
    char *gpsi,
    OpenAPI_list_t *registration_location_info_list,
    char *supported_features
    )
{
    OpenAPI_location_info_t *location_info_local_var = OpenAPI_malloc(sizeof(OpenAPI_location_info_t));
    if (!location_info_local_var) {
        return NULL;
    }
    location_info_local_var->supi = supi;
    location_info_local_var->gpsi = gpsi;
    location_info_local_var->registration_location_info_list = registration_location_info_list;
    location_info_local_var->supported_features = supported_features;

    return location_info_local_var;
}

void OpenAPI_location_info_free(OpenAPI_location_info_t *location_info)
{
    if (NULL == location_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(location_info->supi);
    ogs_free(location_info->gpsi);
    OpenAPI_list_for_each(location_info->registration_location_info_list, node) {
        OpenAPI_object_free(node->data);
    }
    OpenAPI_list_free(location_info->registration_location_info_list);
    ogs_free(location_info->supported_features);
    ogs_free(location_info);
}

cJSON *OpenAPI_location_info_convertToJSON(OpenAPI_location_info_t *location_info)
{
    cJSON *item = NULL;

    if (location_info == NULL) {
        ogs_error("OpenAPI_location_info_convertToJSON() failed [LocationInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (location_info->supi) {
        if (cJSON_AddStringToObject(item, "supi", location_info->supi) == NULL) {
            ogs_error("OpenAPI_location_info_convertToJSON() failed [supi]");
            goto end;
        }
    }

    if (location_info->gpsi) {
        if (cJSON_AddStringToObject(item, "gpsi", location_info->gpsi) == NULL) {
            ogs_error("OpenAPI_location_info_convertToJSON() failed [gpsi]");
            goto end;
        }
    }

    if (!location_info->registration_location_info_list) {
        ogs_error("OpenAPI_location_info_convertToJSON() failed [registration_location_info_list]");
        goto end;
    }
    cJSON *registration_location_info_listList = cJSON_AddArrayToObject(item, "registrationLocationInfoList");
    if (registration_location_info_listList == NULL) {
        ogs_error("OpenAPI_location_info_convertToJSON() failed [registration_location_info_list]");
        goto end;
    }

    OpenAPI_lnode_t *registration_location_info_list_node;
    if (location_info->registration_location_info_list) {
        OpenAPI_list_for_each(location_info->registration_location_info_list, registration_location_info_list_node) {
            cJSON *itemLocal = OpenAPI_object_convertToJSON(registration_location_info_list_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_location_info_convertToJSON() failed [registration_location_info_list]");
                goto end;
            }
            cJSON_AddItemToArray(registration_location_info_listList, itemLocal);
        }
    }

    if (location_info->supported_features) {
        if (cJSON_AddStringToObject(item, "supportedFeatures", location_info->supported_features) == NULL) {
            ogs_error("OpenAPI_location_info_convertToJSON() failed [supported_features]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_location_info_t *OpenAPI_location_info_parseFromJSON(cJSON *location_infoJSON)
{
    OpenAPI_location_info_t *location_info_local_var = NULL;
    cJSON *supi = cJSON_GetObjectItemCaseSensitive(location_infoJSON, "supi");

    if (supi) {
        if (!cJSON_IsString(supi)) {
            ogs_error("OpenAPI_location_info_parseFromJSON() failed [supi]");
            goto end;
        }
    }

    cJSON *gpsi = cJSON_GetObjectItemCaseSensitive(location_infoJSON, "gpsi");

    if (gpsi) {
        if (!cJSON_IsString(gpsi)) {
            ogs_error("OpenAPI_location_info_parseFromJSON() failed [gpsi]");
            goto end;
        }
    }

    cJSON *registration_location_info_list = cJSON_GetObjectItemCaseSensitive(location_infoJSON, "registrationLocationInfoList");
    if (!registration_location_info_list) {
        ogs_error("OpenAPI_location_info_parseFromJSON() failed [registration_location_info_list]");
        goto end;
    }

    OpenAPI_list_t *registration_location_info_listList;

    cJSON *registration_location_info_list_local_nonprimitive;
    if (!cJSON_IsArray(registration_location_info_list)) {
        ogs_error("OpenAPI_location_info_parseFromJSON() failed [registration_location_info_list]");
        goto end;
    }

    registration_location_info_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(registration_location_info_list_local_nonprimitive, registration_location_info_list ) {
        if (!cJSON_IsObject(registration_location_info_list_local_nonprimitive)) {
            ogs_error("OpenAPI_location_info_parseFromJSON() failed [registration_location_info_list]");
            goto end;
        }
        OpenAPI_object_t *registration_location_info_listItem = OpenAPI_object_parseFromJSON(registration_location_info_list_local_nonprimitive);

        OpenAPI_list_add(registration_location_info_listList, registration_location_info_listItem);
    }

    cJSON *supported_features = cJSON_GetObjectItemCaseSensitive(location_infoJSON, "supportedFeatures");

    if (supported_features) {
        if (!cJSON_IsString(supported_features)) {
            ogs_error("OpenAPI_location_info_parseFromJSON() failed [supported_features]");
            goto end;
        }
    }

    location_info_local_var = OpenAPI_location_info_create (
        supi ? ogs_strdup(supi->valuestring) : NULL,
        gpsi ? ogs_strdup(gpsi->valuestring) : NULL,
        registration_location_info_listList,
        supported_features ? ogs_strdup(supported_features->valuestring) : NULL
        );

    return location_info_local_var;
end:
    return NULL;
}

OpenAPI_location_info_t *OpenAPI_location_info_copy(OpenAPI_location_info_t *dst, OpenAPI_location_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_location_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_location_info_convertToJSON() failed");
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

    OpenAPI_location_info_free(dst);
    dst = OpenAPI_location_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

