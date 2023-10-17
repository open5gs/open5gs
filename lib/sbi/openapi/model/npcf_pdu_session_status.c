
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "npcf_pdu_session_status.h"

char* OpenAPI_npcf_pdu_session_status_ToString(OpenAPI_npcf_pdu_session_status_e npcf_pdu_session_status)
{
    const char *npcf_pdu_session_statusArray[] =  { "NULL", "ESTABLISHED", "TERMINATED" };
    size_t sizeofArray = sizeof(npcf_pdu_session_statusArray) / sizeof(npcf_pdu_session_statusArray[0]);
    if (npcf_pdu_session_status < sizeofArray)
        return (char *)npcf_pdu_session_statusArray[npcf_pdu_session_status];
    else
        return (char *)"Unknown";
}

OpenAPI_npcf_pdu_session_status_e OpenAPI_npcf_pdu_session_status_FromString(char* npcf_pdu_session_status)
{
    int stringToReturn = 0;
    const char *npcf_pdu_session_statusArray[] =  { "NULL", "ESTABLISHED", "TERMINATED" };
    size_t sizeofArray = sizeof(npcf_pdu_session_statusArray) / sizeof(npcf_pdu_session_statusArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(npcf_pdu_session_status, npcf_pdu_session_statusArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

