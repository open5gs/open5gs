
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "event_for_mdt.h"

char* OpenAPI_event_for_mdt_ToString(OpenAPI_event_for_mdt_e event_for_mdt)
{
    const char *event_for_mdtArray[] =  { "NULL", "OUT_OF_COVERAG", "A2_EVENT" };
    size_t sizeofArray = sizeof(event_for_mdtArray) / sizeof(event_for_mdtArray[0]);
    if (event_for_mdt < sizeofArray)
        return (char *)event_for_mdtArray[event_for_mdt];
    else
        return (char *)"Unknown";
}

OpenAPI_event_for_mdt_e OpenAPI_event_for_mdt_FromString(char* event_for_mdt)
{
    int stringToReturn = 0;
    const char *event_for_mdtArray[] =  { "NULL", "OUT_OF_COVERAG", "A2_EVENT" };
    size_t sizeofArray = sizeof(event_for_mdtArray) / sizeof(event_for_mdtArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(event_for_mdt, event_for_mdtArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

