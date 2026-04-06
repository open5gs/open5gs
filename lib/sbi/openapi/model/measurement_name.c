
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "measurement_name.h"

OpenAPI_measurement_name_t *OpenAPI_measurement_name_create(
    char *bluetooth_name,
    char *wlan_name
)
{
    OpenAPI_measurement_name_t *measurement_name_local_var = ogs_malloc(sizeof(OpenAPI_measurement_name_t));
    ogs_assert(measurement_name_local_var);

    measurement_name_local_var->bluetooth_name = bluetooth_name;
    measurement_name_local_var->wlan_name = wlan_name;

    return measurement_name_local_var;
}

void OpenAPI_measurement_name_free(OpenAPI_measurement_name_t *measurement_name)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == measurement_name) {
        return;
    }
    if (measurement_name->bluetooth_name) {
        ogs_free(measurement_name->bluetooth_name);
        measurement_name->bluetooth_name = NULL;
    }
    if (measurement_name->wlan_name) {
        ogs_free(measurement_name->wlan_name);
        measurement_name->wlan_name = NULL;
    }
    ogs_free(measurement_name);
}

cJSON *OpenAPI_measurement_name_convertToJSON(OpenAPI_measurement_name_t *measurement_name)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (measurement_name == NULL) {
        ogs_error("OpenAPI_measurement_name_convertToJSON() failed [MeasurementName]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (measurement_name->bluetooth_name) {
    if (cJSON_AddStringToObject(item, "bluetoothName", measurement_name->bluetooth_name) == NULL) {
        ogs_error("OpenAPI_measurement_name_convertToJSON() failed [bluetooth_name]");
        goto end;
    }
    }

    if (measurement_name->wlan_name) {
    if (cJSON_AddStringToObject(item, "wlanName", measurement_name->wlan_name) == NULL) {
        ogs_error("OpenAPI_measurement_name_convertToJSON() failed [wlan_name]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_measurement_name_t *OpenAPI_measurement_name_parseFromJSON(cJSON *measurement_nameJSON)
{
    OpenAPI_measurement_name_t *measurement_name_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *bluetooth_name = NULL;
    cJSON *wlan_name = NULL;
    bluetooth_name = cJSON_GetObjectItemCaseSensitive(measurement_nameJSON, "bluetoothName");
    if (bluetooth_name) {
    if (!cJSON_IsString(bluetooth_name) && !cJSON_IsNull(bluetooth_name)) {
        ogs_error("OpenAPI_measurement_name_parseFromJSON() failed [bluetooth_name]");
        goto end;
    }
    }

    wlan_name = cJSON_GetObjectItemCaseSensitive(measurement_nameJSON, "wlanName");
    if (wlan_name) {
    if (!cJSON_IsString(wlan_name) && !cJSON_IsNull(wlan_name)) {
        ogs_error("OpenAPI_measurement_name_parseFromJSON() failed [wlan_name]");
        goto end;
    }
    }

    measurement_name_local_var = OpenAPI_measurement_name_create (
        bluetooth_name && !cJSON_IsNull(bluetooth_name) ? ogs_strdup(bluetooth_name->valuestring) : NULL,
        wlan_name && !cJSON_IsNull(wlan_name) ? ogs_strdup(wlan_name->valuestring) : NULL
    );

    return measurement_name_local_var;
end:
    return NULL;
}

OpenAPI_measurement_name_t *OpenAPI_measurement_name_copy(OpenAPI_measurement_name_t *dst, OpenAPI_measurement_name_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_measurement_name_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_measurement_name_convertToJSON() failed");
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

    OpenAPI_measurement_name_free(dst);
    dst = OpenAPI_measurement_name_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

