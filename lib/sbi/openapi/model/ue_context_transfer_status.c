
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_context_transfer_status.h"

char* OpenAPI_ue_context_transfer_status_ToString(OpenAPI_ue_context_transfer_status_e ue_context_transfer_status)
{
    const char *ue_context_transfer_statusArray[] =  { "NULL", "TRANSFERRED", "NOT_TRANSFERRED" };
    size_t sizeofArray = sizeof(ue_context_transfer_statusArray) / sizeof(ue_context_transfer_statusArray[0]);
    if (ue_context_transfer_status < sizeofArray)
        return (char *)ue_context_transfer_statusArray[ue_context_transfer_status];
    else
        return (char *)"Unknown";
}

OpenAPI_ue_context_transfer_status_e OpenAPI_ue_context_transfer_status_FromString(char* ue_context_transfer_status)
{
    int stringToReturn = 0;
    const char *ue_context_transfer_statusArray[] =  { "NULL", "TRANSFERRED", "NOT_TRANSFERRED" };
    size_t sizeofArray = sizeof(ue_context_transfer_statusArray) / sizeof(ue_context_transfer_statusArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(ue_context_transfer_status, ue_context_transfer_statusArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

