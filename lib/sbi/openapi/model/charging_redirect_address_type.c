
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "charging_redirect_address_type.h"

char* OpenAPI_charging_redirect_address_type_ToString(OpenAPI_charging_redirect_address_type_e charging_redirect_address_type)
{
    const char *charging_redirect_address_typeArray[] =  { "NULL", "IPV4", "IPV6", "URL", "URI" };
    size_t sizeofArray = sizeof(charging_redirect_address_typeArray) / sizeof(charging_redirect_address_typeArray[0]);
    if (charging_redirect_address_type < sizeofArray)
        return (char *)charging_redirect_address_typeArray[charging_redirect_address_type];
    else
        return (char *)"Unknown";
}

OpenAPI_charging_redirect_address_type_e OpenAPI_charging_redirect_address_type_FromString(char* charging_redirect_address_type)
{
    int stringToReturn = 0;
    const char *charging_redirect_address_typeArray[] =  { "NULL", "IPV4", "IPV6", "URL", "URI" };
    size_t sizeofArray = sizeof(charging_redirect_address_typeArray) / sizeof(charging_redirect_address_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(charging_redirect_address_type, charging_redirect_address_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

