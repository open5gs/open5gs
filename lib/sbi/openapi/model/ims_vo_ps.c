
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ims_vo_ps.h"

char* OpenAPI_ims_vo_ps_ToString(OpenAPI_ims_vo_ps_e ims_vo_ps)
{
    const char *ims_vo_psArray[] =  { "NULL", "HOMOGENEOUS_SUPPORT", "HOMOGENEOUS_NON_SUPPORT", "NON_HOMOGENEOUS_OR_UNKNOWN" };
    size_t sizeofArray = sizeof(ims_vo_psArray) / sizeof(ims_vo_psArray[0]);
    if (ims_vo_ps < sizeofArray)
        return (char *)ims_vo_psArray[ims_vo_ps];
    else
        return (char *)"Unknown";
}

OpenAPI_ims_vo_ps_e OpenAPI_ims_vo_ps_FromString(char* ims_vo_ps)
{
    int stringToReturn = 0;
    const char *ims_vo_psArray[] =  { "NULL", "HOMOGENEOUS_SUPPORT", "HOMOGENEOUS_NON_SUPPORT", "NON_HOMOGENEOUS_OR_UNKNOWN" };
    size_t sizeofArray = sizeof(ims_vo_psArray) / sizeof(ims_vo_psArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(ims_vo_ps, ims_vo_psArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

