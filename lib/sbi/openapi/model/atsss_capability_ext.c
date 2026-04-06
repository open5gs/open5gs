
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "atsss_capability_ext.h"

char* OpenAPI_atsss_capability_ext_ToString(OpenAPI_atsss_capability_ext_e atsss_capability_ext)
{
    const char *atsss_capability_extArray[] =  { "NULL", "MPTCP", "MPQUIC_UDP", "MPQUIC_IP", "MPQUIC_E", "ATSSS_LL", "ATSSS_LL_WITH_ASMODE_UL", "ATSSS_LL_WITH_EXSDMODE_DL_ASMODE_UL", "ATSSS_LL_WITH_ASMODE_DLUL" };
    size_t sizeofArray = sizeof(atsss_capability_extArray) / sizeof(atsss_capability_extArray[0]);
    if (atsss_capability_ext < sizeofArray)
        return (char *)atsss_capability_extArray[atsss_capability_ext];
    else
        return (char *)"Unknown";
}

OpenAPI_atsss_capability_ext_e OpenAPI_atsss_capability_ext_FromString(char* atsss_capability_ext)
{
    int stringToReturn = 0;
    const char *atsss_capability_extArray[] =  { "NULL", "MPTCP", "MPQUIC_UDP", "MPQUIC_IP", "MPQUIC_E", "ATSSS_LL", "ATSSS_LL_WITH_ASMODE_UL", "ATSSS_LL_WITH_EXSDMODE_DL_ASMODE_UL", "ATSSS_LL_WITH_ASMODE_DLUL" };
    size_t sizeofArray = sizeof(atsss_capability_extArray) / sizeof(atsss_capability_extArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(atsss_capability_ext, atsss_capability_extArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

