
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "geographical_area.h"

OpenAPI_geographical_area_t *OpenAPI_geographical_area_create(
    OpenAPI_civic_address_t *civic_address,
    OpenAPI_geographic_area_t *shapes
)
{
    OpenAPI_geographical_area_t *geographical_area_local_var = ogs_malloc(sizeof(OpenAPI_geographical_area_t));
    ogs_assert(geographical_area_local_var);

    geographical_area_local_var->civic_address = civic_address;
    geographical_area_local_var->shapes = shapes;

    return geographical_area_local_var;
}

void OpenAPI_geographical_area_free(OpenAPI_geographical_area_t *geographical_area)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == geographical_area) {
        return;
    }
    if (geographical_area->civic_address) {
        OpenAPI_civic_address_free(geographical_area->civic_address);
        geographical_area->civic_address = NULL;
    }
    if (geographical_area->shapes) {
        OpenAPI_geographic_area_free(geographical_area->shapes);
        geographical_area->shapes = NULL;
    }
    ogs_free(geographical_area);
}

cJSON *OpenAPI_geographical_area_convertToJSON(OpenAPI_geographical_area_t *geographical_area)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (geographical_area == NULL) {
        ogs_error("OpenAPI_geographical_area_convertToJSON() failed [GeographicalArea]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (geographical_area->civic_address) {
    cJSON *civic_address_local_JSON = OpenAPI_civic_address_convertToJSON(geographical_area->civic_address);
    if (civic_address_local_JSON == NULL) {
        ogs_error("OpenAPI_geographical_area_convertToJSON() failed [civic_address]");
        goto end;
    }
    cJSON_AddItemToObject(item, "civicAddress", civic_address_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_geographical_area_convertToJSON() failed [civic_address]");
        goto end;
    }
    }

    if (geographical_area->shapes) {
    cJSON *shapes_local_JSON = OpenAPI_geographic_area_convertToJSON(geographical_area->shapes);
    if (shapes_local_JSON == NULL) {
        ogs_error("OpenAPI_geographical_area_convertToJSON() failed [shapes]");
        goto end;
    }
    cJSON_AddItemToObject(item, "shapes", shapes_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_geographical_area_convertToJSON() failed [shapes]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_geographical_area_t *OpenAPI_geographical_area_parseFromJSON(cJSON *geographical_areaJSON)
{
    OpenAPI_geographical_area_t *geographical_area_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *civic_address = NULL;
    OpenAPI_civic_address_t *civic_address_local_nonprim = NULL;
    cJSON *shapes = NULL;
    OpenAPI_geographic_area_t *shapes_local_nonprim = NULL;
    civic_address = cJSON_GetObjectItemCaseSensitive(geographical_areaJSON, "civicAddress");
    if (civic_address) {
    civic_address_local_nonprim = OpenAPI_civic_address_parseFromJSON(civic_address);
    if (!civic_address_local_nonprim) {
        ogs_error("OpenAPI_civic_address_parseFromJSON failed [civic_address]");
        goto end;
    }
    }

    shapes = cJSON_GetObjectItemCaseSensitive(geographical_areaJSON, "shapes");
    if (shapes) {
    shapes_local_nonprim = OpenAPI_geographic_area_parseFromJSON(shapes);
    if (!shapes_local_nonprim) {
        ogs_error("OpenAPI_geographic_area_parseFromJSON failed [shapes]");
        goto end;
    }
    }

    geographical_area_local_var = OpenAPI_geographical_area_create (
        civic_address ? civic_address_local_nonprim : NULL,
        shapes ? shapes_local_nonprim : NULL
    );

    return geographical_area_local_var;
end:
    if (civic_address_local_nonprim) {
        OpenAPI_civic_address_free(civic_address_local_nonprim);
        civic_address_local_nonprim = NULL;
    }
    if (shapes_local_nonprim) {
        OpenAPI_geographic_area_free(shapes_local_nonprim);
        shapes_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_geographical_area_t *OpenAPI_geographical_area_copy(OpenAPI_geographical_area_t *dst, OpenAPI_geographical_area_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_geographical_area_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_geographical_area_convertToJSON() failed");
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

    OpenAPI_geographical_area_free(dst);
    dst = OpenAPI_geographical_area_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

