
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "bluetooth_measurement.h"

OpenAPI_bluetooth_measurement_t *OpenAPI_bluetooth_measurement_create(
    OpenAPI_list_t *measurement_name_list,
    OpenAPI_bluetooth_rssi_e bluetooth_rssi
)
{
    OpenAPI_bluetooth_measurement_t *bluetooth_measurement_local_var = ogs_malloc(sizeof(OpenAPI_bluetooth_measurement_t));
    ogs_assert(bluetooth_measurement_local_var);

    bluetooth_measurement_local_var->measurement_name_list = measurement_name_list;
    bluetooth_measurement_local_var->bluetooth_rssi = bluetooth_rssi;

    return bluetooth_measurement_local_var;
}

void OpenAPI_bluetooth_measurement_free(OpenAPI_bluetooth_measurement_t *bluetooth_measurement)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == bluetooth_measurement) {
        return;
    }
    if (bluetooth_measurement->measurement_name_list) {
        OpenAPI_list_for_each(bluetooth_measurement->measurement_name_list, node) {
            OpenAPI_measurement_name_free(node->data);
        }
        OpenAPI_list_free(bluetooth_measurement->measurement_name_list);
        bluetooth_measurement->measurement_name_list = NULL;
    }
    ogs_free(bluetooth_measurement);
}

cJSON *OpenAPI_bluetooth_measurement_convertToJSON(OpenAPI_bluetooth_measurement_t *bluetooth_measurement)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (bluetooth_measurement == NULL) {
        ogs_error("OpenAPI_bluetooth_measurement_convertToJSON() failed [BluetoothMeasurement]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (bluetooth_measurement->measurement_name_list) {
    cJSON *measurement_name_listList = cJSON_AddArrayToObject(item, "measurementNameList");
    if (measurement_name_listList == NULL) {
        ogs_error("OpenAPI_bluetooth_measurement_convertToJSON() failed [measurement_name_list]");
        goto end;
    }
    OpenAPI_list_for_each(bluetooth_measurement->measurement_name_list, node) {
        cJSON *itemLocal = OpenAPI_measurement_name_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_bluetooth_measurement_convertToJSON() failed [measurement_name_list]");
            goto end;
        }
        cJSON_AddItemToArray(measurement_name_listList, itemLocal);
    }
    }

    if (bluetooth_measurement->bluetooth_rssi != OpenAPI_bluetooth_rssi_NULL) {
    if (cJSON_AddStringToObject(item, "bluetoothRssi", OpenAPI_bluetooth_rssi_ToString(bluetooth_measurement->bluetooth_rssi)) == NULL) {
        ogs_error("OpenAPI_bluetooth_measurement_convertToJSON() failed [bluetooth_rssi]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_bluetooth_measurement_t *OpenAPI_bluetooth_measurement_parseFromJSON(cJSON *bluetooth_measurementJSON)
{
    OpenAPI_bluetooth_measurement_t *bluetooth_measurement_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *measurement_name_list = NULL;
    OpenAPI_list_t *measurement_name_listList = NULL;
    cJSON *bluetooth_rssi = NULL;
    OpenAPI_bluetooth_rssi_e bluetooth_rssiVariable = 0;
    measurement_name_list = cJSON_GetObjectItemCaseSensitive(bluetooth_measurementJSON, "measurementNameList");
    if (measurement_name_list) {
        cJSON *measurement_name_list_local = NULL;
        if (!cJSON_IsArray(measurement_name_list)) {
            ogs_error("OpenAPI_bluetooth_measurement_parseFromJSON() failed [measurement_name_list]");
            goto end;
        }

        measurement_name_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(measurement_name_list_local, measurement_name_list) {
            if (!cJSON_IsObject(measurement_name_list_local)) {
                ogs_error("OpenAPI_bluetooth_measurement_parseFromJSON() failed [measurement_name_list]");
                goto end;
            }
            OpenAPI_measurement_name_t *measurement_name_listItem = OpenAPI_measurement_name_parseFromJSON(measurement_name_list_local);
            if (!measurement_name_listItem) {
                ogs_error("No measurement_name_listItem");
                goto end;
            }
            OpenAPI_list_add(measurement_name_listList, measurement_name_listItem);
        }
    }

    bluetooth_rssi = cJSON_GetObjectItemCaseSensitive(bluetooth_measurementJSON, "bluetoothRssi");
    if (bluetooth_rssi) {
    if (!cJSON_IsString(bluetooth_rssi)) {
        ogs_error("OpenAPI_bluetooth_measurement_parseFromJSON() failed [bluetooth_rssi]");
        goto end;
    }
    bluetooth_rssiVariable = OpenAPI_bluetooth_rssi_FromString(bluetooth_rssi->valuestring);
    }

    bluetooth_measurement_local_var = OpenAPI_bluetooth_measurement_create (
        measurement_name_list ? measurement_name_listList : NULL,
        bluetooth_rssi ? bluetooth_rssiVariable : 0
    );

    return bluetooth_measurement_local_var;
end:
    if (measurement_name_listList) {
        OpenAPI_list_for_each(measurement_name_listList, node) {
            OpenAPI_measurement_name_free(node->data);
        }
        OpenAPI_list_free(measurement_name_listList);
        measurement_name_listList = NULL;
    }
    return NULL;
}

OpenAPI_bluetooth_measurement_t *OpenAPI_bluetooth_measurement_copy(OpenAPI_bluetooth_measurement_t *dst, OpenAPI_bluetooth_measurement_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_bluetooth_measurement_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_bluetooth_measurement_convertToJSON() failed");
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

    OpenAPI_bluetooth_measurement_free(dst);
    dst = OpenAPI_bluetooth_measurement_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

