
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "domain_name_protocol.h"

char* OpenAPI_domain_name_protocol_ToString(OpenAPI_domain_name_protocol_e domain_name_protocol)
{
    const char *domain_name_protocolArray[] =  { "NULL", "DNS_QNAME", "TLS_SNI", "TLS_SAN", "TSL_SCN" };
    size_t sizeofArray = sizeof(domain_name_protocolArray) / sizeof(domain_name_protocolArray[0]);
    if (domain_name_protocol < sizeofArray)
        return (char *)domain_name_protocolArray[domain_name_protocol];
    else
        return (char *)"Unknown";
}

OpenAPI_domain_name_protocol_e OpenAPI_domain_name_protocol_FromString(char* domain_name_protocol)
{
    int stringToReturn = 0;
    const char *domain_name_protocolArray[] =  { "NULL", "DNS_QNAME", "TLS_SNI", "TLS_SAN", "TSL_SCN" };
    size_t sizeofArray = sizeof(domain_name_protocolArray) / sizeof(domain_name_protocolArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(domain_name_protocol, domain_name_protocolArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

