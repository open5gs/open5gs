
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "upf_packet_inspection_functionality.h"

char* OpenAPI_upf_packet_inspection_functionality_ToString(OpenAPI_upf_packet_inspection_functionality_e upf_packet_inspection_functionality)
{
    const char *upf_packet_inspection_functionalityArray[] =  { "NULL", "DEEP_PACKET_INSPECTION", "MAC_FILTER_BASED_PACKET_DETECTION", "IP_FILTER_BASED_PACKET_DETECTION" };
    size_t sizeofArray = sizeof(upf_packet_inspection_functionalityArray) / sizeof(upf_packet_inspection_functionalityArray[0]);
    if (upf_packet_inspection_functionality < sizeofArray)
        return (char *)upf_packet_inspection_functionalityArray[upf_packet_inspection_functionality];
    else
        return (char *)"Unknown";
}

OpenAPI_upf_packet_inspection_functionality_e OpenAPI_upf_packet_inspection_functionality_FromString(char* upf_packet_inspection_functionality)
{
    int stringToReturn = 0;
    const char *upf_packet_inspection_functionalityArray[] =  { "NULL", "DEEP_PACKET_INSPECTION", "MAC_FILTER_BASED_PACKET_DETECTION", "IP_FILTER_BASED_PACKET_DETECTION" };
    size_t sizeofArray = sizeof(upf_packet_inspection_functionalityArray) / sizeof(upf_packet_inspection_functionalityArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(upf_packet_inspection_functionality, upf_packet_inspection_functionalityArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

