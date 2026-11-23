
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "quota_consumption_indicator.h"

char* OpenAPI_quota_consumption_indicator_ToString(OpenAPI_quota_consumption_indicator_e quota_consumption_indicator)
{
    const char *quota_consumption_indicatorArray[] =  { "NULL", "QUOTA_NOT_USED", "QUOTA_IS_USED" };
    size_t sizeofArray = sizeof(quota_consumption_indicatorArray) / sizeof(quota_consumption_indicatorArray[0]);
    if (quota_consumption_indicator < sizeofArray)
        return (char *)quota_consumption_indicatorArray[quota_consumption_indicator];
    else
        return (char *)"Unknown";
}

OpenAPI_quota_consumption_indicator_e OpenAPI_quota_consumption_indicator_FromString(char* quota_consumption_indicator)
{
    int stringToReturn = 0;
    const char *quota_consumption_indicatorArray[] =  { "NULL", "QUOTA_NOT_USED", "QUOTA_IS_USED" };
    size_t sizeofArray = sizeof(quota_consumption_indicatorArray) / sizeof(quota_consumption_indicatorArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(quota_consumption_indicator, quota_consumption_indicatorArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

