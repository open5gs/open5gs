
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "odb_packet_services.h"

char* OpenAPI_odb_packet_services_ToString(OpenAPI_odb_packet_services_e odb_packet_services)
{
    const char *odb_packet_servicesArray[] =  { "NULL", "ALL_PACKET_SERVICES", "ROAMER_ACCESS_HPLMN_AP", "ROAMER_ACCESS_VPLMN_AP" };
    size_t sizeofArray = sizeof(odb_packet_servicesArray) / sizeof(odb_packet_servicesArray[0]);
    if (odb_packet_services < sizeofArray)
        return (char *)odb_packet_servicesArray[odb_packet_services];
    else
        return (char *)"Unknown";
}

OpenAPI_odb_packet_services_e OpenAPI_odb_packet_services_FromString(char* odb_packet_services)
{
    int stringToReturn = 0;
    const char *odb_packet_servicesArray[] =  { "NULL", "ALL_PACKET_SERVICES", "ROAMER_ACCESS_HPLMN_AP", "ROAMER_ACCESS_VPLMN_AP" };
    size_t sizeofArray = sizeof(odb_packet_servicesArray) / sizeof(odb_packet_servicesArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(odb_packet_services, odb_packet_servicesArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

