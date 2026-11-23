
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "allocate_unit_indicator.h"

char* OpenAPI_allocate_unit_indicator_ToString(OpenAPI_allocate_unit_indicator_e allocate_unit_indicator)
{
    const char *allocate_unit_indicatorArray[] =  { "NULL", "CHF_DETERMINED", "CTF_DETERMINED" };
    size_t sizeofArray = sizeof(allocate_unit_indicatorArray) / sizeof(allocate_unit_indicatorArray[0]);
    if (allocate_unit_indicator < sizeofArray)
        return (char *)allocate_unit_indicatorArray[allocate_unit_indicator];
    else
        return (char *)"Unknown";
}

OpenAPI_allocate_unit_indicator_e OpenAPI_allocate_unit_indicator_FromString(char* allocate_unit_indicator)
{
    int stringToReturn = 0;
    const char *allocate_unit_indicatorArray[] =  { "NULL", "CHF_DETERMINED", "CTF_DETERMINED" };
    size_t sizeofArray = sizeof(allocate_unit_indicatorArray) / sizeof(allocate_unit_indicatorArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(allocate_unit_indicator, allocate_unit_indicatorArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

