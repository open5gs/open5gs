
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "up_loc_rep_ind_af.h"

char* OpenAPI_up_loc_rep_ind_af_ToString(OpenAPI_up_loc_rep_ind_af_e up_loc_rep_ind_af)
{
    const char *up_loc_rep_ind_afArray[] =  { "NULL", "USER_PLANE_REPORT_ALLOWED", "USER_PLANE_REPORT_NOT_ALLOWED" };
    size_t sizeofArray = sizeof(up_loc_rep_ind_afArray) / sizeof(up_loc_rep_ind_afArray[0]);
    if (up_loc_rep_ind_af < sizeofArray)
        return (char *)up_loc_rep_ind_afArray[up_loc_rep_ind_af];
    else
        return (char *)"Unknown";
}

OpenAPI_up_loc_rep_ind_af_e OpenAPI_up_loc_rep_ind_af_FromString(char* up_loc_rep_ind_af)
{
    int stringToReturn = 0;
    const char *up_loc_rep_ind_afArray[] =  { "NULL", "USER_PLANE_REPORT_ALLOWED", "USER_PLANE_REPORT_NOT_ALLOWED" };
    size_t sizeofArray = sizeof(up_loc_rep_ind_afArray) / sizeof(up_loc_rep_ind_afArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(up_loc_rep_ind_af, up_loc_rep_ind_afArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

