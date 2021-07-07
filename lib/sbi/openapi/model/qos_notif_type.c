
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "qos_notif_type.h"

char* OpenAPI_qos_notif_type_ToString(OpenAPI_qos_notif_type_e qos_notif_type)
{
    const char *qos_notif_typeArray[] =  { "NULL", "GUARANTEED", "NOT_GUARANTEED" };
    size_t sizeofArray = sizeof(qos_notif_typeArray) / sizeof(qos_notif_typeArray[0]);
    if (qos_notif_type < sizeofArray)
        return (char *)qos_notif_typeArray[qos_notif_type];
    else
        return (char *)"Unknown";
}

OpenAPI_qos_notif_type_e OpenAPI_qos_notif_type_FromString(char* qos_notif_type)
{
    int stringToReturn = 0;
    const char *qos_notif_typeArray[] =  { "NULL", "GUARANTEED", "NOT_GUARANTEED" };
    size_t sizeofArray = sizeof(qos_notif_typeArray) / sizeof(qos_notif_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(qos_notif_type, qos_notif_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

