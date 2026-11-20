
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "supplementary_service_mode.h"

char* OpenAPI_supplementary_service_mode_ToString(OpenAPI_supplementary_service_mode_e supplementary_service_mode)
{
    const char *supplementary_service_modeArray[] =  { "NULL", "CFU", "CFB", "CFNR", "CFNL", "CD", "CFNRC", "ICB", "OCB", "ACR", "BLIND_TRANFER", "CONSULTATIVE_TRANFER" };
    size_t sizeofArray = sizeof(supplementary_service_modeArray) / sizeof(supplementary_service_modeArray[0]);
    if (supplementary_service_mode < sizeofArray)
        return (char *)supplementary_service_modeArray[supplementary_service_mode];
    else
        return (char *)"Unknown";
}

OpenAPI_supplementary_service_mode_e OpenAPI_supplementary_service_mode_FromString(char* supplementary_service_mode)
{
    int stringToReturn = 0;
    const char *supplementary_service_modeArray[] =  { "NULL", "CFU", "CFB", "CFNR", "CFNL", "CD", "CFNRC", "ICB", "OCB", "ACR", "BLIND_TRANFER", "CONSULTATIVE_TRANFER" };
    size_t sizeofArray = sizeof(supplementary_service_modeArray) / sizeof(supplementary_service_modeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(supplementary_service_mode, supplementary_service_modeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

