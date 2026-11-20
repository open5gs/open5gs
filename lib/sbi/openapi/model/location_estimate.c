
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "location_estimate.h"

OpenAPI_location_estimate_t *OpenAPI_location_estimate_create(
    OpenAPI_charging_user_location_t *user_location_information,
    char *horizontal_accuracy,
    char *vertical_accuracy
)
{
    OpenAPI_location_estimate_t *location_estimate_local_var = ogs_malloc(sizeof(OpenAPI_location_estimate_t));
    ogs_assert(location_estimate_local_var);

    location_estimate_local_var->user_location_information = user_location_information;
    location_estimate_local_var->horizontal_accuracy = horizontal_accuracy;
    location_estimate_local_var->vertical_accuracy = vertical_accuracy;

    return location_estimate_local_var;
}

void OpenAPI_location_estimate_free(OpenAPI_location_estimate_t *location_estimate)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == location_estimate) {
        return;
    }
    if (location_estimate->user_location_information) {
        OpenAPI_charging_user_location_free(location_estimate->user_location_information);
        location_estimate->user_location_information = NULL;
    }
    if (location_estimate->horizontal_accuracy) {
        ogs_free(location_estimate->horizontal_accuracy);
        location_estimate->horizontal_accuracy = NULL;
    }
    if (location_estimate->vertical_accuracy) {
        ogs_free(location_estimate->vertical_accuracy);
        location_estimate->vertical_accuracy = NULL;
    }
    ogs_free(location_estimate);
}

cJSON *OpenAPI_location_estimate_convertToJSON(OpenAPI_location_estimate_t *location_estimate)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (location_estimate == NULL) {
        ogs_error("OpenAPI_location_estimate_convertToJSON() failed [LocationEstimate]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (location_estimate->user_location_information) {
    cJSON *user_location_information_local_JSON = OpenAPI_charging_user_location_convertToJSON(location_estimate->user_location_information);
    if (user_location_information_local_JSON == NULL) {
        ogs_error("OpenAPI_location_estimate_convertToJSON() failed [user_location_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "userLocationInformation", user_location_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_location_estimate_convertToJSON() failed [user_location_information]");
        goto end;
    }
    }

    if (location_estimate->horizontal_accuracy) {
    if (cJSON_AddStringToObject(item, "horizontalAccuracy", location_estimate->horizontal_accuracy) == NULL) {
        ogs_error("OpenAPI_location_estimate_convertToJSON() failed [horizontal_accuracy]");
        goto end;
    }
    }

    if (location_estimate->vertical_accuracy) {
    if (cJSON_AddStringToObject(item, "verticalAccuracy", location_estimate->vertical_accuracy) == NULL) {
        ogs_error("OpenAPI_location_estimate_convertToJSON() failed [vertical_accuracy]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_location_estimate_t *OpenAPI_location_estimate_parseFromJSON(cJSON *location_estimateJSON)
{
    OpenAPI_location_estimate_t *location_estimate_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *user_location_information = NULL;
    OpenAPI_charging_user_location_t *user_location_information_local_nonprim = NULL;
    cJSON *horizontal_accuracy = NULL;
    cJSON *vertical_accuracy = NULL;
    user_location_information = cJSON_GetObjectItemCaseSensitive(location_estimateJSON, "userLocationInformation");
    if (user_location_information) {
    user_location_information_local_nonprim = OpenAPI_charging_user_location_parseFromJSON(user_location_information);
    if (!user_location_information_local_nonprim) {
        ogs_error("OpenAPI_charging_user_location_parseFromJSON failed [user_location_information]");
        goto end;
    }
    }

    horizontal_accuracy = cJSON_GetObjectItemCaseSensitive(location_estimateJSON, "horizontalAccuracy");
    if (horizontal_accuracy) {
    if (!cJSON_IsString(horizontal_accuracy) && !cJSON_IsNull(horizontal_accuracy)) {
        ogs_error("OpenAPI_location_estimate_parseFromJSON() failed [horizontal_accuracy]");
        goto end;
    }
    }

    vertical_accuracy = cJSON_GetObjectItemCaseSensitive(location_estimateJSON, "verticalAccuracy");
    if (vertical_accuracy) {
    if (!cJSON_IsString(vertical_accuracy) && !cJSON_IsNull(vertical_accuracy)) {
        ogs_error("OpenAPI_location_estimate_parseFromJSON() failed [vertical_accuracy]");
        goto end;
    }
    }

    location_estimate_local_var = OpenAPI_location_estimate_create (
        user_location_information ? user_location_information_local_nonprim : NULL,
        horizontal_accuracy && !cJSON_IsNull(horizontal_accuracy) ? ogs_strdup(horizontal_accuracy->valuestring) : NULL,
        vertical_accuracy && !cJSON_IsNull(vertical_accuracy) ? ogs_strdup(vertical_accuracy->valuestring) : NULL
    );

    return location_estimate_local_var;
end:
    if (user_location_information_local_nonprim) {
        OpenAPI_charging_user_location_free(user_location_information_local_nonprim);
        user_location_information_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_location_estimate_t *OpenAPI_location_estimate_copy(OpenAPI_location_estimate_t *dst, OpenAPI_location_estimate_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_location_estimate_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_location_estimate_convertToJSON() failed");
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

    OpenAPI_location_estimate_free(dst);
    dst = OpenAPI_location_estimate_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

