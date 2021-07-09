
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_update_status.h"

char* OpenAPI_ue_update_status_ToString(OpenAPI_ue_update_status_e ue_update_status)
{
    const char *ue_update_statusArray[] =  { "NULL", "NOT_SENT", "SENT_NO_ACK_REQUIRED", "WAITING_FOR_ACK", "ACK_RECEIVED", "NEGATIVE_ACK_RECEIVED" };
    size_t sizeofArray = sizeof(ue_update_statusArray) / sizeof(ue_update_statusArray[0]);
    if (ue_update_status < sizeofArray)
        return (char *)ue_update_statusArray[ue_update_status];
    else
        return (char *)"Unknown";
}

OpenAPI_ue_update_status_e OpenAPI_ue_update_status_FromString(char* ue_update_status)
{
    int stringToReturn = 0;
    const char *ue_update_statusArray[] =  { "NULL", "NOT_SENT", "SENT_NO_ACK_REQUIRED", "WAITING_FOR_ACK", "ACK_RECEIVED", "NEGATIVE_ACK_RECEIVED" };
    size_t sizeofArray = sizeof(ue_update_statusArray) / sizeof(ue_update_statusArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(ue_update_status, ue_update_statusArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

