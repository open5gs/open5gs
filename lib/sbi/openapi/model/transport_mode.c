
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "transport_mode.h"

char* OpenAPI_transport_mode_ToString(OpenAPI_transport_mode_e transport_mode)
{
    const char *transport_modeArray[] =  { "NULL", "DATAGRAM_MODE_1", "DATAGRAM_MODE_2", "STREAM_MODE" };
    size_t sizeofArray = sizeof(transport_modeArray) / sizeof(transport_modeArray[0]);
    if (transport_mode < sizeofArray)
        return (char *)transport_modeArray[transport_mode];
    else
        return (char *)"Unknown";
}

OpenAPI_transport_mode_e OpenAPI_transport_mode_FromString(char* transport_mode)
{
    int stringToReturn = 0;
    const char *transport_modeArray[] =  { "NULL", "DATAGRAM_MODE_1", "DATAGRAM_MODE_2", "STREAM_MODE" };
    size_t sizeofArray = sizeof(transport_modeArray) / sizeof(transport_modeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(transport_mode, transport_modeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

