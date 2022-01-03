
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mbsfn_area.h"

OpenAPI_mbsfn_area_t *OpenAPI_mbsfn_area_create(
    bool is_mbsfn_area_id,
    int mbsfn_area_id,
    bool is_carrier_frequency,
    int carrier_frequency
)
{
    OpenAPI_mbsfn_area_t *mbsfn_area_local_var = ogs_malloc(sizeof(OpenAPI_mbsfn_area_t));
    ogs_assert(mbsfn_area_local_var);

    mbsfn_area_local_var->is_mbsfn_area_id = is_mbsfn_area_id;
    mbsfn_area_local_var->mbsfn_area_id = mbsfn_area_id;
    mbsfn_area_local_var->is_carrier_frequency = is_carrier_frequency;
    mbsfn_area_local_var->carrier_frequency = carrier_frequency;

    return mbsfn_area_local_var;
}

void OpenAPI_mbsfn_area_free(OpenAPI_mbsfn_area_t *mbsfn_area)
{
    if (NULL == mbsfn_area) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(mbsfn_area);
}

cJSON *OpenAPI_mbsfn_area_convertToJSON(OpenAPI_mbsfn_area_t *mbsfn_area)
{
    cJSON *item = NULL;

    if (mbsfn_area == NULL) {
        ogs_error("OpenAPI_mbsfn_area_convertToJSON() failed [MbsfnArea]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (mbsfn_area->is_mbsfn_area_id) {
    if (cJSON_AddNumberToObject(item, "mbsfnAreaId", mbsfn_area->mbsfn_area_id) == NULL) {
        ogs_error("OpenAPI_mbsfn_area_convertToJSON() failed [mbsfn_area_id]");
        goto end;
    }
    }

    if (mbsfn_area->is_carrier_frequency) {
    if (cJSON_AddNumberToObject(item, "carrierFrequency", mbsfn_area->carrier_frequency) == NULL) {
        ogs_error("OpenAPI_mbsfn_area_convertToJSON() failed [carrier_frequency]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_mbsfn_area_t *OpenAPI_mbsfn_area_parseFromJSON(cJSON *mbsfn_areaJSON)
{
    OpenAPI_mbsfn_area_t *mbsfn_area_local_var = NULL;
    cJSON *mbsfn_area_id = cJSON_GetObjectItemCaseSensitive(mbsfn_areaJSON, "mbsfnAreaId");

    if (mbsfn_area_id) {
    if (!cJSON_IsNumber(mbsfn_area_id)) {
        ogs_error("OpenAPI_mbsfn_area_parseFromJSON() failed [mbsfn_area_id]");
        goto end;
    }
    }

    cJSON *carrier_frequency = cJSON_GetObjectItemCaseSensitive(mbsfn_areaJSON, "carrierFrequency");

    if (carrier_frequency) {
    if (!cJSON_IsNumber(carrier_frequency)) {
        ogs_error("OpenAPI_mbsfn_area_parseFromJSON() failed [carrier_frequency]");
        goto end;
    }
    }

    mbsfn_area_local_var = OpenAPI_mbsfn_area_create (
        mbsfn_area_id ? true : false,
        mbsfn_area_id ? mbsfn_area_id->valuedouble : 0,
        carrier_frequency ? true : false,
        carrier_frequency ? carrier_frequency->valuedouble : 0
    );

    return mbsfn_area_local_var;
end:
    return NULL;
}

OpenAPI_mbsfn_area_t *OpenAPI_mbsfn_area_copy(OpenAPI_mbsfn_area_t *dst, OpenAPI_mbsfn_area_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_mbsfn_area_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_mbsfn_area_convertToJSON() failed");
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

    OpenAPI_mbsfn_area_free(dst);
    dst = OpenAPI_mbsfn_area_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

