
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
    OpenAPI_location_info_t *location_info_local_var = ogs_malloc(sizeof(OpenAPI_location_info_t));
    ogs_assert(location_info_local_var);

    location_info_local_var->supi = supi;
    location_info_local_var->gpsi = gpsi;
    location_info_local_var->registration_location_info_list = registration_location_info_list;
    location_info_local_var->supported_features = supported_features;

    return location_info_local_var;
}

void OpenAPI_location_info_free(OpenAPI_location_info_t *location_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == location_info) {
        return;
    }
    if (location_info->supi) {
        ogs_free(location_info->supi);
        location_info->supi = NULL;
    }
    if (location_info->gpsi) {
        ogs_free(location_info->gpsi);
        location_info->gpsi = NULL;
    }
    if (location_info->registration_location_info_list) {
        OpenAPI_list_for_each(location_info->registration_location_info_list, node) {
            OpenAPI_registration_location_info_free(node->data);
        }
        OpenAPI_list_free(location_info->registration_location_info_list);
        location_info->registration_location_info_list = NULL;
    }
    if (location_info->supported_features) {
        ogs_free(location_info->supported_features);
        location_info->supported_features = NULL;
    }
    ogs_free(location_info);
}

cJSON *OpenAPI_location_info_convertToJSON(OpenAPI_location_info_t *location_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

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
        return NULL;
    }
    cJSON *registration_location_info_listList = cJSON_AddArrayToObject(item, "registrationLocationInfoList");
    if (registration_location_info_listList == NULL) {
        ogs_error("OpenAPI_location_info_convertToJSON() failed [registration_location_info_list]");
        goto end;
    }
    OpenAPI_list_for_each(location_info->registration_location_info_list, node) {
        cJSON *itemLocal = OpenAPI_registration_location_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_location_info_convertToJSON() failed [registration_location_info_list]");
            goto end;
        }
        cJSON_AddItemToArray(registration_location_info_listList, itemLocal);
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
    OpenAPI_lnode_t *node = NULL;
    cJSON *supi = NULL;
    cJSON *gpsi = NULL;
    cJSON *registration_location_info_list = NULL;
    OpenAPI_list_t *registration_location_info_listList = NULL;
    cJSON *supported_features = NULL;
    supi = cJSON_GetObjectItemCaseSensitive(location_infoJSON, "supi");
    if (supi) {
    if (!cJSON_IsString(supi) && !cJSON_IsNull(supi)) {
        ogs_error("OpenAPI_location_info_parseFromJSON() failed [supi]");
        goto end;
    }
    }

    gpsi = cJSON_GetObjectItemCaseSensitive(location_infoJSON, "gpsi");
    if (gpsi) {
    if (!cJSON_IsString(gpsi) && !cJSON_IsNull(gpsi)) {
        ogs_error("OpenAPI_location_info_parseFromJSON() failed [gpsi]");
        goto end;
    }
    }

    registration_location_info_list = cJSON_GetObjectItemCaseSensitive(location_infoJSON, "registrationLocationInfoList");
    if (!registration_location_info_list) {
        ogs_error("OpenAPI_location_info_parseFromJSON() failed [registration_location_info_list]");
        goto end;
    }
        cJSON *registration_location_info_list_local = NULL;
        if (!cJSON_IsArray(registration_location_info_list)) {
            ogs_error("OpenAPI_location_info_parseFromJSON() failed [registration_location_info_list]");
            goto end;
        }

        registration_location_info_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(registration_location_info_list_local, registration_location_info_list) {
            if (!cJSON_IsObject(registration_location_info_list_local)) {
                ogs_error("OpenAPI_location_info_parseFromJSON() failed [registration_location_info_list]");
                goto end;
            }
            OpenAPI_registration_location_info_t *registration_location_info_listItem = OpenAPI_registration_location_info_parseFromJSON(registration_location_info_list_local);
            if (!registration_location_info_listItem) {
                ogs_error("No registration_location_info_listItem");
                OpenAPI_list_free(registration_location_info_listList);
                goto end;
            }
            OpenAPI_list_add(registration_location_info_listList, registration_location_info_listItem);
        }

    supported_features = cJSON_GetObjectItemCaseSensitive(location_infoJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_location_info_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    location_info_local_var = OpenAPI_location_info_create (
        supi && !cJSON_IsNull(supi) ? ogs_strdup(supi->valuestring) : NULL,
        gpsi && !cJSON_IsNull(gpsi) ? ogs_strdup(gpsi->valuestring) : NULL,
        registration_location_info_listList,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL
    );

    return location_info_local_var;
end:
    if (registration_location_info_listList) {
        OpenAPI_list_for_each(registration_location_info_listList, node) {
            OpenAPI_registration_location_info_free(node->data);
        }
        OpenAPI_list_free(registration_location_info_listList);
        registration_location_info_listList = NULL;
    }
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

