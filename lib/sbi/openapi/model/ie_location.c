
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ie_location.h"

char* OpenAPI_ie_location_ToString(OpenAPI_ie_location_e ie_location)
{
    const char *ie_locationArray[] =  { "NULL", "URI_PARAM", "HEADER", "BODY", "MULTIPART_BINARY" };
    size_t sizeofArray = sizeof(ie_locationArray) / sizeof(ie_locationArray[0]);
    if (ie_location < sizeofArray)
        return (char *)ie_locationArray[ie_location];
    else
        return (char *)"Unknown";
}

OpenAPI_ie_location_e OpenAPI_ie_location_FromString(char* ie_location)
{
    int stringToReturn = 0;
    const char *ie_locationArray[] =  { "NULL", "URI_PARAM", "HEADER", "BODY", "MULTIPART_BINARY" };
    size_t sizeofArray = sizeof(ie_locationArray) / sizeof(ie_locationArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(ie_location, ie_locationArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

