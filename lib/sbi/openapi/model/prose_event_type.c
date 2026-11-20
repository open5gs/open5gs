
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "prose_event_type.h"

char* OpenAPI_prose_event_type_ToString(OpenAPI_prose_event_type_e prose_event_type)
{
    const char *prose_event_typeArray[] =  { "NULL", "ANNOUNCING", "MONITORING", "MATCH_REPORT" };
    size_t sizeofArray = sizeof(prose_event_typeArray) / sizeof(prose_event_typeArray[0]);
    if (prose_event_type < sizeofArray)
        return (char *)prose_event_typeArray[prose_event_type];
    else
        return (char *)"Unknown";
}

OpenAPI_prose_event_type_e OpenAPI_prose_event_type_FromString(char* prose_event_type)
{
    int stringToReturn = 0;
    const char *prose_event_typeArray[] =  { "NULL", "ANNOUNCING", "MONITORING", "MATCH_REPORT" };
    size_t sizeofArray = sizeof(prose_event_typeArray) / sizeof(prose_event_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(prose_event_type, prose_event_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

