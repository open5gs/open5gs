
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "n1_n2_message_transfer_cause.h"

char* OpenAPI_n1_n2_message_transfer_cause_ToString(OpenAPI_n1_n2_message_transfer_cause_e n1_n2_message_transfer_cause)
{
    const char *n1_n2_message_transfer_causeArray[] =  { "NULL", "ATTEMPTING_TO_REACH_UE", "N1_N2_TRANSFER_INITIATED", "WAITING_FOR_ASYNCHRONOUS_TRANSFER", "UE_NOT_RESPONDING", "N1_MSG_NOT_TRANSFERRED", "UE_NOT_REACHABLE_FOR_SESSION", "TEMPORARY_REJECT_REGISTRATION_ONGOING", "TEMPORARY_REJECT_HANDOVER_ONGOING" };
    size_t sizeofArray = sizeof(n1_n2_message_transfer_causeArray) / sizeof(n1_n2_message_transfer_causeArray[0]);
    if (n1_n2_message_transfer_cause < sizeofArray)
        return (char *)n1_n2_message_transfer_causeArray[n1_n2_message_transfer_cause];
    else
        return (char *)"Unknown";
}

OpenAPI_n1_n2_message_transfer_cause_e OpenAPI_n1_n2_message_transfer_cause_FromString(char* n1_n2_message_transfer_cause)
{
    int stringToReturn = 0;
    const char *n1_n2_message_transfer_causeArray[] =  { "NULL", "ATTEMPTING_TO_REACH_UE", "N1_N2_TRANSFER_INITIATED", "WAITING_FOR_ASYNCHRONOUS_TRANSFER", "UE_NOT_RESPONDING", "N1_MSG_NOT_TRANSFERRED", "UE_NOT_REACHABLE_FOR_SESSION", "TEMPORARY_REJECT_REGISTRATION_ONGOING", "TEMPORARY_REJECT_HANDOVER_ONGOING" };
    size_t sizeofArray = sizeof(n1_n2_message_transfer_causeArray) / sizeof(n1_n2_message_transfer_causeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(n1_n2_message_transfer_cause, n1_n2_message_transfer_causeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

