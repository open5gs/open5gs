
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "participant_action_type.h"

char* OpenAPI_participant_action_type_ToString(OpenAPI_participant_action_type_e participant_action_type)
{
    const char *participant_action_typeArray[] =  { "NULL", "CREATE", "JOIN", "INVITE_INTO", "QUIT" };
    size_t sizeofArray = sizeof(participant_action_typeArray) / sizeof(participant_action_typeArray[0]);
    if (participant_action_type < sizeofArray)
        return (char *)participant_action_typeArray[participant_action_type];
    else
        return (char *)"Unknown";
}

OpenAPI_participant_action_type_e OpenAPI_participant_action_type_FromString(char* participant_action_type)
{
    int stringToReturn = 0;
    const char *participant_action_typeArray[] =  { "NULL", "CREATE", "JOIN", "INVITE_INTO", "QUIT" };
    size_t sizeofArray = sizeof(participant_action_typeArray) / sizeof(participant_action_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(participant_action_type, participant_action_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

