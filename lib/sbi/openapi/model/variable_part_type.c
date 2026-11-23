
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "variable_part_type.h"

char* OpenAPI_variable_part_type_ToString(OpenAPI_variable_part_type_e variable_part_type)
{
    const char *variable_part_typeArray[] =  { "NULL", "INTEGER", "NUMBER", "TIME", "DATE", "CURRENCY" };
    size_t sizeofArray = sizeof(variable_part_typeArray) / sizeof(variable_part_typeArray[0]);
    if (variable_part_type < sizeofArray)
        return (char *)variable_part_typeArray[variable_part_type];
    else
        return (char *)"Unknown";
}

OpenAPI_variable_part_type_e OpenAPI_variable_part_type_FromString(char* variable_part_type)
{
    int stringToReturn = 0;
    const char *variable_part_typeArray[] =  { "NULL", "INTEGER", "NUMBER", "TIME", "DATE", "CURRENCY" };
    size_t sizeofArray = sizeof(variable_part_typeArray) / sizeof(variable_part_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(variable_part_type, variable_part_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

