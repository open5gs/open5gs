
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ie_location_any_of.h"

char* OpenAPI_ie_location_any_of_ToString(OpenAPI_ie_location_any_of_e ie_location_any_of)
{
    const char *ie_location_any_ofArray[] =  { "NULL", "URI_PARAM", "HEADER", "BODY", "MULTIPART_BINARY" };
    size_t sizeofArray = sizeof(ie_location_any_ofArray) / sizeof(ie_location_any_ofArray[0]);
    if (ie_location_any_of < sizeofArray)
        return (char *)ie_location_any_ofArray[ie_location_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_ie_location_any_of_e OpenAPI_ie_location_any_of_FromString(char* ie_location_any_of)
{
    int stringToReturn = 0;
    const char *ie_location_any_ofArray[] =  { "NULL", "URI_PARAM", "HEADER", "BODY", "MULTIPART_BINARY" };
    size_t sizeofArray = sizeof(ie_location_any_ofArray) / sizeof(ie_location_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(ie_location_any_of, ie_location_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

