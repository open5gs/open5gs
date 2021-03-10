
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "hss_auth_type_in_uri.h"

char* OpenAPI_hss_auth_type_in_uri_ToString(OpenAPI_hss_auth_type_in_uri_e hss_auth_type_in_uri)
{
    const char *hss_auth_type_in_uriArray[] =  { "NULL", "eps_aka", "eap_aka", "eap_aka_prime", "ims_aka", "gba_aka" };
    size_t sizeofArray = sizeof(hss_auth_type_in_uriArray) / sizeof(hss_auth_type_in_uriArray[0]);
    if (hss_auth_type_in_uri < sizeofArray)
        return (char *)hss_auth_type_in_uriArray[hss_auth_type_in_uri];
    else
        return (char *)"Unknown";
}

OpenAPI_hss_auth_type_in_uri_e OpenAPI_hss_auth_type_in_uri_FromString(char* hss_auth_type_in_uri)
{
    int stringToReturn = 0;
    const char *hss_auth_type_in_uriArray[] =  { "NULL", "eps_aka", "eap_aka", "eap_aka_prime", "ims_aka", "gba_aka" };
    size_t sizeofArray = sizeof(hss_auth_type_in_uriArray) / sizeof(hss_auth_type_in_uriArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(hss_auth_type_in_uri, hss_auth_type_in_uriArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

