
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "exception_id.h"

char* OpenAPI_exception_id_ToString(OpenAPI_exception_id_e exception_id)
{
    const char *exception_idArray[] =  { "NULL", "UNEXPECTED_UE_LOCATION", "UNEXPECTED_LONG_LIVE_FLOW", "UNEXPECTED_LARGE_RATE_FLOW", "UNEXPECTED_WAKEUP", "SUSPICION_OF_DDOS_ATTACK", "WRONG_DESTINATION_ADDRESS", "TOO_FREQUENT_SERVICE_ACCESS", "UNEXPECTED_RADIO_LINK_FAILURES", "PING_PONG_ACROSS_CELLS" };
    size_t sizeofArray = sizeof(exception_idArray) / sizeof(exception_idArray[0]);
    if (exception_id < sizeofArray)
        return (char *)exception_idArray[exception_id];
    else
        return (char *)"Unknown";
}

OpenAPI_exception_id_e OpenAPI_exception_id_FromString(char* exception_id)
{
    int stringToReturn = 0;
    const char *exception_idArray[] =  { "NULL", "UNEXPECTED_UE_LOCATION", "UNEXPECTED_LONG_LIVE_FLOW", "UNEXPECTED_LARGE_RATE_FLOW", "UNEXPECTED_WAKEUP", "SUSPICION_OF_DDOS_ATTACK", "WRONG_DESTINATION_ADDRESS", "TOO_FREQUENT_SERVICE_ACCESS", "UNEXPECTED_RADIO_LINK_FAILURES", "PING_PONG_ACROSS_CELLS" };
    size_t sizeofArray = sizeof(exception_idArray) / sizeof(exception_idArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(exception_id, exception_idArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

