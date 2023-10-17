
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "location_filter_any_of.h"

char* OpenAPI_location_filter_any_of_ToString(OpenAPI_location_filter_any_of_e location_filter_any_of)
{
    const char *location_filter_any_ofArray[] =  { "NULL", "TAI", "CELL_ID", "RAN_NODE", "N3IWF", "UE_IP", "UDP_PORT", "TNAP_ID", "GLI", "TWAP_ID" };
    size_t sizeofArray = sizeof(location_filter_any_ofArray) / sizeof(location_filter_any_ofArray[0]);
    if (location_filter_any_of < sizeofArray)
        return (char *)location_filter_any_ofArray[location_filter_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_location_filter_any_of_e OpenAPI_location_filter_any_of_FromString(char* location_filter_any_of)
{
    int stringToReturn = 0;
    const char *location_filter_any_ofArray[] =  { "NULL", "TAI", "CELL_ID", "RAN_NODE", "N3IWF", "UE_IP", "UDP_PORT", "TNAP_ID", "GLI", "TWAP_ID" };
    size_t sizeofArray = sizeof(location_filter_any_ofArray) / sizeof(location_filter_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(location_filter_any_of, location_filter_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

