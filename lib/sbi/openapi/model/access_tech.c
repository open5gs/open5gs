
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "access_tech.h"

char* OpenAPI_access_tech_ToString(OpenAPI_access_tech_e access_tech)
{
    const char *access_techArray[] =  { "NULL", "NR", "EUTRAN_IN_WBS1_MODE_AND_NBS1_MODE", "EUTRAN_IN_NBS1_MODE_ONLY", "EUTRAN_IN_WBS1_MODE_ONLY", "UTRAN", "GSM_AND_ECGSM_IoT", "GSM_WITHOUT_ECGSM_IoT", "ECGSM_IoT_ONLY", "CDMA_1xRTT", "CDMA_HRPD", "GSM_COMPACT", "SATELLITE_NG_RAN", "SATELLITE_EUTRAN_IN_NBS1_MODE", "SATELLITE_EUTRAN_IN_WBS1_MODE" };
    size_t sizeofArray = sizeof(access_techArray) / sizeof(access_techArray[0]);
    if (access_tech < sizeofArray)
        return (char *)access_techArray[access_tech];
    else
        return (char *)"Unknown";
}

OpenAPI_access_tech_e OpenAPI_access_tech_FromString(char* access_tech)
{
    int stringToReturn = 0;
    const char *access_techArray[] =  { "NULL", "NR", "EUTRAN_IN_WBS1_MODE_AND_NBS1_MODE", "EUTRAN_IN_NBS1_MODE_ONLY", "EUTRAN_IN_WBS1_MODE_ONLY", "UTRAN", "GSM_AND_ECGSM_IoT", "GSM_WITHOUT_ECGSM_IoT", "ECGSM_IoT_ONLY", "CDMA_1xRTT", "CDMA_HRPD", "GSM_COMPACT", "SATELLITE_NG_RAN", "SATELLITE_EUTRAN_IN_NBS1_MODE", "SATELLITE_EUTRAN_IN_WBS1_MODE" };
    size_t sizeofArray = sizeof(access_techArray) / sizeof(access_techArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(access_tech, access_techArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

