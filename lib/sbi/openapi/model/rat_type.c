
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "rat_type.h"

char* OpenAPI_rat_type_ToString(OpenAPI_rat_type_e rat_type)
{
    const char *rat_typeArray[] =  { "NULL", "NR", "EUTRA", "WLAN", "VIRTUAL", "NBIOT", "WIRELINE", "WIRELINE_CABLE", "WIRELINE_BBF", "LTE-M", "NR_U", "EUTRA_U", "TRUSTED_N3GA", "TRUSTED_WLAN", "UTRA", "GERA", "NR_LEO", "NR_MEO", "NR_GEO", "NR_OTHER_SAT", "NR_REDCAP", "WB_E_UTRAN_LEO", "WB_E_UTRAN_MEO", "WB_E_UTRAN_GEO", "WB_E_UTRAN_OTHERSAT", "NB_IOT_LEO", "NB_IOT_MEO", "NB_IOT_GEO", "NB_IOT_OTHERSAT", "LTE_M_LEO", "LTE_M_MEO", "LTE_M_GEO", "LTE_M_OTHERSAT", "NR_EREDCAP", "HSPA_EVOLUTION" };
    size_t sizeofArray = sizeof(rat_typeArray) / sizeof(rat_typeArray[0]);
    if (rat_type < sizeofArray)
        return (char *)rat_typeArray[rat_type];
    else
        return (char *)"Unknown";
}

OpenAPI_rat_type_e OpenAPI_rat_type_FromString(char* rat_type)
{
    int stringToReturn = 0;
    const char *rat_typeArray[] =  { "NULL", "NR", "EUTRA", "WLAN", "VIRTUAL", "NBIOT", "WIRELINE", "WIRELINE_CABLE", "WIRELINE_BBF", "LTE-M", "NR_U", "EUTRA_U", "TRUSTED_N3GA", "TRUSTED_WLAN", "UTRA", "GERA", "NR_LEO", "NR_MEO", "NR_GEO", "NR_OTHER_SAT", "NR_REDCAP", "WB_E_UTRAN_LEO", "WB_E_UTRAN_MEO", "WB_E_UTRAN_GEO", "WB_E_UTRAN_OTHERSAT", "NB_IOT_LEO", "NB_IOT_MEO", "NB_IOT_GEO", "NB_IOT_OTHERSAT", "LTE_M_LEO", "LTE_M_MEO", "LTE_M_GEO", "LTE_M_OTHERSAT", "NR_EREDCAP", "HSPA_EVOLUTION" };
    size_t sizeofArray = sizeof(rat_typeArray) / sizeof(rat_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(rat_type, rat_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

