
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sm_address_type.h"

char* OpenAPI_sm_address_type_ToString(OpenAPI_sm_address_type_e sm_address_type)
{
    const char *sm_address_typeArray[] =  { "NULL", "EMAIL_ADDRESS", "MSISDN", "IPV4_ADDRESS", "IPV6_ADDRESS", "NUMERIC_SHORTCODE", "ALPHANUMERIC_SHORTCODE", "OTHER", "IMSI" };
    size_t sizeofArray = sizeof(sm_address_typeArray) / sizeof(sm_address_typeArray[0]);
    if (sm_address_type < sizeofArray)
        return (char *)sm_address_typeArray[sm_address_type];
    else
        return (char *)"Unknown";
}

OpenAPI_sm_address_type_e OpenAPI_sm_address_type_FromString(char* sm_address_type)
{
    int stringToReturn = 0;
    const char *sm_address_typeArray[] =  { "NULL", "EMAIL_ADDRESS", "MSISDN", "IPV4_ADDRESS", "IPV6_ADDRESS", "NUMERIC_SHORTCODE", "ALPHANUMERIC_SHORTCODE", "OTHER", "IMSI" };
    size_t sizeofArray = sizeof(sm_address_typeArray) / sizeof(sm_address_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(sm_address_type, sm_address_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

