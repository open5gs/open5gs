
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mico_mode_indication.h"

char* OpenAPI_mico_mode_indication_ToString(OpenAPI_mico_mode_indication_e mico_mode_indication)
{
    const char *mico_mode_indicationArray[] =  { "NULL", "MICO_MODE", "NO_MICO_MODE" };
    size_t sizeofArray = sizeof(mico_mode_indicationArray) / sizeof(mico_mode_indicationArray[0]);
    if (mico_mode_indication < sizeofArray)
        return (char *)mico_mode_indicationArray[mico_mode_indication];
    else
        return (char *)"Unknown";
}

OpenAPI_mico_mode_indication_e OpenAPI_mico_mode_indication_FromString(char* mico_mode_indication)
{
    int stringToReturn = 0;
    const char *mico_mode_indicationArray[] =  { "NULL", "MICO_MODE", "NO_MICO_MODE" };
    size_t sizeofArray = sizeof(mico_mode_indicationArray) / sizeof(mico_mode_indicationArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(mico_mode_indication, mico_mode_indicationArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

