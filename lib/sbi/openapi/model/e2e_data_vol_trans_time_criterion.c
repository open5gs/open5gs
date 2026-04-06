
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "e2e_data_vol_trans_time_criterion.h"

char* OpenAPI_e2e_data_vol_trans_time_criterion_ToString(OpenAPI_e2e_data_vol_trans_time_criterion_e e2e_data_vol_trans_time_criterion)
{
    const char *e2e_data_vol_trans_time_criterionArray[] =  { "NULL", "E2E_DATA_VOL_TRANS_TIME" };
    size_t sizeofArray = sizeof(e2e_data_vol_trans_time_criterionArray) / sizeof(e2e_data_vol_trans_time_criterionArray[0]);
    if (e2e_data_vol_trans_time_criterion < sizeofArray)
        return (char *)e2e_data_vol_trans_time_criterionArray[e2e_data_vol_trans_time_criterion];
    else
        return (char *)"Unknown";
}

OpenAPI_e2e_data_vol_trans_time_criterion_e OpenAPI_e2e_data_vol_trans_time_criterion_FromString(char* e2e_data_vol_trans_time_criterion)
{
    int stringToReturn = 0;
    const char *e2e_data_vol_trans_time_criterionArray[] =  { "NULL", "E2E_DATA_VOL_TRANS_TIME" };
    size_t sizeofArray = sizeof(e2e_data_vol_trans_time_criterionArray) / sizeof(e2e_data_vol_trans_time_criterionArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(e2e_data_vol_trans_time_criterion, e2e_data_vol_trans_time_criterionArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

