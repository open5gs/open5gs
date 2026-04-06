
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "establishment_rejection_cause.h"

char* OpenAPI_establishment_rejection_cause_ToString(OpenAPI_establishment_rejection_cause_e establishment_rejection_cause)
{
    const char *establishment_rejection_causeArray[] =  { "NULL", "OPERATOR_DETERMINED_BARRING" };
    size_t sizeofArray = sizeof(establishment_rejection_causeArray) / sizeof(establishment_rejection_causeArray[0]);
    if (establishment_rejection_cause < sizeofArray)
        return (char *)establishment_rejection_causeArray[establishment_rejection_cause];
    else
        return (char *)"Unknown";
}

OpenAPI_establishment_rejection_cause_e OpenAPI_establishment_rejection_cause_FromString(char* establishment_rejection_cause)
{
    int stringToReturn = 0;
    const char *establishment_rejection_causeArray[] =  { "NULL", "OPERATOR_DETERMINED_BARRING" };
    size_t sizeofArray = sizeof(establishment_rejection_causeArray) / sizeof(establishment_rejection_causeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(establishment_rejection_cause, establishment_rejection_causeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

