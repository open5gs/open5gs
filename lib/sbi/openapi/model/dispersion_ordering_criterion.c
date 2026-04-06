
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dispersion_ordering_criterion.h"

char* OpenAPI_dispersion_ordering_criterion_ToString(OpenAPI_dispersion_ordering_criterion_e dispersion_ordering_criterion)
{
    const char *dispersion_ordering_criterionArray[] =  { "NULL", "TIME_SLOT_START", "DISPERSION", "CLASSIFICATION", "RANKING", "PERCENTILE_RANKING" };
    size_t sizeofArray = sizeof(dispersion_ordering_criterionArray) / sizeof(dispersion_ordering_criterionArray[0]);
    if (dispersion_ordering_criterion < sizeofArray)
        return (char *)dispersion_ordering_criterionArray[dispersion_ordering_criterion];
    else
        return (char *)"Unknown";
}

OpenAPI_dispersion_ordering_criterion_e OpenAPI_dispersion_ordering_criterion_FromString(char* dispersion_ordering_criterion)
{
    int stringToReturn = 0;
    const char *dispersion_ordering_criterionArray[] =  { "NULL", "TIME_SLOT_START", "DISPERSION", "CLASSIFICATION", "RANKING", "PERCENTILE_RANKING" };
    size_t sizeofArray = sizeof(dispersion_ordering_criterionArray) / sizeof(dispersion_ordering_criterionArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(dispersion_ordering_criterion, dispersion_ordering_criterionArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

