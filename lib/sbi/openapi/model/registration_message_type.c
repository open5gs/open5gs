
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "registration_message_type.h"

char* OpenAPI_registration_message_type_ToString(OpenAPI_registration_message_type_e registration_message_type)
{
    const char *registration_message_typeArray[] =  { "NULL", "INITIAL", "MOBILITY", "PERIODIC", "EMERGENCY", "DEREGISTRATION" };
    size_t sizeofArray = sizeof(registration_message_typeArray) / sizeof(registration_message_typeArray[0]);
    if (registration_message_type < sizeofArray)
        return (char *)registration_message_typeArray[registration_message_type];
    else
        return (char *)"Unknown";
}

OpenAPI_registration_message_type_e OpenAPI_registration_message_type_FromString(char* registration_message_type)
{
    int stringToReturn = 0;
    const char *registration_message_typeArray[] =  { "NULL", "INITIAL", "MOBILITY", "PERIODIC", "EMERGENCY", "DEREGISTRATION" };
    size_t sizeofArray = sizeof(registration_message_typeArray) / sizeof(registration_message_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(registration_message_type, registration_message_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

