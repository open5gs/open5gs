
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "key_amf_type.h"

char* OpenAPI_key_amf_type_ToString(OpenAPI_key_amf_type_e key_amf_type)
{
    const char *key_amf_typeArray[] =  { "NULL", "KAMF", "KPRIMEAMF" };
    size_t sizeofArray = sizeof(key_amf_typeArray) / sizeof(key_amf_typeArray[0]);
    if (key_amf_type < sizeofArray)
        return (char *)key_amf_typeArray[key_amf_type];
    else
        return (char *)"Unknown";
}

OpenAPI_key_amf_type_e OpenAPI_key_amf_type_FromString(char* key_amf_type)
{
    int stringToReturn = 0;
    const char *key_amf_typeArray[] =  { "NULL", "KAMF", "KPRIMEAMF" };
    size_t sizeofArray = sizeof(key_amf_typeArray) / sizeof(key_amf_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(key_amf_type, key_amf_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

