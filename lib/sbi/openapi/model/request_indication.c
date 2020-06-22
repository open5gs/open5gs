
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "request_indication.h"

char* OpenAPI_request_indication_ToString(OpenAPI_request_indication_e request_indication)
{
    const char *request_indicationArray[] =  { "NULL", "UE_REQ_PDU_SES_MOD", "UE_REQ_PDU_SES_REL", "PDU_SES_MOB", "NW_REQ_PDU_SES_AUTH", "NW_REQ_PDU_SES_MOD", "NW_REQ_PDU_SES_REL", "EBI_ASSIGNMENT_REQ" };
    size_t sizeofArray = sizeof(request_indicationArray) / sizeof(request_indicationArray[0]);
    if (request_indication < sizeofArray)
        return (char *)request_indicationArray[request_indication];
    else
        return (char *)"Unknown";
}

OpenAPI_request_indication_e OpenAPI_request_indication_FromString(char* request_indication)
{
    int stringToReturn = 0;
    const char *request_indicationArray[] =  { "NULL", "UE_REQ_PDU_SES_MOD", "UE_REQ_PDU_SES_REL", "PDU_SES_MOB", "NW_REQ_PDU_SES_AUTH", "NW_REQ_PDU_SES_MOD", "NW_REQ_PDU_SES_REL", "EBI_ASSIGNMENT_REQ" };
    size_t sizeofArray = sizeof(request_indicationArray) / sizeof(request_indicationArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(request_indication, request_indicationArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

