
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "atsss_capability_any_of.h"

char* OpenAPI_atsss_capability_any_of_ToString(OpenAPI_atsss_capability_any_of_e atsss_capability_any_of)
{
    const char *atsss_capability_any_ofArray[] =  { "NULL", "MPTCP_ATSSS_LL_WITH_ASMODE_UL", "MPTCP_ATSSS_LL_WITH_EXSDMODE_DL_ASMODE_UL", "MPTCP_ATSSS_LL_WITH_ASMODE_DLUL", "ATSSS_LL", "MPTCP_ATSSS_LL" };
    size_t sizeofArray = sizeof(atsss_capability_any_ofArray) / sizeof(atsss_capability_any_ofArray[0]);
    if (atsss_capability_any_of < sizeofArray)
        return (char *)atsss_capability_any_ofArray[atsss_capability_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_atsss_capability_any_of_e OpenAPI_atsss_capability_any_of_FromString(char* atsss_capability_any_of)
{
    int stringToReturn = 0;
    const char *atsss_capability_any_ofArray[] =  { "NULL", "MPTCP_ATSSS_LL_WITH_ASMODE_UL", "MPTCP_ATSSS_LL_WITH_EXSDMODE_DL_ASMODE_UL", "MPTCP_ATSSS_LL_WITH_ASMODE_DLUL", "ATSSS_LL", "MPTCP_ATSSS_LL" };
    size_t sizeofArray = sizeof(atsss_capability_any_ofArray) / sizeof(atsss_capability_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(atsss_capability_any_of, atsss_capability_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

