
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ip_reachability.h"

char* OpenAPI_ip_reachability_ToString(OpenAPI_ip_reachability_e ip_reachability)
{
    const char *ip_reachabilityArray[] =  { "NULL", "IPV4", "IPV6", "IPV4V6" };
    size_t sizeofArray = sizeof(ip_reachabilityArray) / sizeof(ip_reachabilityArray[0]);
    if (ip_reachability < sizeofArray)
        return (char *)ip_reachabilityArray[ip_reachability];
    else
        return (char *)"Unknown";
}

OpenAPI_ip_reachability_e OpenAPI_ip_reachability_FromString(char* ip_reachability)
{
    int stringToReturn = 0;
    const char *ip_reachabilityArray[] =  { "NULL", "IPV4", "IPV6", "IPV4V6" };
    size_t sizeofArray = sizeof(ip_reachabilityArray) / sizeof(ip_reachabilityArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(ip_reachability, ip_reachabilityArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

