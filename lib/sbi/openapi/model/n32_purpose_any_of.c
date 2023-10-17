
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "n32_purpose_any_of.h"

char* OpenAPI_n32_purpose_any_of_ToString(OpenAPI_n32_purpose_any_of_e n32_purpose_any_of)
{
    const char *n32_purpose_any_ofArray[] =  { "NULL", "ROAMING", "INTER_PLMN_MOBILITY", "SMS_INTERCONNECT", "ROAMING_TEST", "INTER_PLMN_MOBILITY_TEST", "SMS_INTERCONNECT_TEST", "SNPN_INTERCONNECT", "SNPN_INTERCONNECT_TEST", "DISASTER_ROAMING", "DISASTER_ROAMING_TEST" };
    size_t sizeofArray = sizeof(n32_purpose_any_ofArray) / sizeof(n32_purpose_any_ofArray[0]);
    if (n32_purpose_any_of < sizeofArray)
        return (char *)n32_purpose_any_ofArray[n32_purpose_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_n32_purpose_any_of_e OpenAPI_n32_purpose_any_of_FromString(char* n32_purpose_any_of)
{
    int stringToReturn = 0;
    const char *n32_purpose_any_ofArray[] =  { "NULL", "ROAMING", "INTER_PLMN_MOBILITY", "SMS_INTERCONNECT", "ROAMING_TEST", "INTER_PLMN_MOBILITY_TEST", "SMS_INTERCONNECT_TEST", "SNPN_INTERCONNECT", "SNPN_INTERCONNECT_TEST", "DISASTER_ROAMING", "DISASTER_ROAMING_TEST" };
    size_t sizeofArray = sizeof(n32_purpose_any_ofArray) / sizeof(n32_purpose_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(n32_purpose_any_of, n32_purpose_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

