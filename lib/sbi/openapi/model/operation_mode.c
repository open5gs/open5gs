
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "operation_mode.h"

char* OpenAPI_operation_mode_ToString(OpenAPI_operation_mode_e operation_mode)
{
    const char *operation_modeArray[] =  { "NULL", "WB_S1", "NB_S1", "WB_N1", "NB_N1" };
    size_t sizeofArray = sizeof(operation_modeArray) / sizeof(operation_modeArray[0]);
    if (operation_mode < sizeofArray)
        return (char *)operation_modeArray[operation_mode];
    else
        return (char *)"Unknown";
}

OpenAPI_operation_mode_e OpenAPI_operation_mode_FromString(char* operation_mode)
{
    int stringToReturn = 0;
    const char *operation_modeArray[] =  { "NULL", "WB_S1", "NB_S1", "WB_N1", "NB_N1" };
    size_t sizeofArray = sizeof(operation_modeArray) / sizeof(operation_modeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(operation_mode, operation_modeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

