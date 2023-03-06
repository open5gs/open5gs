
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "psa_indication.h"

char* OpenAPI_psa_indication_ToString(OpenAPI_psa_indication_e psa_indication)
{
    const char *psa_indicationArray[] =  { "NULL", "PSA_INSERTED", "PSA_REMOVED", "PSA_INSERTED_ONLY", "PSA_REMOVED_ONLY" };
    size_t sizeofArray = sizeof(psa_indicationArray) / sizeof(psa_indicationArray[0]);
    if (psa_indication < sizeofArray)
        return (char *)psa_indicationArray[psa_indication];
    else
        return (char *)"Unknown";
}

OpenAPI_psa_indication_e OpenAPI_psa_indication_FromString(char* psa_indication)
{
    int stringToReturn = 0;
    const char *psa_indicationArray[] =  { "NULL", "PSA_INSERTED", "PSA_REMOVED", "PSA_INSERTED_ONLY", "PSA_REMOVED_ONLY" };
    size_t sizeofArray = sizeof(psa_indicationArray) / sizeof(psa_indicationArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(psa_indication, psa_indicationArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

