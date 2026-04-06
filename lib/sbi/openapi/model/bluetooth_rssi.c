
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "bluetooth_rssi.h"

char* OpenAPI_bluetooth_rssi_ToString(OpenAPI_bluetooth_rssi_e bluetooth_rssi)
{
    const char *bluetooth_rssiArray[] =  { "NULL", "true" };
    size_t sizeofArray = sizeof(bluetooth_rssiArray) / sizeof(bluetooth_rssiArray[0]);
    if (bluetooth_rssi < sizeofArray)
        return (char *)bluetooth_rssiArray[bluetooth_rssi];
    else
        return (char *)"Unknown";
}

OpenAPI_bluetooth_rssi_e OpenAPI_bluetooth_rssi_FromString(char* bluetooth_rssi)
{
    int stringToReturn = 0;
    const char *bluetooth_rssiArray[] =  { "NULL", "true" };
    size_t sizeofArray = sizeof(bluetooth_rssiArray) / sizeof(bluetooth_rssiArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(bluetooth_rssi, bluetooth_rssiArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

