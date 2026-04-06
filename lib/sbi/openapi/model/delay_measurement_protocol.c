
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "delay_measurement_protocol.h"

char* OpenAPI_delay_measurement_protocol_ToString(OpenAPI_delay_measurement_protocol_e delay_measurement_protocol)
{
    const char *delay_measurement_protocolArray[] =  { "NULL", "TWAMP", "OWAMP", "STAMP", "OTHER" };
    size_t sizeofArray = sizeof(delay_measurement_protocolArray) / sizeof(delay_measurement_protocolArray[0]);
    if (delay_measurement_protocol < sizeofArray)
        return (char *)delay_measurement_protocolArray[delay_measurement_protocol];
    else
        return (char *)"Unknown";
}

OpenAPI_delay_measurement_protocol_e OpenAPI_delay_measurement_protocol_FromString(char* delay_measurement_protocol)
{
    int stringToReturn = 0;
    const char *delay_measurement_protocolArray[] =  { "NULL", "TWAMP", "OWAMP", "STAMP", "OTHER" };
    size_t sizeofArray = sizeof(delay_measurement_protocolArray) / sizeof(delay_measurement_protocolArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(delay_measurement_protocol, delay_measurement_protocolArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

