
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "transport_protocol_1.h"

char* OpenAPI_transport_protocol_1_ToString(OpenAPI_transport_protocol_1_e transport_protocol_1)
{
    const char *transport_protocol_1Array[] =  { "NULL", "TCP" };
    size_t sizeofArray = sizeof(transport_protocol_1Array) / sizeof(transport_protocol_1Array[0]);
    if (transport_protocol_1 < sizeofArray)
        return (char *)transport_protocol_1Array[transport_protocol_1];
    else
        return (char *)"Unknown";
}

OpenAPI_transport_protocol_1_e OpenAPI_transport_protocol_1_FromString(char* transport_protocol_1)
{
    int stringToReturn = 0;
    const char *transport_protocol_1Array[] =  { "NULL", "TCP" };
    size_t sizeofArray = sizeof(transport_protocol_1Array) / sizeof(transport_protocol_1Array[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(transport_protocol_1, transport_protocol_1Array[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

