
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "access_tech_any_of.h"

char* OpenAPI_access_tech_any_of_ToString(OpenAPI_access_tech_any_of_e access_tech_any_of)
{
    const char *access_tech_any_ofArray[] =  { "NULL", "NR", "EUTRAN_IN_WBS1_MODE_AND_NBS1_MODE", "EUTRAN_IN_NBS1_MODE_ONLY", "EUTRAN_IN_WBS1_MODE_ONLY", "UTRAN", "GSM_AND_ECGSM_IoT", "GSM_WITHOUT_ECGSM_IoT", "ECGSM_IoT_ONLY", "CDMA_1xRTT", "CDMA_HRPD", "GSM_COMPACT" };
    size_t sizeofArray = sizeof(access_tech_any_ofArray) / sizeof(access_tech_any_ofArray[0]);
    if (access_tech_any_of < sizeofArray)
        return (char *)access_tech_any_ofArray[access_tech_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_access_tech_any_of_e OpenAPI_access_tech_any_of_FromString(char* access_tech_any_of)
{
    int stringToReturn = 0;
    const char *access_tech_any_ofArray[] =  { "NULL", "NR", "EUTRAN_IN_WBS1_MODE_AND_NBS1_MODE", "EUTRAN_IN_NBS1_MODE_ONLY", "EUTRAN_IN_WBS1_MODE_ONLY", "UTRAN", "GSM_AND_ECGSM_IoT", "GSM_WITHOUT_ECGSM_IoT", "ECGSM_IoT_ONLY", "CDMA_1xRTT", "CDMA_HRPD", "GSM_COMPACT" };
    size_t sizeofArray = sizeof(access_tech_any_ofArray) / sizeof(access_tech_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(access_tech_any_of, access_tech_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

