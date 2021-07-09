
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pdu_session_status_any_of.h"

char* OpenAPI_pdu_session_status_any_of_ToString(OpenAPI_pdu_session_status_any_of_e pdu_session_status_any_of)
{
    const char *pdu_session_status_any_ofArray[] =  { "NULL", "ACTIVE", "RELEASED" };
    size_t sizeofArray = sizeof(pdu_session_status_any_ofArray) / sizeof(pdu_session_status_any_ofArray[0]);
    if (pdu_session_status_any_of < sizeofArray)
        return (char *)pdu_session_status_any_ofArray[pdu_session_status_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_pdu_session_status_any_of_e OpenAPI_pdu_session_status_any_of_FromString(char* pdu_session_status_any_of)
{
    int stringToReturn = 0;
    const char *pdu_session_status_any_ofArray[] =  { "NULL", "ACTIVE", "RELEASED" };
    size_t sizeofArray = sizeof(pdu_session_status_any_ofArray) / sizeof(pdu_session_status_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(pdu_session_status_any_of, pdu_session_status_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

