
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "multicast_access_control_any_of.h"

char* OpenAPI_multicast_access_control_any_of_ToString(OpenAPI_multicast_access_control_any_of_e multicast_access_control_any_of)
{
    const char *multicast_access_control_any_ofArray[] =  { "NULL", "ALLOWED", "NOT_ALLOWED" };
    size_t sizeofArray = sizeof(multicast_access_control_any_ofArray) / sizeof(multicast_access_control_any_ofArray[0]);
    if (multicast_access_control_any_of < sizeofArray)
        return (char *)multicast_access_control_any_ofArray[multicast_access_control_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_multicast_access_control_any_of_e OpenAPI_multicast_access_control_any_of_FromString(char* multicast_access_control_any_of)
{
    int stringToReturn = 0;
    const char *multicast_access_control_any_ofArray[] =  { "NULL", "ALLOWED", "NOT_ALLOWED" };
    size_t sizeofArray = sizeof(multicast_access_control_any_ofArray) / sizeof(multicast_access_control_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(multicast_access_control_any_of, multicast_access_control_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

