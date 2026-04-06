
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "notif_cap.h"

char* OpenAPI_notif_cap_ToString(OpenAPI_notif_cap_e notif_cap)
{
    const char *notif_capArray[] =  { "NULL", "SUPPORTED", "NOT_SUPPORTED" };
    size_t sizeofArray = sizeof(notif_capArray) / sizeof(notif_capArray[0]);
    if (notif_cap < sizeofArray)
        return (char *)notif_capArray[notif_cap];
    else
        return (char *)"Unknown";
}

OpenAPI_notif_cap_e OpenAPI_notif_cap_FromString(char* notif_cap)
{
    int stringToReturn = 0;
    const char *notif_capArray[] =  { "NULL", "SUPPORTED", "NOT_SUPPORTED" };
    size_t sizeofArray = sizeof(notif_capArray) / sizeof(notif_capArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(notif_cap, notif_capArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

