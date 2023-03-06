
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "bsf_event.h"

char* OpenAPI_bsf_event_ToString(OpenAPI_bsf_event_e bsf_event)
{
    const char *bsf_eventArray[] =  { "NULL", "PCF_PDU_SESSION_BINDING_REGISTRATION", "PCF_PDU_SESSION_BINDING_DEREGISTRATION", "PCF_UE_BINDING_REGISTRATION", "PCF_UE_BINDING_DEREGISTRATION", "SNSSAI_DNN_BINDING_REGISTRATION", "SNSSAI_DNN_BINDING_DEREGISTRATION" };
    size_t sizeofArray = sizeof(bsf_eventArray) / sizeof(bsf_eventArray[0]);
    if (bsf_event < sizeofArray)
        return (char *)bsf_eventArray[bsf_event];
    else
        return (char *)"Unknown";
}

OpenAPI_bsf_event_e OpenAPI_bsf_event_FromString(char* bsf_event)
{
    int stringToReturn = 0;
    const char *bsf_eventArray[] =  { "NULL", "PCF_PDU_SESSION_BINDING_REGISTRATION", "PCF_PDU_SESSION_BINDING_DEREGISTRATION", "PCF_UE_BINDING_REGISTRATION", "PCF_UE_BINDING_DEREGISTRATION", "SNSSAI_DNN_BINDING_REGISTRATION", "SNSSAI_DNN_BINDING_DEREGISTRATION" };
    size_t sizeofArray = sizeof(bsf_eventArray) / sizeof(bsf_eventArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(bsf_event, bsf_eventArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

