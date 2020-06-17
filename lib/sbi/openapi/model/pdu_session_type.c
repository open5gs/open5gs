
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pdu_session_type.h"

char* OpenAPI_pdu_session_type_ToString(OpenAPI_pdu_session_type_e pdu_session_type)
{
    const char *pdu_session_typeArray[] =  { "NULL", "IPV4", "IPV6", "IPV4V6", "UNSTRUCTURED", "ETHERNET" };
    size_t sizeofArray = sizeof(pdu_session_typeArray) / sizeof(pdu_session_typeArray[0]);
    if (pdu_session_type < sizeofArray)
        return (char *)pdu_session_typeArray[pdu_session_type];
    else
        return (char *)"Unknown";
}

OpenAPI_pdu_session_type_e OpenAPI_pdu_session_type_FromString(char* pdu_session_type)
{
    int stringToReturn = 0;
    const char *pdu_session_typeArray[] =  { "NULL", "IPV4", "IPV6", "IPV4V6", "UNSTRUCTURED", "ETHERNET" };
    size_t sizeofArray = sizeof(pdu_session_typeArray) / sizeof(pdu_session_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(pdu_session_type, pdu_session_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

