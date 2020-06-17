
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "n2_info_notify_reason.h"

char* OpenAPI_n2_info_notify_reason_ToString(OpenAPI_n2_info_notify_reason_e n2_info_notify_reason)
{
    const char *n2_info_notify_reasonArray[] =  { "NULL", "HANDOVER_COMPLETED" };
    size_t sizeofArray = sizeof(n2_info_notify_reasonArray) / sizeof(n2_info_notify_reasonArray[0]);
    if (n2_info_notify_reason < sizeofArray)
        return (char *)n2_info_notify_reasonArray[n2_info_notify_reason];
    else
        return (char *)"Unknown";
}

OpenAPI_n2_info_notify_reason_e OpenAPI_n2_info_notify_reason_FromString(char* n2_info_notify_reason)
{
    int stringToReturn = 0;
    const char *n2_info_notify_reasonArray[] =  { "NULL", "HANDOVER_COMPLETED" };
    size_t sizeofArray = sizeof(n2_info_notify_reasonArray) / sizeof(n2_info_notify_reasonArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(n2_info_notify_reason, n2_info_notify_reasonArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

