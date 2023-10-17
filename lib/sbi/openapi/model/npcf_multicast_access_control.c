
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "npcf_multicast_access_control.h"

char* OpenAPI_npcf_multicast_access_control_ToString(OpenAPI_npcf_multicast_access_control_e npcf_multicast_access_control)
{
    const char *npcf_multicast_access_controlArray[] =  { "NULL", "ALLOWED", "NOT_ALLOWED" };
    size_t sizeofArray = sizeof(npcf_multicast_access_controlArray) / sizeof(npcf_multicast_access_controlArray[0]);
    if (npcf_multicast_access_control < sizeofArray)
        return (char *)npcf_multicast_access_controlArray[npcf_multicast_access_control];
    else
        return (char *)"Unknown";
}

OpenAPI_npcf_multicast_access_control_e OpenAPI_npcf_multicast_access_control_FromString(char* npcf_multicast_access_control)
{
    int stringToReturn = 0;
    const char *npcf_multicast_access_controlArray[] =  { "NULL", "ALLOWED", "NOT_ALLOWED" };
    size_t sizeofArray = sizeof(npcf_multicast_access_controlArray) / sizeof(npcf_multicast_access_controlArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(npcf_multicast_access_control, npcf_multicast_access_controlArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

