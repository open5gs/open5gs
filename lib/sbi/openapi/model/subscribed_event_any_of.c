
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "subscribed_event_any_of.h"

char* OpenAPI_subscribed_event_any_of_ToString(OpenAPI_subscribed_event_any_of_e subscribed_event_any_of)
{
    const char *subscribed_event_any_ofArray[] =  { "NULL", "UP_PATH_CHANGE" };
    size_t sizeofArray = sizeof(subscribed_event_any_ofArray) / sizeof(subscribed_event_any_ofArray[0]);
    if (subscribed_event_any_of < sizeofArray)
        return (char *)subscribed_event_any_ofArray[subscribed_event_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_subscribed_event_any_of_e OpenAPI_subscribed_event_any_of_FromString(char* subscribed_event_any_of)
{
    int stringToReturn = 0;
    const char *subscribed_event_any_ofArray[] =  { "NULL", "UP_PATH_CHANGE" };
    size_t sizeofArray = sizeof(subscribed_event_any_ofArray) / sizeof(subscribed_event_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(subscribed_event_any_of, subscribed_event_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

