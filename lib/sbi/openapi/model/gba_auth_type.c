
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "gba_auth_type.h"

char* OpenAPI_gba_auth_type_ToString(OpenAPI_gba_auth_type_e gba_auth_type)
{
    const char *gba_auth_typeArray[] =  { "NULL", "DIGEST_AKAV1_MD5" };
    size_t sizeofArray = sizeof(gba_auth_typeArray) / sizeof(gba_auth_typeArray[0]);
    if (gba_auth_type < sizeofArray)
        return (char *)gba_auth_typeArray[gba_auth_type];
    else
        return (char *)"Unknown";
}

OpenAPI_gba_auth_type_e OpenAPI_gba_auth_type_FromString(char* gba_auth_type)
{
    int stringToReturn = 0;
    const char *gba_auth_typeArray[] =  { "NULL", "DIGEST_AKAV1_MD5" };
    size_t sizeofArray = sizeof(gba_auth_typeArray) / sizeof(gba_auth_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(gba_auth_type, gba_auth_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

