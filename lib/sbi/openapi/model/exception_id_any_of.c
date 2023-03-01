
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "exception_id_any_of.h"

char* OpenAPI_exception_id_any_of_ToString(OpenAPI_exception_id_any_of_e exception_id_any_of)
{
    const char *exception_id_any_ofArray[] =  { "NULL", "UNEXPECTED_UE_LOCATION", "UNEXPECTED_LONG_LIVE_FLOW", "UNEXPECTED_LARGE_RATE_FLOW", "UNEXPECTED_WAKEUP", "SUSPICION_OF_DDOS_ATTACK", "WRONG_DESTINATION_ADDRESS", "TOO_FREQUENT_SERVICE_ACCESS", "UNEXPECTED_RADIO_LINK_FAILURES", "PING_PONG_ACROSS_CELLS" };
    size_t sizeofArray = sizeof(exception_id_any_ofArray) / sizeof(exception_id_any_ofArray[0]);
    if (exception_id_any_of < sizeofArray)
        return (char *)exception_id_any_ofArray[exception_id_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_exception_id_any_of_e OpenAPI_exception_id_any_of_FromString(char* exception_id_any_of)
{
    int stringToReturn = 0;
    const char *exception_id_any_ofArray[] =  { "NULL", "UNEXPECTED_UE_LOCATION", "UNEXPECTED_LONG_LIVE_FLOW", "UNEXPECTED_LARGE_RATE_FLOW", "UNEXPECTED_WAKEUP", "SUSPICION_OF_DDOS_ATTACK", "WRONG_DESTINATION_ADDRESS", "TOO_FREQUENT_SERVICE_ACCESS", "UNEXPECTED_RADIO_LINK_FAILURES", "PING_PONG_ACROSS_CELLS" };
    size_t sizeofArray = sizeof(exception_id_any_ofArray) / sizeof(exception_id_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(exception_id_any_of, exception_id_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

