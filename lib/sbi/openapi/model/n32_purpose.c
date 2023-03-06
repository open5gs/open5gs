
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "n32_purpose.h"

char* OpenAPI_n32_purpose_ToString(OpenAPI_n32_purpose_e n32_purpose)
{
    const char *n32_purposeArray[] =  { "NULL", "ROAMING", "INTER_PLMN_MOBILITY", "SMS_INTERCONNECT", "ROAMING_TEST", "INTER_PLMN_MOBILITY_TEST", "SMS_INTERCONNECT_TEST", "SNPN_INTERCONNECT", "SNPN_INTERCONNECT_TEST", "DISASTER_ROAMING", "DISASTER_ROAMING_TEST" };
    size_t sizeofArray = sizeof(n32_purposeArray) / sizeof(n32_purposeArray[0]);
    if (n32_purpose < sizeofArray)
        return (char *)n32_purposeArray[n32_purpose];
    else
        return (char *)"Unknown";
}

OpenAPI_n32_purpose_e OpenAPI_n32_purpose_FromString(char* n32_purpose)
{
    int stringToReturn = 0;
    const char *n32_purposeArray[] =  { "NULL", "ROAMING", "INTER_PLMN_MOBILITY", "SMS_INTERCONNECT", "ROAMING_TEST", "INTER_PLMN_MOBILITY_TEST", "SMS_INTERCONNECT_TEST", "SNPN_INTERCONNECT", "SNPN_INTERCONNECT_TEST", "DISASTER_ROAMING", "DISASTER_ROAMING_TEST" };
    size_t sizeofArray = sizeof(n32_purposeArray) / sizeof(n32_purposeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(n32_purpose, n32_purposeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

