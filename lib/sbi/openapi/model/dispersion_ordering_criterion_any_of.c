
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dispersion_ordering_criterion_any_of.h"

char* OpenAPI_dispersion_ordering_criterion_any_of_ToString(OpenAPI_dispersion_ordering_criterion_any_of_e dispersion_ordering_criterion_any_of)
{
    const char *dispersion_ordering_criterion_any_ofArray[] =  { "NULL", "TIME_SLOT_START", "DISPERSION", "CLASSIFICATION", "RANKING", "PERCENTILE_RANKING" };
    size_t sizeofArray = sizeof(dispersion_ordering_criterion_any_ofArray) / sizeof(dispersion_ordering_criterion_any_ofArray[0]);
    if (dispersion_ordering_criterion_any_of < sizeofArray)
        return (char *)dispersion_ordering_criterion_any_ofArray[dispersion_ordering_criterion_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_dispersion_ordering_criterion_any_of_e OpenAPI_dispersion_ordering_criterion_any_of_FromString(char* dispersion_ordering_criterion_any_of)
{
    int stringToReturn = 0;
    const char *dispersion_ordering_criterion_any_ofArray[] =  { "NULL", "TIME_SLOT_START", "DISPERSION", "CLASSIFICATION", "RANKING", "PERCENTILE_RANKING" };
    size_t sizeofArray = sizeof(dispersion_ordering_criterion_any_ofArray) / sizeof(dispersion_ordering_criterion_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(dispersion_ordering_criterion_any_of, dispersion_ordering_criterion_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

