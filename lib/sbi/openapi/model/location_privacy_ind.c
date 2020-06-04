
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "location_privacy_ind.h"

char* OpenAPI_location_privacy_ind_ToString(OpenAPI_location_privacy_ind_e location_privacy_ind)
{
    const char *location_privacy_indArray[] =  { "NULL", "LOCATION_DISALLOWED", "LOCATION_ALLOWED" };
    size_t sizeofArray = sizeof(location_privacy_indArray) / sizeof(location_privacy_indArray[0]);
    if (location_privacy_ind < sizeofArray)
        return (char *)location_privacy_indArray[location_privacy_ind];
    else
        return (char *)"Unknown";
}

OpenAPI_location_privacy_ind_e OpenAPI_location_privacy_ind_FromString(char* location_privacy_ind)
{
    int stringToReturn = 0;
    const char *location_privacy_indArray[] =  { "NULL", "LOCATION_DISALLOWED", "LOCATION_ALLOWED" };
    size_t sizeofArray = sizeof(location_privacy_indArray) / sizeof(location_privacy_indArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(location_privacy_ind, location_privacy_indArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

