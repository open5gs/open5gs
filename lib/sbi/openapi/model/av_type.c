
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "av_type.h"

char* OpenAPI_av_type_ToString(OpenAPI_av_type_e av_type)
{
    const char *av_typeArray[] =  { "NULL", "5G_HE_AKA", "EAP_AKA_PRIME" };
    size_t sizeofArray = sizeof(av_typeArray) / sizeof(av_typeArray[0]);
    if (av_type < sizeofArray)
        return (char *)av_typeArray[av_type];
    else
        return (char *)"Unknown";
}

OpenAPI_av_type_e OpenAPI_av_type_FromString(char* av_type)
{
    int stringToReturn = 0;
    const char *av_typeArray[] =  { "NULL", "5G_HE_AKA", "EAP_AKA_PRIME" };
    size_t sizeofArray = sizeof(av_typeArray) / sizeof(av_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(av_type, av_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

