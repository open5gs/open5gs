
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "header_handling_action.h"

char* OpenAPI_header_handling_action_ToString(OpenAPI_header_handling_action_e header_handling_action)
{
    const char *header_handling_actionArray[] =  { "NULL", "DETECT", "REMOVE", "REPLACE", "INSERT" };
    size_t sizeofArray = sizeof(header_handling_actionArray) / sizeof(header_handling_actionArray[0]);
    if (header_handling_action < sizeofArray)
        return (char *)header_handling_actionArray[header_handling_action];
    else
        return (char *)"Unknown";
}

OpenAPI_header_handling_action_e OpenAPI_header_handling_action_FromString(char* header_handling_action)
{
    int stringToReturn = 0;
    const char *header_handling_actionArray[] =  { "NULL", "DETECT", "REMOVE", "REPLACE", "INSERT" };
    size_t sizeofArray = sizeof(header_handling_actionArray) / sizeof(header_handling_actionArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(header_handling_action, header_handling_actionArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

