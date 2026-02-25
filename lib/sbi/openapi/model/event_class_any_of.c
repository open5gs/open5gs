
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "event_class_any_of.h"

char* OpenAPI_event_class_any_of_ToString(OpenAPI_event_class_any_of_e event_class_any_of)
{
    const char *event_class_any_ofArray[] =  { "NULL", "SUPPLEMENTARY_SERVICES" };
    size_t sizeofArray = sizeof(event_class_any_ofArray) / sizeof(event_class_any_ofArray[0]);
    if (event_class_any_of < sizeofArray)
        return (char *)event_class_any_ofArray[event_class_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_event_class_any_of_e OpenAPI_event_class_any_of_FromString(char* event_class_any_of)
{
    int stringToReturn = 0;
    const char *event_class_any_ofArray[] =  { "NULL", "SUPPLEMENTARY_SERVICES" };
    size_t sizeofArray = sizeof(event_class_any_ofArray) / sizeof(event_class_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(event_class_any_of, event_class_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

