
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "traffic_profile.h"

char* OpenAPI_traffic_profile_ToString(OpenAPI_traffic_profile_e traffic_profile)
{
    const char *traffic_profileArray[] =  { "NULL", "SINGLE_TRANS_UL", "SINGLE_TRANS_DL", "DUAL_TRANS_UL_FIRST", "DUAL_TRANS_DL_FIRST", "MULTI_TRANS" };
    size_t sizeofArray = sizeof(traffic_profileArray) / sizeof(traffic_profileArray[0]);
    if (traffic_profile < sizeofArray)
        return (char *)traffic_profileArray[traffic_profile];
    else
        return (char *)"Unknown";
}

OpenAPI_traffic_profile_e OpenAPI_traffic_profile_FromString(char* traffic_profile)
{
    int stringToReturn = 0;
    const char *traffic_profileArray[] =  { "NULL", "SINGLE_TRANS_UL", "SINGLE_TRANS_DL", "DUAL_TRANS_UL_FIRST", "DUAL_TRANS_DL_FIRST", "MULTI_TRANS" };
    size_t sizeofArray = sizeof(traffic_profileArray) / sizeof(traffic_profileArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(traffic_profile, traffic_profileArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

