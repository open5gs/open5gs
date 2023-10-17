
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "expected_analytics_type_any_of.h"

char* OpenAPI_expected_analytics_type_any_of_ToString(OpenAPI_expected_analytics_type_any_of_e expected_analytics_type_any_of)
{
    const char *expected_analytics_type_any_ofArray[] =  { "NULL", "MOBILITY", "COMMUN", "MOBILITY_AND_COMMUN" };
    size_t sizeofArray = sizeof(expected_analytics_type_any_ofArray) / sizeof(expected_analytics_type_any_ofArray[0]);
    if (expected_analytics_type_any_of < sizeofArray)
        return (char *)expected_analytics_type_any_ofArray[expected_analytics_type_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_expected_analytics_type_any_of_e OpenAPI_expected_analytics_type_any_of_FromString(char* expected_analytics_type_any_of)
{
    int stringToReturn = 0;
    const char *expected_analytics_type_any_ofArray[] =  { "NULL", "MOBILITY", "COMMUN", "MOBILITY_AND_COMMUN" };
    size_t sizeofArray = sizeof(expected_analytics_type_any_ofArray) / sizeof(expected_analytics_type_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(expected_analytics_type_any_of, expected_analytics_type_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

