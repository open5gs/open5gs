
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "hss_av_type.h"

char* OpenAPI_hss_av_type_ToString(OpenAPI_hss_av_type_e hss_av_type)
{
    const char *hss_av_typeArray[] =  { "NULL", "EPS_AKA", "EAP_AKA", "IMS_AKA", "GBA_AKA", "UMTS_AKA" };
    size_t sizeofArray = sizeof(hss_av_typeArray) / sizeof(hss_av_typeArray[0]);
    if (hss_av_type < sizeofArray)
        return (char *)hss_av_typeArray[hss_av_type];
    else
        return (char *)"Unknown";
}

OpenAPI_hss_av_type_e OpenAPI_hss_av_type_FromString(char* hss_av_type)
{
    int stringToReturn = 0;
    const char *hss_av_typeArray[] =  { "NULL", "EPS_AKA", "EAP_AKA", "IMS_AKA", "GBA_AKA", "UMTS_AKA" };
    size_t sizeofArray = sizeof(hss_av_typeArray) / sizeof(hss_av_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(hss_av_type, hss_av_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

