
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "access_state_transition_type.h"

char* OpenAPI_access_state_transition_type_ToString(OpenAPI_access_state_transition_type_e access_state_transition_type)
{
    const char *access_state_transition_typeArray[] =  { "NULL", "ACCESS_TYPE_CHANGE_3GPP", "ACCESS_TYPE_CHANGE_N3GPP", "RM_STATE_CHANGE_DEREGISTERED", "RM_STATE_CHANGE_REGISTERED", "CM_STATE_CHANGE_IDLE", "CM_STATE_CHANGE_CONNECTED", "HANDOVER", "MOBILITY_REGISTRATION_UPDATE" };
    size_t sizeofArray = sizeof(access_state_transition_typeArray) / sizeof(access_state_transition_typeArray[0]);
    if (access_state_transition_type < sizeofArray)
        return (char *)access_state_transition_typeArray[access_state_transition_type];
    else
        return (char *)"Unknown";
}

OpenAPI_access_state_transition_type_e OpenAPI_access_state_transition_type_FromString(char* access_state_transition_type)
{
    int stringToReturn = 0;
    const char *access_state_transition_typeArray[] =  { "NULL", "ACCESS_TYPE_CHANGE_3GPP", "ACCESS_TYPE_CHANGE_N3GPP", "RM_STATE_CHANGE_DEREGISTERED", "RM_STATE_CHANGE_REGISTERED", "CM_STATE_CHANGE_IDLE", "CM_STATE_CHANGE_CONNECTED", "HANDOVER", "MOBILITY_REGISTRATION_UPDATE" };
    size_t sizeofArray = sizeof(access_state_transition_typeArray) / sizeof(access_state_transition_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(access_state_transition_type, access_state_transition_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

