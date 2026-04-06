
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "wlan_rssi.h"

char* OpenAPI_wlan_rssi_ToString(OpenAPI_wlan_rssi_e wlan_rssi)
{
    const char *wlan_rssiArray[] =  { "NULL", "true" };
    size_t sizeofArray = sizeof(wlan_rssiArray) / sizeof(wlan_rssiArray[0]);
    if (wlan_rssi < sizeofArray)
        return (char *)wlan_rssiArray[wlan_rssi];
    else
        return (char *)"Unknown";
}

OpenAPI_wlan_rssi_e OpenAPI_wlan_rssi_FromString(char* wlan_rssi)
{
    int stringToReturn = 0;
    const char *wlan_rssiArray[] =  { "NULL", "true" };
    size_t sizeofArray = sizeof(wlan_rssiArray) / sizeof(wlan_rssiArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(wlan_rssi, wlan_rssiArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

