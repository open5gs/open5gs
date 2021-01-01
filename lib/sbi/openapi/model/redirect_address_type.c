
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "redirect_address_type.h"

char* OpenAPI_redirect_address_type_ToString(OpenAPI_redirect_address_type_e redirect_address_type)
{
    const char *redirect_address_typeArray[] =  { "NULL", "IPV4_ADDR", "IPV6_ADDR", "URL", "SIP_URI" };
    size_t sizeofArray = sizeof(redirect_address_typeArray) / sizeof(redirect_address_typeArray[0]);
    if (redirect_address_type < sizeofArray)
        return (char *)redirect_address_typeArray[redirect_address_type];
    else
        return (char *)"Unknown";
}

OpenAPI_redirect_address_type_e OpenAPI_redirect_address_type_FromString(char* redirect_address_type)
{
    int stringToReturn = 0;
    const char *redirect_address_typeArray[] =  { "NULL", "IPV4_ADDR", "IPV6_ADDR", "URL", "SIP_URI" };
    size_t sizeofArray = sizeof(redirect_address_typeArray) / sizeof(redirect_address_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(redirect_address_type, redirect_address_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

