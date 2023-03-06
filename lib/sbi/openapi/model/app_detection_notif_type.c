
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "app_detection_notif_type.h"

char* OpenAPI_app_detection_notif_type_ToString(OpenAPI_app_detection_notif_type_e app_detection_notif_type)
{
    const char *app_detection_notif_typeArray[] =  { "NULL", "APP_START", "APP_STOP" };
    size_t sizeofArray = sizeof(app_detection_notif_typeArray) / sizeof(app_detection_notif_typeArray[0]);
    if (app_detection_notif_type < sizeofArray)
        return (char *)app_detection_notif_typeArray[app_detection_notif_type];
    else
        return (char *)"Unknown";
}

OpenAPI_app_detection_notif_type_e OpenAPI_app_detection_notif_type_FromString(char* app_detection_notif_type)
{
    int stringToReturn = 0;
    const char *app_detection_notif_typeArray[] =  { "NULL", "APP_START", "APP_STOP" };
    size_t sizeofArray = sizeof(app_detection_notif_typeArray) / sizeof(app_detection_notif_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(app_detection_notif_type, app_detection_notif_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

