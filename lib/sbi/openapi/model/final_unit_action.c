
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "final_unit_action.h"

char* OpenAPI_final_unit_action_ToString(OpenAPI_final_unit_action_e final_unit_action)
{
    const char *final_unit_actionArray[] =  { "NULL", "TERMINATE", "REDIRECT", "RESTRICT_ACCESS" };
    size_t sizeofArray = sizeof(final_unit_actionArray) / sizeof(final_unit_actionArray[0]);
    if (final_unit_action < sizeofArray)
        return (char *)final_unit_actionArray[final_unit_action];
    else
        return (char *)"Unknown";
}

OpenAPI_final_unit_action_e OpenAPI_final_unit_action_FromString(char* final_unit_action)
{
    int stringToReturn = 0;
    const char *final_unit_actionArray[] =  { "NULL", "TERMINATE", "REDIRECT", "RESTRICT_ACCESS" };
    size_t sizeofArray = sizeof(final_unit_actionArray) / sizeof(final_unit_actionArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(final_unit_action, final_unit_actionArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

