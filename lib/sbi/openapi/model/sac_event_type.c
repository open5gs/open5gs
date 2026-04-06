
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sac_event_type.h"

char* OpenAPI_sac_event_type_ToString(OpenAPI_sac_event_type_e sac_event_type)
{
    const char *sac_event_typeArray[] =  { "NULL", "NUM_OF_REGD_UES", "NUM_OF_ESTD_PDU_SESSIONS" };
    size_t sizeofArray = sizeof(sac_event_typeArray) / sizeof(sac_event_typeArray[0]);
    if (sac_event_type < sizeofArray)
        return (char *)sac_event_typeArray[sac_event_type];
    else
        return (char *)"Unknown";
}

OpenAPI_sac_event_type_e OpenAPI_sac_event_type_FromString(char* sac_event_type)
{
    int stringToReturn = 0;
    const char *sac_event_typeArray[] =  { "NULL", "NUM_OF_REGD_UES", "NUM_OF_ESTD_PDU_SESSIONS" };
    size_t sizeofArray = sizeof(sac_event_typeArray) / sizeof(sac_event_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(sac_event_type, sac_event_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

