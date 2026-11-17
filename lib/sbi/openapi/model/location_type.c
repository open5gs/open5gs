
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "location_type.h"

char* OpenAPI_location_type_ToString(OpenAPI_location_type_e location_type)
{
    const char *location_typeArray[] =  { "NULL", "CURRENT_LOCATION", "LAST_KNOWN_LOCATION", "INITIAL_LOCATION", "DEFERRED_LOCATION", "NOTIFICATION_VERIFICATION" };
    size_t sizeofArray = sizeof(location_typeArray) / sizeof(location_typeArray[0]);
    if (location_type < sizeofArray)
        return (char *)location_typeArray[location_type];
    else
        return (char *)"Unknown";
}

OpenAPI_location_type_e OpenAPI_location_type_FromString(char* location_type)
{
    int stringToReturn = 0;
    const char *location_typeArray[] =  { "NULL", "CURRENT_LOCATION", "LAST_KNOWN_LOCATION", "INITIAL_LOCATION", "DEFERRED_LOCATION", "NOTIFICATION_VERIFICATION" };
    size_t sizeofArray = sizeof(location_typeArray) / sizeof(location_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(location_type, location_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

