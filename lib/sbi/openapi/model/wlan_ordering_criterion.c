
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "wlan_ordering_criterion.h"

char* OpenAPI_wlan_ordering_criterion_ToString(OpenAPI_wlan_ordering_criterion_e wlan_ordering_criterion)
{
    const char *wlan_ordering_criterionArray[] =  { "NULL", "TIME_SLOT_START", "NUMBER_OF_UES", "RSSI", "RTT", "TRAFFIC_INFO" };
    size_t sizeofArray = sizeof(wlan_ordering_criterionArray) / sizeof(wlan_ordering_criterionArray[0]);
    if (wlan_ordering_criterion < sizeofArray)
        return (char *)wlan_ordering_criterionArray[wlan_ordering_criterion];
    else
        return (char *)"Unknown";
}

OpenAPI_wlan_ordering_criterion_e OpenAPI_wlan_ordering_criterion_FromString(char* wlan_ordering_criterion)
{
    int stringToReturn = 0;
    const char *wlan_ordering_criterionArray[] =  { "NULL", "TIME_SLOT_START", "NUMBER_OF_UES", "RSSI", "RTT", "TRAFFIC_INFO" };
    size_t sizeofArray = sizeof(wlan_ordering_criterionArray) / sizeof(wlan_ordering_criterionArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(wlan_ordering_criterion, wlan_ordering_criterionArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

