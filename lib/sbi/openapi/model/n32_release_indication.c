
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "n32_release_indication.h"

char* OpenAPI_n32_release_indication_ToString(OpenAPI_n32_release_indication_e n32_release_indication)
{
    const char *n32_release_indicationArray[] =  { "NULL", "RELEASE_REESTABLISHMENT_ALLOWED", "RELEASE_REESTABLISHMENT_NOT_ALLOWED", "REESTABLISH" };
    size_t sizeofArray = sizeof(n32_release_indicationArray) / sizeof(n32_release_indicationArray[0]);
    if (n32_release_indication < sizeofArray)
        return (char *)n32_release_indicationArray[n32_release_indication];
    else
        return (char *)"Unknown";
}

OpenAPI_n32_release_indication_e OpenAPI_n32_release_indication_FromString(char* n32_release_indication)
{
    int stringToReturn = 0;
    const char *n32_release_indicationArray[] =  { "NULL", "RELEASE_REESTABLISHMENT_ALLOWED", "RELEASE_REESTABLISHMENT_NOT_ALLOWED", "REESTABLISH" };
    size_t sizeofArray = sizeof(n32_release_indicationArray) / sizeof(n32_release_indicationArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(n32_release_indication, n32_release_indicationArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

