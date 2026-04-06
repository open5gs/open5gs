
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "timestamped_location.h"

OpenAPI_timestamped_location_t *OpenAPI_timestamped_location_create(
    char *ts,
    OpenAPI_location_info_t *loc_info,
    char *supi,
    char *gpsi
)
{
    OpenAPI_timestamped_location_t *timestamped_location_local_var = ogs_malloc(sizeof(OpenAPI_timestamped_location_t));
    ogs_assert(timestamped_location_local_var);

    timestamped_location_local_var->ts = ts;
    timestamped_location_local_var->loc_info = loc_info;
    timestamped_location_local_var->supi = supi;
    timestamped_location_local_var->gpsi = gpsi;

    return timestamped_location_local_var;
}

void OpenAPI_timestamped_location_free(OpenAPI_timestamped_location_t *timestamped_location)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == timestamped_location) {
        return;
    }
    if (timestamped_location->ts) {
        ogs_free(timestamped_location->ts);
        timestamped_location->ts = NULL;
    }
    if (timestamped_location->loc_info) {
        OpenAPI_location_info_free(timestamped_location->loc_info);
        timestamped_location->loc_info = NULL;
    }
    if (timestamped_location->supi) {
        ogs_free(timestamped_location->supi);
        timestamped_location->supi = NULL;
    }
    if (timestamped_location->gpsi) {
        ogs_free(timestamped_location->gpsi);
        timestamped_location->gpsi = NULL;
    }
    ogs_free(timestamped_location);
}

cJSON *OpenAPI_timestamped_location_convertToJSON(OpenAPI_timestamped_location_t *timestamped_location)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (timestamped_location == NULL) {
        ogs_error("OpenAPI_timestamped_location_convertToJSON() failed [TimestampedLocation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!timestamped_location->ts) {
        ogs_error("OpenAPI_timestamped_location_convertToJSON() failed [ts]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "ts", timestamped_location->ts) == NULL) {
        ogs_error("OpenAPI_timestamped_location_convertToJSON() failed [ts]");
        goto end;
    }

    if (!timestamped_location->loc_info) {
        ogs_error("OpenAPI_timestamped_location_convertToJSON() failed [loc_info]");
        return NULL;
    }
    cJSON *loc_info_local_JSON = OpenAPI_location_info_convertToJSON(timestamped_location->loc_info);
    if (loc_info_local_JSON == NULL) {
        ogs_error("OpenAPI_timestamped_location_convertToJSON() failed [loc_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "locInfo", loc_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_timestamped_location_convertToJSON() failed [loc_info]");
        goto end;
    }

    if (timestamped_location->supi) {
    if (cJSON_AddStringToObject(item, "supi", timestamped_location->supi) == NULL) {
        ogs_error("OpenAPI_timestamped_location_convertToJSON() failed [supi]");
        goto end;
    }
    }

    if (timestamped_location->gpsi) {
    if (cJSON_AddStringToObject(item, "gpsi", timestamped_location->gpsi) == NULL) {
        ogs_error("OpenAPI_timestamped_location_convertToJSON() failed [gpsi]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_timestamped_location_t *OpenAPI_timestamped_location_parseFromJSON(cJSON *timestamped_locationJSON)
{
    OpenAPI_timestamped_location_t *timestamped_location_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ts = NULL;
    cJSON *loc_info = NULL;
    OpenAPI_location_info_t *loc_info_local_nonprim = NULL;
    cJSON *supi = NULL;
    cJSON *gpsi = NULL;
    ts = cJSON_GetObjectItemCaseSensitive(timestamped_locationJSON, "ts");
    if (!ts) {
        ogs_error("OpenAPI_timestamped_location_parseFromJSON() failed [ts]");
        goto end;
    }
    if (!cJSON_IsString(ts) && !cJSON_IsNull(ts)) {
        ogs_error("OpenAPI_timestamped_location_parseFromJSON() failed [ts]");
        goto end;
    }

    loc_info = cJSON_GetObjectItemCaseSensitive(timestamped_locationJSON, "locInfo");
    if (!loc_info) {
        ogs_error("OpenAPI_timestamped_location_parseFromJSON() failed [loc_info]");
        goto end;
    }
    loc_info_local_nonprim = OpenAPI_location_info_parseFromJSON(loc_info);
    if (!loc_info_local_nonprim) {
        ogs_error("OpenAPI_location_info_parseFromJSON failed [loc_info]");
        goto end;
    }

    supi = cJSON_GetObjectItemCaseSensitive(timestamped_locationJSON, "supi");
    if (supi) {
    if (!cJSON_IsString(supi) && !cJSON_IsNull(supi)) {
        ogs_error("OpenAPI_timestamped_location_parseFromJSON() failed [supi]");
        goto end;
    }
    }

    gpsi = cJSON_GetObjectItemCaseSensitive(timestamped_locationJSON, "gpsi");
    if (gpsi) {
    if (!cJSON_IsString(gpsi) && !cJSON_IsNull(gpsi)) {
        ogs_error("OpenAPI_timestamped_location_parseFromJSON() failed [gpsi]");
        goto end;
    }
    }

    timestamped_location_local_var = OpenAPI_timestamped_location_create (
        ogs_strdup(ts->valuestring),
        loc_info_local_nonprim,
        supi && !cJSON_IsNull(supi) ? ogs_strdup(supi->valuestring) : NULL,
        gpsi && !cJSON_IsNull(gpsi) ? ogs_strdup(gpsi->valuestring) : NULL
    );

    return timestamped_location_local_var;
end:
    if (loc_info_local_nonprim) {
        OpenAPI_location_info_free(loc_info_local_nonprim);
        loc_info_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_timestamped_location_t *OpenAPI_timestamped_location_copy(OpenAPI_timestamped_location_t *dst, OpenAPI_timestamped_location_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_timestamped_location_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_timestamped_location_convertToJSON() failed");
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

    OpenAPI_timestamped_location_free(dst);
    dst = OpenAPI_timestamped_location_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

