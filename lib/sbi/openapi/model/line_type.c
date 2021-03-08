
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "line_type.h"

char* OpenAPI_line_type_ToString(OpenAPI_line_type_e line_type)
{
    const char *line_typeArray[] =  { "NULL", "DSL", "PON" };
    size_t sizeofArray = sizeof(line_typeArray) / sizeof(line_typeArray[0]);
    if (line_type < sizeofArray)
        return (char *)line_typeArray[line_type];
    else
        return (char *)"Unknown";
}

OpenAPI_line_type_e OpenAPI_line_type_FromString(char* line_type)
{
    int stringToReturn = 0;
    const char *line_typeArray[] =  { "NULL", "DSL", "PON" };
    size_t sizeofArray = sizeof(line_typeArray) / sizeof(line_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(line_type, line_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

