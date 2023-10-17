
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "change_type.h"

char* OpenAPI_change_type_ToString(OpenAPI_change_type_e change_type)
{
    const char *change_typeArray[] =  { "NULL", "ADD", "MOVE", "REMOVE", "REPLACE" };
    size_t sizeofArray = sizeof(change_typeArray) / sizeof(change_typeArray[0]);
    if (change_type < sizeofArray)
        return (char *)change_typeArray[change_type];
    else
        return (char *)"Unknown";
}

OpenAPI_change_type_e OpenAPI_change_type_FromString(char* change_type)
{
    int stringToReturn = 0;
    const char *change_typeArray[] =  { "NULL", "ADD", "MOVE", "REMOVE", "REPLACE" };
    size_t sizeofArray = sizeof(change_typeArray) / sizeof(change_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(change_type, change_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

