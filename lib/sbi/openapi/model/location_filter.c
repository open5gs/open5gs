
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "location_filter.h"

char* OpenAPI_location_filter_ToString(OpenAPI_location_filter_e location_filter)
{
    const char *location_filterArray[] =  { "NULL", "TAI", "CELL_ID", "RAN_NODE", "N3IWF", "UE_IP", "UDP_PORT", "TNAP_ID", "GLI", "TWAP_ID" };
    size_t sizeofArray = sizeof(location_filterArray) / sizeof(location_filterArray[0]);
    if (location_filter < sizeofArray)
        return (char *)location_filterArray[location_filter];
    else
        return (char *)"Unknown";
}

OpenAPI_location_filter_e OpenAPI_location_filter_FromString(char* location_filter)
{
    int stringToReturn = 0;
    const char *location_filterArray[] =  { "NULL", "TAI", "CELL_ID", "RAN_NODE", "N3IWF", "UE_IP", "UDP_PORT", "TNAP_ID", "GLI", "TWAP_ID" };
    size_t sizeofArray = sizeof(location_filterArray) / sizeof(location_filterArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(location_filter, location_filterArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

