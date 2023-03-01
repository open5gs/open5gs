
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pdu_session_status.h"

char* OpenAPI_pdu_session_status_ToString(OpenAPI_pdu_session_status_e pdu_session_status)
{
    const char *pdu_session_statusArray[] =  { "NULL", "ACTIVE", "RELEASED" };
    size_t sizeofArray = sizeof(pdu_session_statusArray) / sizeof(pdu_session_statusArray[0]);
    if (pdu_session_status < sizeofArray)
        return (char *)pdu_session_statusArray[pdu_session_status];
    else
        return (char *)"Unknown";
}

OpenAPI_pdu_session_status_e OpenAPI_pdu_session_status_FromString(char* pdu_session_status)
{
    int stringToReturn = 0;
    const char *pdu_session_statusArray[] =  { "NULL", "ACTIVE", "RELEASED" };
    size_t sizeofArray = sizeof(pdu_session_statusArray) / sizeof(pdu_session_statusArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(pdu_session_status, pdu_session_statusArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

