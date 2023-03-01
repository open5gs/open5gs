
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "npcf_atsss_capability.h"

char* OpenAPI_npcf_atsss_capability_ToString(OpenAPI_npcf_atsss_capability_e npcf_atsss_capability)
{
    const char *npcf_atsss_capabilityArray[] =  { "NULL", "MPTCP_ATSSS_LL_WITH_ASMODE_UL", "MPTCP_ATSSS_LL_WITH_EXSDMODE_DL_ASMODE_UL", "MPTCP_ATSSS_LL_WITH_ASMODE_DLUL", "ATSSS_LL", "MPTCP_ATSSS_LL" };
    size_t sizeofArray = sizeof(npcf_atsss_capabilityArray) / sizeof(npcf_atsss_capabilityArray[0]);
    if (npcf_atsss_capability < sizeofArray)
        return (char *)npcf_atsss_capabilityArray[npcf_atsss_capability];
    else
        return (char *)"Unknown";
}

OpenAPI_npcf_atsss_capability_e OpenAPI_npcf_atsss_capability_FromString(char* npcf_atsss_capability)
{
    int stringToReturn = 0;
    const char *npcf_atsss_capabilityArray[] =  { "NULL", "MPTCP_ATSSS_LL_WITH_ASMODE_UL", "MPTCP_ATSSS_LL_WITH_EXSDMODE_DL_ASMODE_UL", "MPTCP_ATSSS_LL_WITH_ASMODE_DLUL", "ATSSS_LL", "MPTCP_ATSSS_LL" };
    size_t sizeofArray = sizeof(npcf_atsss_capabilityArray) / sizeof(npcf_atsss_capabilityArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(npcf_atsss_capability, npcf_atsss_capabilityArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

