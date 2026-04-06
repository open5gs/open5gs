
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pc_event.h"

char* OpenAPI_pc_event_ToString(OpenAPI_pc_event_e pc_event)
{
    const char *pc_eventArray[] =  { "NULL", "AC_TY_CH", "PLMN_CH", "SAC_CH", "SAT_CATEGORY_CH", "SUCCESS_UE_POL_DEL_SP", "UNSUCCESS_UE_POL_DEL_SP", "UNSUCCESS_PCF_SERVICE_AUTHORIZATION", "APPLICATION_START", "APPLICATION_STOP", "RATE_LIMIT_INFO_REPO", "SIGNALLING_INFO", "SLICE_REPLACE_OUTCOME", "PARTLY_UNSUCC_UE_POL_DEL_SP" };
    size_t sizeofArray = sizeof(pc_eventArray) / sizeof(pc_eventArray[0]);
    if (pc_event < sizeofArray)
        return (char *)pc_eventArray[pc_event];
    else
        return (char *)"Unknown";
}

OpenAPI_pc_event_e OpenAPI_pc_event_FromString(char* pc_event)
{
    int stringToReturn = 0;
    const char *pc_eventArray[] =  { "NULL", "AC_TY_CH", "PLMN_CH", "SAC_CH", "SAT_CATEGORY_CH", "SUCCESS_UE_POL_DEL_SP", "UNSUCCESS_UE_POL_DEL_SP", "UNSUCCESS_PCF_SERVICE_AUTHORIZATION", "APPLICATION_START", "APPLICATION_STOP", "RATE_LIMIT_INFO_REPO", "SIGNALLING_INFO", "SLICE_REPLACE_OUTCOME", "PARTLY_UNSUCC_UE_POL_DEL_SP" };
    size_t sizeofArray = sizeof(pc_eventArray) / sizeof(pc_eventArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(pc_event, pc_eventArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

