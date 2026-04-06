
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "wlan_rtt.h"

char* OpenAPI_wlan_rtt_ToString(OpenAPI_wlan_rtt_e wlan_rtt)
{
    const char *wlan_rttArray[] =  { "NULL", "true" };
    size_t sizeofArray = sizeof(wlan_rttArray) / sizeof(wlan_rttArray[0]);
    if (wlan_rtt < sizeofArray)
        return (char *)wlan_rttArray[wlan_rtt];
    else
        return (char *)"Unknown";
}

OpenAPI_wlan_rtt_e OpenAPI_wlan_rtt_FromString(char* wlan_rtt)
{
    int stringToReturn = 0;
    const char *wlan_rttArray[] =  { "NULL", "true" };
    size_t sizeofArray = sizeof(wlan_rttArray) / sizeof(wlan_rttArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(wlan_rtt, wlan_rttArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

