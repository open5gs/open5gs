
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "quota_management_indicator.h"

char* OpenAPI_quota_management_indicator_ToString(OpenAPI_quota_management_indicator_e quota_management_indicator)
{
    const char *quota_management_indicatorArray[] =  { "NULL", "ONLINE_CHARGING", "OFFLINE_CHARGING", "QUOTA_MANAGEMENT_SUSPENDED" };
    size_t sizeofArray = sizeof(quota_management_indicatorArray) / sizeof(quota_management_indicatorArray[0]);
    if (quota_management_indicator < sizeofArray)
        return (char *)quota_management_indicatorArray[quota_management_indicator];
    else
        return (char *)"Unknown";
}

OpenAPI_quota_management_indicator_e OpenAPI_quota_management_indicator_FromString(char* quota_management_indicator)
{
    int stringToReturn = 0;
    const char *quota_management_indicatorArray[] =  { "NULL", "ONLINE_CHARGING", "OFFLINE_CHARGING", "QUOTA_MANAGEMENT_SUSPENDED" };
    size_t sizeofArray = sizeof(quota_management_indicatorArray) / sizeof(quota_management_indicatorArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(quota_management_indicator, quota_management_indicatorArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

