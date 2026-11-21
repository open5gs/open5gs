
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "charging_transport_protocol.h"

char* OpenAPI_charging_transport_protocol_ToString(OpenAPI_charging_transport_protocol_e charging_transport_protocol)
{
    const char *charging_transport_protocolArray[] =  { "NULL", "UDP", "TCP" };
    size_t sizeofArray = sizeof(charging_transport_protocolArray) / sizeof(charging_transport_protocolArray[0]);
    if (charging_transport_protocol < sizeofArray)
        return (char *)charging_transport_protocolArray[charging_transport_protocol];
    else
        return (char *)"Unknown";
}

OpenAPI_charging_transport_protocol_e OpenAPI_charging_transport_protocol_FromString(char* charging_transport_protocol)
{
    int stringToReturn = 0;
    const char *charging_transport_protocolArray[] =  { "NULL", "UDP", "TCP" };
    size_t sizeofArray = sizeof(charging_transport_protocolArray) / sizeof(charging_transport_protocolArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(charging_transport_protocol, charging_transport_protocolArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

