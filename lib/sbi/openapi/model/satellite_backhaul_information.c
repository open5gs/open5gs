
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "satellite_backhaul_information.h"

OpenAPI_satellite_backhaul_information_t *OpenAPI_satellite_backhaul_information_create(
    OpenAPI_list_t *satellite_backhaul_category,
    char *g_eo_satellite_id
)
{
    OpenAPI_satellite_backhaul_information_t *satellite_backhaul_information_local_var = ogs_malloc(sizeof(OpenAPI_satellite_backhaul_information_t));
    ogs_assert(satellite_backhaul_information_local_var);

    satellite_backhaul_information_local_var->satellite_backhaul_category = satellite_backhaul_category;
    satellite_backhaul_information_local_var->g_eo_satellite_id = g_eo_satellite_id;

    return satellite_backhaul_information_local_var;
}

void OpenAPI_satellite_backhaul_information_free(OpenAPI_satellite_backhaul_information_t *satellite_backhaul_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == satellite_backhaul_information) {
        return;
    }
    if (satellite_backhaul_information->satellite_backhaul_category) {
        OpenAPI_list_free(satellite_backhaul_information->satellite_backhaul_category);
        satellite_backhaul_information->satellite_backhaul_category = NULL;
    }
    if (satellite_backhaul_information->g_eo_satellite_id) {
        ogs_free(satellite_backhaul_information->g_eo_satellite_id);
        satellite_backhaul_information->g_eo_satellite_id = NULL;
    }
    ogs_free(satellite_backhaul_information);
}

cJSON *OpenAPI_satellite_backhaul_information_convertToJSON(OpenAPI_satellite_backhaul_information_t *satellite_backhaul_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (satellite_backhaul_information == NULL) {
        ogs_error("OpenAPI_satellite_backhaul_information_convertToJSON() failed [SatelliteBackhaulInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (satellite_backhaul_information->satellite_backhaul_category != OpenAPI_satellite_backhaul_category_NULL) {
    cJSON *satellite_backhaul_categoryList = cJSON_AddArrayToObject(item, "satelliteBackhaulCategory");
    if (satellite_backhaul_categoryList == NULL) {
        ogs_error("OpenAPI_satellite_backhaul_information_convertToJSON() failed [satellite_backhaul_category]");
        goto end;
    }
    OpenAPI_list_for_each(satellite_backhaul_information->satellite_backhaul_category, node) {
        if (cJSON_AddStringToObject(satellite_backhaul_categoryList, "", OpenAPI_satellite_backhaul_category_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_satellite_backhaul_information_convertToJSON() failed [satellite_backhaul_category]");
            goto end;
        }
    }
    }

    if (satellite_backhaul_information->g_eo_satellite_id) {
    if (cJSON_AddStringToObject(item, "gEOSatelliteID", satellite_backhaul_information->g_eo_satellite_id) == NULL) {
        ogs_error("OpenAPI_satellite_backhaul_information_convertToJSON() failed [g_eo_satellite_id]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_satellite_backhaul_information_t *OpenAPI_satellite_backhaul_information_parseFromJSON(cJSON *satellite_backhaul_informationJSON)
{
    OpenAPI_satellite_backhaul_information_t *satellite_backhaul_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *satellite_backhaul_category = NULL;
    OpenAPI_list_t *satellite_backhaul_categoryList = NULL;
    cJSON *g_eo_satellite_id = NULL;
    satellite_backhaul_category = cJSON_GetObjectItemCaseSensitive(satellite_backhaul_informationJSON, "satelliteBackhaulCategory");
    if (satellite_backhaul_category) {
        cJSON *satellite_backhaul_category_local = NULL;
        if (!cJSON_IsArray(satellite_backhaul_category)) {
            ogs_error("OpenAPI_satellite_backhaul_information_parseFromJSON() failed [satellite_backhaul_category]");
            goto end;
        }

        satellite_backhaul_categoryList = OpenAPI_list_create();

        cJSON_ArrayForEach(satellite_backhaul_category_local, satellite_backhaul_category) {
            OpenAPI_satellite_backhaul_category_e localEnum = OpenAPI_satellite_backhaul_category_NULL;
            if (!cJSON_IsString(satellite_backhaul_category_local)) {
                ogs_error("OpenAPI_satellite_backhaul_information_parseFromJSON() failed [satellite_backhaul_category]");
                goto end;
            }
            localEnum = OpenAPI_satellite_backhaul_category_FromString(satellite_backhaul_category_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"satellite_backhaul_category\" is not supported. Ignoring it ...",
                         satellite_backhaul_category_local->valuestring);
            } else {
                OpenAPI_list_add(satellite_backhaul_categoryList, (void *)localEnum);
            }
        }
        if (satellite_backhaul_categoryList->count == 0) {
            ogs_error("OpenAPI_satellite_backhaul_information_parseFromJSON() failed: Expected satellite_backhaul_categoryList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    g_eo_satellite_id = cJSON_GetObjectItemCaseSensitive(satellite_backhaul_informationJSON, "gEOSatelliteID");
    if (g_eo_satellite_id) {
    if (!cJSON_IsString(g_eo_satellite_id) && !cJSON_IsNull(g_eo_satellite_id)) {
        ogs_error("OpenAPI_satellite_backhaul_information_parseFromJSON() failed [g_eo_satellite_id]");
        goto end;
    }
    }

    satellite_backhaul_information_local_var = OpenAPI_satellite_backhaul_information_create (
        satellite_backhaul_category ? satellite_backhaul_categoryList : NULL,
        g_eo_satellite_id && !cJSON_IsNull(g_eo_satellite_id) ? ogs_strdup(g_eo_satellite_id->valuestring) : NULL
    );

    return satellite_backhaul_information_local_var;
end:
    if (satellite_backhaul_categoryList) {
        OpenAPI_list_free(satellite_backhaul_categoryList);
        satellite_backhaul_categoryList = NULL;
    }
    return NULL;
}

OpenAPI_satellite_backhaul_information_t *OpenAPI_satellite_backhaul_information_copy(OpenAPI_satellite_backhaul_information_t *dst, OpenAPI_satellite_backhaul_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_satellite_backhaul_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_satellite_backhaul_information_convertToJSON() failed");
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

    OpenAPI_satellite_backhaul_information_free(dst);
    dst = OpenAPI_satellite_backhaul_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

