
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "value_expression.h"

char* OpenAPI_value_expression_ToString(OpenAPI_value_expression_e value_expression)
{
    const char *value_expressionArray[] =  { "NULL", "AVERAGE", "PEAK" };
    size_t sizeofArray = sizeof(value_expressionArray) / sizeof(value_expressionArray[0]);
    if (value_expression < sizeofArray)
        return (char *)value_expressionArray[value_expression];
    else
        return (char *)"Unknown";
}

OpenAPI_value_expression_e OpenAPI_value_expression_FromString(char* value_expression)
{
    int stringToReturn = 0;
    const char *value_expressionArray[] =  { "NULL", "AVERAGE", "PEAK" };
    size_t sizeofArray = sizeof(value_expressionArray) / sizeof(value_expressionArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(value_expression, value_expressionArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

