
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "proximity_criterion.h"

char* OpenAPI_proximity_criterion_ToString(OpenAPI_proximity_criterion_e proximity_criterion)
{
    const char *proximity_criterionArray[] =  { "NULL", "VELOCITY", "AVG_SPD", "ORIENTATION", "TRAJECTORY" };
    size_t sizeofArray = sizeof(proximity_criterionArray) / sizeof(proximity_criterionArray[0]);
    if (proximity_criterion < sizeofArray)
        return (char *)proximity_criterionArray[proximity_criterion];
    else
        return (char *)"Unknown";
}

OpenAPI_proximity_criterion_e OpenAPI_proximity_criterion_FromString(char* proximity_criterion)
{
    int stringToReturn = 0;
    const char *proximity_criterionArray[] =  { "NULL", "VELOCITY", "AVG_SPD", "ORIENTATION", "TRAJECTORY" };
    size_t sizeofArray = sizeof(proximity_criterionArray) / sizeof(proximity_criterionArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(proximity_criterion, proximity_criterionArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

