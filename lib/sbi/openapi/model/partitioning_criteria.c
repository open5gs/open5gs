
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "partitioning_criteria.h"

char* OpenAPI_partitioning_criteria_ToString(OpenAPI_partitioning_criteria_e partitioning_criteria)
{
    const char *partitioning_criteriaArray[] =  { "NULL", "TAC", "SUBPLMN", "GEOAREA", "SNSSAI", "DNN" };
    size_t sizeofArray = sizeof(partitioning_criteriaArray) / sizeof(partitioning_criteriaArray[0]);
    if (partitioning_criteria < sizeofArray)
        return (char *)partitioning_criteriaArray[partitioning_criteria];
    else
        return (char *)"Unknown";
}

OpenAPI_partitioning_criteria_e OpenAPI_partitioning_criteria_FromString(char* partitioning_criteria)
{
    int stringToReturn = 0;
    const char *partitioning_criteriaArray[] =  { "NULL", "TAC", "SUBPLMN", "GEOAREA", "SNSSAI", "DNN" };
    size_t sizeofArray = sizeof(partitioning_criteriaArray) / sizeof(partitioning_criteriaArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(partitioning_criteria, partitioning_criteriaArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

