
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "role_of_ue.h"

char* OpenAPI_role_of_ue_ToString(OpenAPI_role_of_ue_e role_of_ue)
{
    const char *role_of_ueArray[] =  { "NULL", "ANNOUNCING_UE", "MONITORING_UE", "REQUESTOR_UE", "REQUESTED_UE" };
    size_t sizeofArray = sizeof(role_of_ueArray) / sizeof(role_of_ueArray[0]);
    if (role_of_ue < sizeofArray)
        return (char *)role_of_ueArray[role_of_ue];
    else
        return (char *)"Unknown";
}

OpenAPI_role_of_ue_e OpenAPI_role_of_ue_FromString(char* role_of_ue)
{
    int stringToReturn = 0;
    const char *role_of_ueArray[] =  { "NULL", "ANNOUNCING_UE", "MONITORING_UE", "REQUESTOR_UE", "REQUESTED_UE" };
    size_t sizeofArray = sizeof(role_of_ueArray) / sizeof(role_of_ueArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(role_of_ue, role_of_ueArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

