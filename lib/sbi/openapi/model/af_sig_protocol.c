
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "af_sig_protocol.h"

char* OpenAPI_af_sig_protocol_ToString(OpenAPI_af_sig_protocol_e af_sig_protocol)
{
    const char *af_sig_protocolArray[] =  { "NULL", "NO_INFORMATION", "SIP" };
    size_t sizeofArray = sizeof(af_sig_protocolArray) / sizeof(af_sig_protocolArray[0]);
    if (af_sig_protocol < sizeofArray)
        return (char *)af_sig_protocolArray[af_sig_protocol];
    else
        return (char *)"Unknown";
}

OpenAPI_af_sig_protocol_e OpenAPI_af_sig_protocol_FromString(char* af_sig_protocol)
{
    int stringToReturn = 0;
    const char *af_sig_protocolArray[] =  { "NULL", "NO_INFORMATION", "SIP" };
    size_t sizeofArray = sizeof(af_sig_protocolArray) / sizeof(af_sig_protocolArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(af_sig_protocol, af_sig_protocolArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

