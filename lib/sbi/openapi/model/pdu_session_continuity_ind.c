
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pdu_session_continuity_ind.h"

char* OpenAPI_pdu_session_continuity_ind_ToString(OpenAPI_pdu_session_continuity_ind_e pdu_session_continuity_ind)
{
    const char *pdu_session_continuity_indArray[] =  { "NULL", "MAINTAIN_PDUSESSION", "RECONNECT_PDUSESSION", "RELEASE_PDUSESSION" };
    size_t sizeofArray = sizeof(pdu_session_continuity_indArray) / sizeof(pdu_session_continuity_indArray[0]);
    if (pdu_session_continuity_ind < sizeofArray)
        return (char *)pdu_session_continuity_indArray[pdu_session_continuity_ind];
    else
        return (char *)"Unknown";
}

OpenAPI_pdu_session_continuity_ind_e OpenAPI_pdu_session_continuity_ind_FromString(char* pdu_session_continuity_ind)
{
    int stringToReturn = 0;
    const char *pdu_session_continuity_indArray[] =  { "NULL", "MAINTAIN_PDUSESSION", "RECONNECT_PDUSESSION", "RELEASE_PDUSESSION" };
    size_t sizeofArray = sizeof(pdu_session_continuity_indArray) / sizeof(pdu_session_continuity_indArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(pdu_session_continuity_ind, pdu_session_continuity_indArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

