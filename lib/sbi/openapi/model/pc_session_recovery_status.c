
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pc_session_recovery_status.h"

char* OpenAPI_pc_session_recovery_status_ToString(OpenAPI_pc_session_recovery_status_e pc_session_recovery_status)
{
    const char *pc_session_recovery_statusArray[] =  { "NULL", "REST_REQUEST", "REST_TRIGGERED", "REST_NOT_TRIGGERED", "NOT_FOUND" };
    size_t sizeofArray = sizeof(pc_session_recovery_statusArray) / sizeof(pc_session_recovery_statusArray[0]);
    if (pc_session_recovery_status < sizeofArray)
        return (char *)pc_session_recovery_statusArray[pc_session_recovery_status];
    else
        return (char *)"Unknown";
}

OpenAPI_pc_session_recovery_status_e OpenAPI_pc_session_recovery_status_FromString(char* pc_session_recovery_status)
{
    int stringToReturn = 0;
    const char *pc_session_recovery_statusArray[] =  { "NULL", "REST_REQUEST", "REST_TRIGGERED", "REST_NOT_TRIGGERED", "NOT_FOUND" };
    size_t sizeofArray = sizeof(pc_session_recovery_statusArray) / sizeof(pc_session_recovery_statusArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(pc_session_recovery_status, pc_session_recovery_statusArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

