
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "unavailable_access_indication.h"

char* OpenAPI_unavailable_access_indication_ToString(OpenAPI_unavailable_access_indication_e unavailable_access_indication)
{
    const char *unavailable_access_indicationArray[] =  { "NULL", "3GA_UNAVAILABLE", "N3GA_UNAVAILABLE" };
    size_t sizeofArray = sizeof(unavailable_access_indicationArray) / sizeof(unavailable_access_indicationArray[0]);
    if (unavailable_access_indication < sizeofArray)
        return (char *)unavailable_access_indicationArray[unavailable_access_indication];
    else
        return (char *)"Unknown";
}

OpenAPI_unavailable_access_indication_e OpenAPI_unavailable_access_indication_FromString(char* unavailable_access_indication)
{
    int stringToReturn = 0;
    const char *unavailable_access_indicationArray[] =  { "NULL", "3GA_UNAVAILABLE", "N3GA_UNAVAILABLE" };
    size_t sizeofArray = sizeof(unavailable_access_indicationArray) / sizeof(unavailable_access_indicationArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(unavailable_access_indication, unavailable_access_indicationArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

