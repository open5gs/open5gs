
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "domain_name_protocol_any_of.h"

char* OpenAPI_domain_name_protocol_any_of_ToString(OpenAPI_domain_name_protocol_any_of_e domain_name_protocol_any_of)
{
    const char *domain_name_protocol_any_ofArray[] =  { "NULL", "DNS_QNAME", "TLS_SNI", "TLS_SAN", "TSL_SCN" };
    size_t sizeofArray = sizeof(domain_name_protocol_any_ofArray) / sizeof(domain_name_protocol_any_ofArray[0]);
    if (domain_name_protocol_any_of < sizeofArray)
        return (char *)domain_name_protocol_any_ofArray[domain_name_protocol_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_domain_name_protocol_any_of_e OpenAPI_domain_name_protocol_any_of_FromString(char* domain_name_protocol_any_of)
{
    int stringToReturn = 0;
    const char *domain_name_protocol_any_ofArray[] =  { "NULL", "DNS_QNAME", "TLS_SNI", "TLS_SAN", "TSL_SCN" };
    size_t sizeofArray = sizeof(domain_name_protocol_any_ofArray) / sizeof(domain_name_protocol_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(domain_name_protocol_any_of, domain_name_protocol_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

