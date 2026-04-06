
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "area_usage_ind.h"

char* OpenAPI_area_usage_ind_ToString(OpenAPI_area_usage_ind_e area_usage_ind)
{
    const char *area_usage_indArray[] =  { "NULL", "INSIDE_REPORTING", "OUTSIDE_REPORTING" };
    size_t sizeofArray = sizeof(area_usage_indArray) / sizeof(area_usage_indArray[0]);
    if (area_usage_ind < sizeofArray)
        return (char *)area_usage_indArray[area_usage_ind];
    else
        return (char *)"Unknown";
}

OpenAPI_area_usage_ind_e OpenAPI_area_usage_ind_FromString(char* area_usage_ind)
{
    int stringToReturn = 0;
    const char *area_usage_indArray[] =  { "NULL", "INSIDE_REPORTING", "OUTSIDE_REPORTING" };
    size_t sizeofArray = sizeof(area_usage_indArray) / sizeof(area_usage_indArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(area_usage_ind, area_usage_indArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

