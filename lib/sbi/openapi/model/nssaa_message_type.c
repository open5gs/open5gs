
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nssaa_message_type.h"

char* OpenAPI_nssaa_message_type_ToString(OpenAPI_nssaa_message_type_e nssaa_message_type)
{
    const char *nssaa_message_typeArray[] =  { "NULL", "Authenticate", "Re-Authentication-Notification", "Revocation-Notification" };
    size_t sizeofArray = sizeof(nssaa_message_typeArray) / sizeof(nssaa_message_typeArray[0]);
    if (nssaa_message_type < sizeofArray)
        return (char *)nssaa_message_typeArray[nssaa_message_type];
    else
        return (char *)"Unknown";
}

OpenAPI_nssaa_message_type_e OpenAPI_nssaa_message_type_FromString(char* nssaa_message_type)
{
    int stringToReturn = 0;
    const char *nssaa_message_typeArray[] =  { "NULL", "Authenticate", "Re-Authentication-Notification", "Revocation-Notification" };
    size_t sizeofArray = sizeof(nssaa_message_typeArray) / sizeof(nssaa_message_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(nssaa_message_type, nssaa_message_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

