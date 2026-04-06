
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nsac_admission_mode.h"

char* OpenAPI_nsac_admission_mode_ToString(OpenAPI_nsac_admission_mode_e nsac_admission_mode)
{
    const char *nsac_admission_modeArray[] =  { "NULL", "VPLMN_ADMISSION", "VPLMN_WITH_HPLMN_ASSISTANCE", "HPLMN_ADMISSION" };
    size_t sizeofArray = sizeof(nsac_admission_modeArray) / sizeof(nsac_admission_modeArray[0]);
    if (nsac_admission_mode < sizeofArray)
        return (char *)nsac_admission_modeArray[nsac_admission_mode];
    else
        return (char *)"Unknown";
}

OpenAPI_nsac_admission_mode_e OpenAPI_nsac_admission_mode_FromString(char* nsac_admission_mode)
{
    int stringToReturn = 0;
    const char *nsac_admission_modeArray[] =  { "NULL", "VPLMN_ADMISSION", "VPLMN_WITH_HPLMN_ASSISTANCE", "HPLMN_ADMISSION" };
    size_t sizeofArray = sizeof(nsac_admission_modeArray) / sizeof(nsac_admission_modeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(nsac_admission_mode, nsac_admission_modeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

