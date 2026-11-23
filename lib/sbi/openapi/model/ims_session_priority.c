
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ims_session_priority.h"

char* OpenAPI_ims_session_priority_ToString(OpenAPI_ims_session_priority_e ims_session_priority)
{
    const char *ims_session_priorityArray[] =  { "NULL", "PRIORITY_0", "PRIORITY_1", "PRIORITY_2", "PRIORITY_3", "PRIORITY_4" };
    size_t sizeofArray = sizeof(ims_session_priorityArray) / sizeof(ims_session_priorityArray[0]);
    if (ims_session_priority < sizeofArray)
        return (char *)ims_session_priorityArray[ims_session_priority];
    else
        return (char *)"Unknown";
}

OpenAPI_ims_session_priority_e OpenAPI_ims_session_priority_FromString(char* ims_session_priority)
{
    int stringToReturn = 0;
    const char *ims_session_priorityArray[] =  { "NULL", "PRIORITY_0", "PRIORITY_1", "PRIORITY_2", "PRIORITY_3", "PRIORITY_4" };
    size_t sizeofArray = sizeof(ims_session_priorityArray) / sizeof(ims_session_priorityArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(ims_session_priority, ims_session_priorityArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

