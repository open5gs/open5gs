
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sm_priority.h"

char* OpenAPI_sm_priority_ToString(OpenAPI_sm_priority_e sm_priority)
{
    const char *sm_priorityArray[] =  { "NULL", "LOW", "NORMAL", "HIGH" };
    size_t sizeofArray = sizeof(sm_priorityArray) / sizeof(sm_priorityArray[0]);
    if (sm_priority < sizeofArray)
        return (char *)sm_priorityArray[sm_priority];
    else
        return (char *)"Unknown";
}

OpenAPI_sm_priority_e OpenAPI_sm_priority_FromString(char* sm_priority)
{
    int stringToReturn = 0;
    const char *sm_priorityArray[] =  { "NULL", "LOW", "NORMAL", "HIGH" };
    size_t sizeofArray = sizeof(sm_priorityArray) / sizeof(sm_priorityArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(sm_priority, sm_priorityArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

