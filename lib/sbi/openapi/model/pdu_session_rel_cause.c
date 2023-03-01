
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pdu_session_rel_cause.h"

char* OpenAPI_pdu_session_rel_cause_ToString(OpenAPI_pdu_session_rel_cause_e pdu_session_rel_cause)
{
    const char *pdu_session_rel_causeArray[] =  { "NULL", "PS_TO_CS_HO", "RULE_ERROR" };
    size_t sizeofArray = sizeof(pdu_session_rel_causeArray) / sizeof(pdu_session_rel_causeArray[0]);
    if (pdu_session_rel_cause < sizeofArray)
        return (char *)pdu_session_rel_causeArray[pdu_session_rel_cause];
    else
        return (char *)"Unknown";
}

OpenAPI_pdu_session_rel_cause_e OpenAPI_pdu_session_rel_cause_FromString(char* pdu_session_rel_cause)
{
    int stringToReturn = 0;
    const char *pdu_session_rel_causeArray[] =  { "NULL", "PS_TO_CS_HO", "RULE_ERROR" };
    size_t sizeofArray = sizeof(pdu_session_rel_causeArray) / sizeof(pdu_session_rel_causeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(pdu_session_rel_cause, pdu_session_rel_causeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

