
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "location_info.h"

OpenAPI_location_info_t *OpenAPI_location_info_create(
    OpenAPI_user_location_t *loc,
    bool is_ratio,
    int ratio,
    bool is_confidence,
    int confidence
)
{
    OpenAPI_location_info_t *location_info_local_var = ogs_malloc(sizeof(OpenAPI_location_info_t));
    ogs_assert(location_info_local_var);

    location_info_local_var->loc = loc;
    location_info_local_var->is_ratio = is_ratio;
    location_info_local_var->ratio = ratio;
    location_info_local_var->is_confidence = is_confidence;
    location_info_local_var->confidence = confidence;

    return location_info_local_var;
}

void OpenAPI_location_info_free(OpenAPI_location_info_t *location_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == location_info) {
        return;
    }
    if (location_info->loc) {
        OpenAPI_user_location_free(location_info->loc);
        location_info->loc = NULL;
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
    if (!location_info->loc) {
        ogs_error("OpenAPI_location_info_convertToJSON() failed [loc]");
        return NULL;
    }
    cJSON *loc_local_JSON = OpenAPI_user_location_convertToJSON(location_info->loc);
    if (loc_local_JSON == NULL) {
        ogs_error("OpenAPI_location_info_convertToJSON() failed [loc]");
        goto end;
    }
    cJSON_AddItemToObject(item, "loc", loc_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_location_info_convertToJSON() failed [loc]");
        goto end;
    }

    if (location_info->is_ratio) {
    if (cJSON_AddNumberToObject(item, "ratio", location_info->ratio) == NULL) {
        ogs_error("OpenAPI_location_info_convertToJSON() failed [ratio]");
        goto end;
    }
    }

    if (location_info->is_confidence) {
    if (cJSON_AddNumberToObject(item, "confidence", location_info->confidence) == NULL) {
        ogs_error("OpenAPI_location_info_convertToJSON() failed [confidence]");
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
    cJSON *loc = NULL;
    OpenAPI_user_location_t *loc_local_nonprim = NULL;
    cJSON *ratio = NULL;
    cJSON *confidence = NULL;
    loc = cJSON_GetObjectItemCaseSensitive(location_infoJSON, "loc");
    if (!loc) {
        ogs_error("OpenAPI_location_info_parseFromJSON() failed [loc]");
        goto end;
    }
    loc_local_nonprim = OpenAPI_user_location_parseFromJSON(loc);
    if (!loc_local_nonprim) {
        ogs_error("OpenAPI_user_location_parseFromJSON failed [loc]");
        goto end;
    }

    ratio = cJSON_GetObjectItemCaseSensitive(location_infoJSON, "ratio");
    if (ratio) {
    if (!cJSON_IsNumber(ratio)) {
        ogs_error("OpenAPI_location_info_parseFromJSON() failed [ratio]");
        goto end;
    }
    }

    confidence = cJSON_GetObjectItemCaseSensitive(location_infoJSON, "confidence");
    if (confidence) {
    if (!cJSON_IsNumber(confidence)) {
        ogs_error("OpenAPI_location_info_parseFromJSON() failed [confidence]");
        goto end;
    }
    }

    location_info_local_var = OpenAPI_location_info_create (
        loc_local_nonprim,
        ratio ? true : false,
        ratio ? ratio->valuedouble : 0,
        confidence ? true : false,
        confidence ? confidence->valuedouble : 0
    );

    return location_info_local_var;
end:
    if (loc_local_nonprim) {
        OpenAPI_user_location_free(loc_local_nonprim);
        loc_local_nonprim = NULL;
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

