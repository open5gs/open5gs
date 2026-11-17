
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "session_failover.h"

char* OpenAPI_session_failover_ToString(OpenAPI_session_failover_e session_failover)
{
    const char *session_failoverArray[] =  { "NULL", "FAILOVER_NOT_SUPPORTED", "FAILOVER_SUPPORTED" };
    size_t sizeofArray = sizeof(session_failoverArray) / sizeof(session_failoverArray[0]);
    if (session_failover < sizeofArray)
        return (char *)session_failoverArray[session_failover];
    else
        return (char *)"Unknown";
}

OpenAPI_session_failover_e OpenAPI_session_failover_FromString(char* session_failover)
{
    int stringToReturn = 0;
    const char *session_failoverArray[] =  { "NULL", "FAILOVER_NOT_SUPPORTED", "FAILOVER_SUPPORTED" };
    size_t sizeofArray = sizeof(session_failoverArray) / sizeof(session_failoverArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(session_failover, session_failoverArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

