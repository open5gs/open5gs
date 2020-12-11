
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "stationary_indication.h"

char* OpenAPI_stationary_indication_ToString(OpenAPI_stationary_indication_e stationary_indication)
{
    const char *stationary_indicationArray[] =  { "NULL", "STATIONARY", "MOBILE" };
    size_t sizeofArray = sizeof(stationary_indicationArray) / sizeof(stationary_indicationArray[0]);
    if (stationary_indication < sizeofArray)
        return (char *)stationary_indicationArray[stationary_indication];
    else
        return (char *)"Unknown";
}

OpenAPI_stationary_indication_e OpenAPI_stationary_indication_FromString(char* stationary_indication)
{
    int stringToReturn = 0;
    const char *stationary_indicationArray[] =  { "NULL", "STATIONARY", "MOBILE" };
    size_t sizeofArray = sizeof(stationary_indicationArray) / sizeof(stationary_indicationArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(stationary_indication, stationary_indicationArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

