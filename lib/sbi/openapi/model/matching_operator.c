
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "matching_operator.h"

char* OpenAPI_matching_operator_ToString(OpenAPI_matching_operator_e matching_operator)
{
    const char *matching_operatorArray[] =  { "NULL", "FULL_MATCH", "MATCH_ALL", "STARTS_WITH", "NOT_START_WITH", "ENDS_WITH", "NOT_END_WITH", "CONTAINS", "NOT_CONTAIN" };
    size_t sizeofArray = sizeof(matching_operatorArray) / sizeof(matching_operatorArray[0]);
    if (matching_operator < sizeofArray)
        return (char *)matching_operatorArray[matching_operator];
    else
        return (char *)"Unknown";
}

OpenAPI_matching_operator_e OpenAPI_matching_operator_FromString(char* matching_operator)
{
    int stringToReturn = 0;
    const char *matching_operatorArray[] =  { "NULL", "FULL_MATCH", "MATCH_ALL", "STARTS_WITH", "NOT_START_WITH", "ENDS_WITH", "NOT_END_WITH", "CONTAINS", "NOT_CONTAIN" };
    size_t sizeofArray = sizeof(matching_operatorArray) / sizeof(matching_operatorArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(matching_operator, matching_operatorArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

