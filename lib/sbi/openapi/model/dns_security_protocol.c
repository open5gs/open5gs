
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dns_security_protocol.h"

char* OpenAPI_dns_security_protocol_ToString(OpenAPI_dns_security_protocol_e dns_security_protocol)
{
    const char *dns_security_protocolArray[] =  { "NULL", "TLS", "DTLS" };
    size_t sizeofArray = sizeof(dns_security_protocolArray) / sizeof(dns_security_protocolArray[0]);
    if (dns_security_protocol < sizeofArray)
        return (char *)dns_security_protocolArray[dns_security_protocol];
    else
        return (char *)"Unknown";
}

OpenAPI_dns_security_protocol_e OpenAPI_dns_security_protocol_FromString(char* dns_security_protocol)
{
    int stringToReturn = 0;
    const char *dns_security_protocolArray[] =  { "NULL", "TLS", "DTLS" };
    size_t sizeofArray = sizeof(dns_security_protocolArray) / sizeof(dns_security_protocolArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(dns_security_protocol, dns_security_protocolArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

