
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "wlan_ordering_criterion_any_of.h"

char* OpenAPI_wlan_ordering_criterion_any_of_ToString(OpenAPI_wlan_ordering_criterion_any_of_e wlan_ordering_criterion_any_of)
{
    const char *wlan_ordering_criterion_any_ofArray[] =  { "NULL", "TIME_SLOT_START", "NUMBER_OF_UES", "RSSI", "RTT", "TRAFFIC_INFO" };
    size_t sizeofArray = sizeof(wlan_ordering_criterion_any_ofArray) / sizeof(wlan_ordering_criterion_any_ofArray[0]);
    if (wlan_ordering_criterion_any_of < sizeofArray)
        return (char *)wlan_ordering_criterion_any_ofArray[wlan_ordering_criterion_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_wlan_ordering_criterion_any_of_e OpenAPI_wlan_ordering_criterion_any_of_FromString(char* wlan_ordering_criterion_any_of)
{
    int stringToReturn = 0;
    const char *wlan_ordering_criterion_any_ofArray[] =  { "NULL", "TIME_SLOT_START", "NUMBER_OF_UES", "RSSI", "RTT", "TRAFFIC_INFO" };
    size_t sizeofArray = sizeof(wlan_ordering_criterion_any_ofArray) / sizeof(wlan_ordering_criterion_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(wlan_ordering_criterion_any_of, wlan_ordering_criterion_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

