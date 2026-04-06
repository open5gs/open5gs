
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "model_2_g3_g_location_area_range.h"

OpenAPI_model_2_g3_g_location_area_range_t *OpenAPI_model_2_g3_g_location_area_range_create(
    OpenAPI_location_area_id_range_t *lai_range,
    OpenAPI_routing_area_id_range_t *rai_range
)
{
    OpenAPI_model_2_g3_g_location_area_range_t *model_2_g3_g_location_area_range_local_var = ogs_malloc(sizeof(OpenAPI_model_2_g3_g_location_area_range_t));
    ogs_assert(model_2_g3_g_location_area_range_local_var);

    model_2_g3_g_location_area_range_local_var->lai_range = lai_range;
    model_2_g3_g_location_area_range_local_var->rai_range = rai_range;

    return model_2_g3_g_location_area_range_local_var;
}

void OpenAPI_model_2_g3_g_location_area_range_free(OpenAPI_model_2_g3_g_location_area_range_t *model_2_g3_g_location_area_range)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == model_2_g3_g_location_area_range) {
        return;
    }
    if (model_2_g3_g_location_area_range->lai_range) {
        OpenAPI_location_area_id_range_free(model_2_g3_g_location_area_range->lai_range);
        model_2_g3_g_location_area_range->lai_range = NULL;
    }
    if (model_2_g3_g_location_area_range->rai_range) {
        OpenAPI_routing_area_id_range_free(model_2_g3_g_location_area_range->rai_range);
        model_2_g3_g_location_area_range->rai_range = NULL;
    }
    ogs_free(model_2_g3_g_location_area_range);
}

cJSON *OpenAPI_model_2_g3_g_location_area_range_convertToJSON(OpenAPI_model_2_g3_g_location_area_range_t *model_2_g3_g_location_area_range)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (model_2_g3_g_location_area_range == NULL) {
        ogs_error("OpenAPI_model_2_g3_g_location_area_range_convertToJSON() failed [2G3GLocationAreaRange]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (model_2_g3_g_location_area_range->lai_range) {
    cJSON *lai_range_local_JSON = OpenAPI_location_area_id_range_convertToJSON(model_2_g3_g_location_area_range->lai_range);
    if (lai_range_local_JSON == NULL) {
        ogs_error("OpenAPI_model_2_g3_g_location_area_range_convertToJSON() failed [lai_range]");
        goto end;
    }
    cJSON_AddItemToObject(item, "laiRange", lai_range_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_model_2_g3_g_location_area_range_convertToJSON() failed [lai_range]");
        goto end;
    }
    }

    if (model_2_g3_g_location_area_range->rai_range) {
    cJSON *rai_range_local_JSON = OpenAPI_routing_area_id_range_convertToJSON(model_2_g3_g_location_area_range->rai_range);
    if (rai_range_local_JSON == NULL) {
        ogs_error("OpenAPI_model_2_g3_g_location_area_range_convertToJSON() failed [rai_range]");
        goto end;
    }
    cJSON_AddItemToObject(item, "raiRange", rai_range_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_model_2_g3_g_location_area_range_convertToJSON() failed [rai_range]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_model_2_g3_g_location_area_range_t *OpenAPI_model_2_g3_g_location_area_range_parseFromJSON(cJSON *model_2_g3_g_location_area_rangeJSON)
{
    OpenAPI_model_2_g3_g_location_area_range_t *model_2_g3_g_location_area_range_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *lai_range = NULL;
    OpenAPI_location_area_id_range_t *lai_range_local_nonprim = NULL;
    cJSON *rai_range = NULL;
    OpenAPI_routing_area_id_range_t *rai_range_local_nonprim = NULL;
    lai_range = cJSON_GetObjectItemCaseSensitive(model_2_g3_g_location_area_rangeJSON, "laiRange");
    if (lai_range) {
    lai_range_local_nonprim = OpenAPI_location_area_id_range_parseFromJSON(lai_range);
    if (!lai_range_local_nonprim) {
        ogs_error("OpenAPI_location_area_id_range_parseFromJSON failed [lai_range]");
        goto end;
    }
    }

    rai_range = cJSON_GetObjectItemCaseSensitive(model_2_g3_g_location_area_rangeJSON, "raiRange");
    if (rai_range) {
    rai_range_local_nonprim = OpenAPI_routing_area_id_range_parseFromJSON(rai_range);
    if (!rai_range_local_nonprim) {
        ogs_error("OpenAPI_routing_area_id_range_parseFromJSON failed [rai_range]");
        goto end;
    }
    }

    model_2_g3_g_location_area_range_local_var = OpenAPI_model_2_g3_g_location_area_range_create (
        lai_range ? lai_range_local_nonprim : NULL,
        rai_range ? rai_range_local_nonprim : NULL
    );

    return model_2_g3_g_location_area_range_local_var;
end:
    if (lai_range_local_nonprim) {
        OpenAPI_location_area_id_range_free(lai_range_local_nonprim);
        lai_range_local_nonprim = NULL;
    }
    if (rai_range_local_nonprim) {
        OpenAPI_routing_area_id_range_free(rai_range_local_nonprim);
        rai_range_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_model_2_g3_g_location_area_range_t *OpenAPI_model_2_g3_g_location_area_range_copy(OpenAPI_model_2_g3_g_location_area_range_t *dst, OpenAPI_model_2_g3_g_location_area_range_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_model_2_g3_g_location_area_range_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_model_2_g3_g_location_area_range_convertToJSON() failed");
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

    OpenAPI_model_2_g3_g_location_area_range_free(dst);
    dst = OpenAPI_model_2_g3_g_location_area_range_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

