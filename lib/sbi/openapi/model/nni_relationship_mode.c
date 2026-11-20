
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nni_relationship_mode.h"

char* OpenAPI_nni_relationship_mode_ToString(OpenAPI_nni_relationship_mode_e nni_relationship_mode)
{
    const char *nni_relationship_modeArray[] =  { "NULL", "TRUSTED", "NON_TRUSTED" };
    size_t sizeofArray = sizeof(nni_relationship_modeArray) / sizeof(nni_relationship_modeArray[0]);
    if (nni_relationship_mode < sizeofArray)
        return (char *)nni_relationship_modeArray[nni_relationship_mode];
    else
        return (char *)"Unknown";
}

OpenAPI_nni_relationship_mode_e OpenAPI_nni_relationship_mode_FromString(char* nni_relationship_mode)
{
    int stringToReturn = 0;
    const char *nni_relationship_modeArray[] =  { "NULL", "TRUSTED", "NON_TRUSTED" };
    size_t sizeofArray = sizeof(nni_relationship_modeArray) / sizeof(nni_relationship_modeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(nni_relationship_mode, nni_relationship_modeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

