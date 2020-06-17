
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "location_reporting_configuration.h"

OpenAPI_location_reporting_configuration_t *OpenAPI_location_reporting_configuration_create(
    int current_location,
    int one_time,
    OpenAPI_location_accuracy_t *accuracy,
    OpenAPI_location_accuracy_t *n3gpp_accuracy
    )
{
    OpenAPI_location_reporting_configuration_t *location_reporting_configuration_local_var = OpenAPI_malloc(sizeof(OpenAPI_location_reporting_configuration_t));
    if (!location_reporting_configuration_local_var) {
        return NULL;
    }
    location_reporting_configuration_local_var->current_location = current_location;
    location_reporting_configuration_local_var->one_time = one_time;
    location_reporting_configuration_local_var->accuracy = accuracy;
    location_reporting_configuration_local_var->n3gpp_accuracy = n3gpp_accuracy;

    return location_reporting_configuration_local_var;
}

void OpenAPI_location_reporting_configuration_free(OpenAPI_location_reporting_configuration_t *location_reporting_configuration)
{
    if (NULL == location_reporting_configuration) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_location_accuracy_free(location_reporting_configuration->accuracy);
    OpenAPI_location_accuracy_free(location_reporting_configuration->n3gpp_accuracy);
    ogs_free(location_reporting_configuration);
}

cJSON *OpenAPI_location_reporting_configuration_convertToJSON(OpenAPI_location_reporting_configuration_t *location_reporting_configuration)
{
    cJSON *item = NULL;

    if (location_reporting_configuration == NULL) {
        ogs_error("OpenAPI_location_reporting_configuration_convertToJSON() failed [LocationReportingConfiguration]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddBoolToObject(item, "currentLocation", location_reporting_configuration->current_location) == NULL) {
        ogs_error("OpenAPI_location_reporting_configuration_convertToJSON() failed [current_location]");
        goto end;
    }

    if (location_reporting_configuration->one_time) {
        if (cJSON_AddBoolToObject(item, "oneTime", location_reporting_configuration->one_time) == NULL) {
            ogs_error("OpenAPI_location_reporting_configuration_convertToJSON() failed [one_time]");
            goto end;
        }
    }

    if (location_reporting_configuration->accuracy) {
        cJSON *accuracy_local_JSON = OpenAPI_location_accuracy_convertToJSON(location_reporting_configuration->accuracy);
        if (accuracy_local_JSON == NULL) {
            ogs_error("OpenAPI_location_reporting_configuration_convertToJSON() failed [accuracy]");
            goto end;
        }
        cJSON_AddItemToObject(item, "accuracy", accuracy_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_location_reporting_configuration_convertToJSON() failed [accuracy]");
            goto end;
        }
    }

    if (location_reporting_configuration->n3gpp_accuracy) {
        cJSON *n3gpp_accuracy_local_JSON = OpenAPI_location_accuracy_convertToJSON(location_reporting_configuration->n3gpp_accuracy);
        if (n3gpp_accuracy_local_JSON == NULL) {
            ogs_error("OpenAPI_location_reporting_configuration_convertToJSON() failed [n3gpp_accuracy]");
            goto end;
        }
        cJSON_AddItemToObject(item, "n3gppAccuracy", n3gpp_accuracy_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_location_reporting_configuration_convertToJSON() failed [n3gpp_accuracy]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_location_reporting_configuration_t *OpenAPI_location_reporting_configuration_parseFromJSON(cJSON *location_reporting_configurationJSON)
{
    OpenAPI_location_reporting_configuration_t *location_reporting_configuration_local_var = NULL;
    cJSON *current_location = cJSON_GetObjectItemCaseSensitive(location_reporting_configurationJSON, "currentLocation");
    if (!current_location) {
        ogs_error("OpenAPI_location_reporting_configuration_parseFromJSON() failed [current_location]");
        goto end;
    }


    if (!cJSON_IsBool(current_location)) {
        ogs_error("OpenAPI_location_reporting_configuration_parseFromJSON() failed [current_location]");
        goto end;
    }

    cJSON *one_time = cJSON_GetObjectItemCaseSensitive(location_reporting_configurationJSON, "oneTime");

    if (one_time) {
        if (!cJSON_IsBool(one_time)) {
            ogs_error("OpenAPI_location_reporting_configuration_parseFromJSON() failed [one_time]");
            goto end;
        }
    }

    cJSON *accuracy = cJSON_GetObjectItemCaseSensitive(location_reporting_configurationJSON, "accuracy");

    OpenAPI_location_accuracy_t *accuracy_local_nonprim = NULL;
    if (accuracy) {
        accuracy_local_nonprim = OpenAPI_location_accuracy_parseFromJSON(accuracy);
    }

    cJSON *n3gpp_accuracy = cJSON_GetObjectItemCaseSensitive(location_reporting_configurationJSON, "n3gppAccuracy");

    OpenAPI_location_accuracy_t *n3gpp_accuracy_local_nonprim = NULL;
    if (n3gpp_accuracy) {
        n3gpp_accuracy_local_nonprim = OpenAPI_location_accuracy_parseFromJSON(n3gpp_accuracy);
    }

    location_reporting_configuration_local_var = OpenAPI_location_reporting_configuration_create (
        current_location->valueint,
        one_time ? one_time->valueint : 0,
        accuracy ? accuracy_local_nonprim : NULL,
        n3gpp_accuracy ? n3gpp_accuracy_local_nonprim : NULL
        );

    return location_reporting_configuration_local_var;
end:
    return NULL;
}

OpenAPI_location_reporting_configuration_t *OpenAPI_location_reporting_configuration_copy(OpenAPI_location_reporting_configuration_t *dst, OpenAPI_location_reporting_configuration_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_location_reporting_configuration_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_location_reporting_configuration_convertToJSON() failed");
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

    OpenAPI_location_reporting_configuration_free(dst);
    dst = OpenAPI_location_reporting_configuration_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

