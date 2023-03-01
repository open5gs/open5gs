
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pdu_session_context_type.h"

char* OpenAPI_pdu_session_context_type_ToString(OpenAPI_pdu_session_context_type_e pdu_session_context_type)
{
    const char *pdu_session_context_typeArray[] =  { "NULL", "AF_COORDINATION_INFO" };
    size_t sizeofArray = sizeof(pdu_session_context_typeArray) / sizeof(pdu_session_context_typeArray[0]);
    if (pdu_session_context_type < sizeofArray)
        return (char *)pdu_session_context_typeArray[pdu_session_context_type];
    else
        return (char *)"Unknown";
}

OpenAPI_pdu_session_context_type_e OpenAPI_pdu_session_context_type_FromString(char* pdu_session_context_type)
{
    int stringToReturn = 0;
    const char *pdu_session_context_typeArray[] =  { "NULL", "AF_COORDINATION_INFO" };
    size_t sizeofArray = sizeof(pdu_session_context_typeArray) / sizeof(pdu_session_context_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(pdu_session_context_type, pdu_session_context_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

