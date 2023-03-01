
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "uc_purpose.h"

char* OpenAPI_uc_purpose_ToString(OpenAPI_uc_purpose_e uc_purpose)
{
    const char *uc_purposeArray[] =  { "NULL", "ANALYTICS", "MODEL_TRAINING", "NW_CAP_EXPOSURE", "EDGEAPP_UE_LOCATION" };
    size_t sizeofArray = sizeof(uc_purposeArray) / sizeof(uc_purposeArray[0]);
    if (uc_purpose < sizeofArray)
        return (char *)uc_purposeArray[uc_purpose];
    else
        return (char *)"Unknown";
}

OpenAPI_uc_purpose_e OpenAPI_uc_purpose_FromString(char* uc_purpose)
{
    int stringToReturn = 0;
    const char *uc_purposeArray[] =  { "NULL", "ANALYTICS", "MODEL_TRAINING", "NW_CAP_EXPOSURE", "EDGEAPP_UE_LOCATION" };
    size_t sizeofArray = sizeof(uc_purposeArray) / sizeof(uc_purposeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(uc_purpose, uc_purposeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

