
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "policy_req_trigger.h"

char* OpenAPI_policy_req_trigger_ToString(OpenAPI_policy_req_trigger_e policy_req_trigger)
{
    const char *policy_req_triggerArray[] =  { "NULL", "LOCATION_CHANGE", "PRA_CHANGE", "ALLOWED_NSSAI_CHANGE", "NWDAF_DATA_CHANGE", "PLMN_CHANGE", "CON_STATE_CHANGE", "SMF_SELECT_CHANGE", "ACCESS_TYPE_CHANGE" };
    size_t sizeofArray = sizeof(policy_req_triggerArray) / sizeof(policy_req_triggerArray[0]);
    if (policy_req_trigger < sizeofArray)
        return (char *)policy_req_triggerArray[policy_req_trigger];
    else
        return (char *)"Unknown";
}

OpenAPI_policy_req_trigger_e OpenAPI_policy_req_trigger_FromString(char* policy_req_trigger)
{
    int stringToReturn = 0;
    const char *policy_req_triggerArray[] =  { "NULL", "LOCATION_CHANGE", "PRA_CHANGE", "ALLOWED_NSSAI_CHANGE", "NWDAF_DATA_CHANGE", "PLMN_CHANGE", "CON_STATE_CHANGE", "SMF_SELECT_CHANGE", "ACCESS_TYPE_CHANGE" };
    size_t sizeofArray = sizeof(policy_req_triggerArray) / sizeof(policy_req_triggerArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(policy_req_trigger, policy_req_triggerArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

