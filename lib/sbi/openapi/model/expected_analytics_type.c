
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "expected_analytics_type.h"

char* OpenAPI_expected_analytics_type_ToString(OpenAPI_expected_analytics_type_e expected_analytics_type)
{
    const char *expected_analytics_typeArray[] =  { "NULL", "MOBILITY", "COMMUN", "MOBILITY_AND_COMMUN" };
    size_t sizeofArray = sizeof(expected_analytics_typeArray) / sizeof(expected_analytics_typeArray[0]);
    if (expected_analytics_type < sizeofArray)
        return (char *)expected_analytics_typeArray[expected_analytics_type];
    else
        return (char *)"Unknown";
}

OpenAPI_expected_analytics_type_e OpenAPI_expected_analytics_type_FromString(char* expected_analytics_type)
{
    int stringToReturn = 0;
    const char *expected_analytics_typeArray[] =  { "NULL", "MOBILITY", "COMMUN", "MOBILITY_AND_COMMUN" };
    size_t sizeofArray = sizeof(expected_analytics_typeArray) / sizeof(expected_analytics_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(expected_analytics_type, expected_analytics_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

