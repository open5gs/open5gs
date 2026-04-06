
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "device_type.h"

char* OpenAPI_device_type_ToString(OpenAPI_device_type_e device_type)
{
    const char *device_typeArray[] =  { "NULL", "MOBILE_PHONE", "SMART_PHONE", "TABLET", "DONGLE", "MODEM", "WLAN_ROUTER", "IOT_DEVICE", "WEARABLE", "MOBILE_TEST_PLATFORM", "UNDEFINED" };
    size_t sizeofArray = sizeof(device_typeArray) / sizeof(device_typeArray[0]);
    if (device_type < sizeofArray)
        return (char *)device_typeArray[device_type];
    else
        return (char *)"Unknown";
}

OpenAPI_device_type_e OpenAPI_device_type_FromString(char* device_type)
{
    int stringToReturn = 0;
    const char *device_typeArray[] =  { "NULL", "MOBILE_PHONE", "SMART_PHONE", "TABLET", "DONGLE", "MODEM", "WLAN_ROUTER", "IOT_DEVICE", "WEARABLE", "MOBILE_TEST_PLATFORM", "UNDEFINED" };
    size_t sizeofArray = sizeof(device_typeArray) / sizeof(device_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(device_type, device_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

