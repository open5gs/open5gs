
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "model_2_g3_g_location_area.h"

OpenAPI_model_2_g3_g_location_area_t *OpenAPI_model_2_g3_g_location_area_create(
    OpenAPI_location_area_id_t *lai,
    OpenAPI_routing_area_id_t *rai
)
{
    OpenAPI_model_2_g3_g_location_area_t *model_2_g3_g_location_area_local_var = ogs_malloc(sizeof(OpenAPI_model_2_g3_g_location_area_t));
    ogs_assert(model_2_g3_g_location_area_local_var);

    model_2_g3_g_location_area_local_var->lai = lai;
    model_2_g3_g_location_area_local_var->rai = rai;

    return model_2_g3_g_location_area_local_var;
}

void OpenAPI_model_2_g3_g_location_area_free(OpenAPI_model_2_g3_g_location_area_t *model_2_g3_g_location_area)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == model_2_g3_g_location_area) {
        return;
    }
    if (model_2_g3_g_location_area->lai) {
        OpenAPI_location_area_id_free(model_2_g3_g_location_area->lai);
        model_2_g3_g_location_area->lai = NULL;
    }
    if (model_2_g3_g_location_area->rai) {
        OpenAPI_routing_area_id_free(model_2_g3_g_location_area->rai);
        model_2_g3_g_location_area->rai = NULL;
    }
    ogs_free(model_2_g3_g_location_area);
}

cJSON *OpenAPI_model_2_g3_g_location_area_convertToJSON(OpenAPI_model_2_g3_g_location_area_t *model_2_g3_g_location_area)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (model_2_g3_g_location_area == NULL) {
        ogs_error("OpenAPI_model_2_g3_g_location_area_convertToJSON() failed [2G3GLocationArea]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (model_2_g3_g_location_area->lai) {
    cJSON *lai_local_JSON = OpenAPI_location_area_id_convertToJSON(model_2_g3_g_location_area->lai);
    if (lai_local_JSON == NULL) {
        ogs_error("OpenAPI_model_2_g3_g_location_area_convertToJSON() failed [lai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "lai", lai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_model_2_g3_g_location_area_convertToJSON() failed [lai]");
        goto end;
    }
    }

    if (model_2_g3_g_location_area->rai) {
    cJSON *rai_local_JSON = OpenAPI_routing_area_id_convertToJSON(model_2_g3_g_location_area->rai);
    if (rai_local_JSON == NULL) {
        ogs_error("OpenAPI_model_2_g3_g_location_area_convertToJSON() failed [rai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "rai", rai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_model_2_g3_g_location_area_convertToJSON() failed [rai]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_model_2_g3_g_location_area_t *OpenAPI_model_2_g3_g_location_area_parseFromJSON(cJSON *model_2_g3_g_location_areaJSON)
{
    OpenAPI_model_2_g3_g_location_area_t *model_2_g3_g_location_area_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *lai = NULL;
    OpenAPI_location_area_id_t *lai_local_nonprim = NULL;
    cJSON *rai = NULL;
    OpenAPI_routing_area_id_t *rai_local_nonprim = NULL;
    lai = cJSON_GetObjectItemCaseSensitive(model_2_g3_g_location_areaJSON, "lai");
    if (lai) {
    lai_local_nonprim = OpenAPI_location_area_id_parseFromJSON(lai);
    if (!lai_local_nonprim) {
        ogs_error("OpenAPI_location_area_id_parseFromJSON failed [lai]");
        goto end;
    }
    }

    rai = cJSON_GetObjectItemCaseSensitive(model_2_g3_g_location_areaJSON, "rai");
    if (rai) {
    rai_local_nonprim = OpenAPI_routing_area_id_parseFromJSON(rai);
    if (!rai_local_nonprim) {
        ogs_error("OpenAPI_routing_area_id_parseFromJSON failed [rai]");
        goto end;
    }
    }

    model_2_g3_g_location_area_local_var = OpenAPI_model_2_g3_g_location_area_create (
        lai ? lai_local_nonprim : NULL,
        rai ? rai_local_nonprim : NULL
    );

    return model_2_g3_g_location_area_local_var;
end:
    if (lai_local_nonprim) {
        OpenAPI_location_area_id_free(lai_local_nonprim);
        lai_local_nonprim = NULL;
    }
    if (rai_local_nonprim) {
        OpenAPI_routing_area_id_free(rai_local_nonprim);
        rai_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_model_2_g3_g_location_area_t *OpenAPI_model_2_g3_g_location_area_copy(OpenAPI_model_2_g3_g_location_area_t *dst, OpenAPI_model_2_g3_g_location_area_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_model_2_g3_g_location_area_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_model_2_g3_g_location_area_convertToJSON() failed");
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

    OpenAPI_model_2_g3_g_location_area_free(dst);
    dst = OpenAPI_model_2_g3_g_location_area_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

