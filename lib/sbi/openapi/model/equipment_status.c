
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "equipment_status.h"

char* OpenAPI_equipment_status_ToString(OpenAPI_equipment_status_e equipment_status)
{
    const char *equipment_statusArray[] =  { "NULL", "WHITELISTED", "BLACKLISTED", "GREYLISTED" };
    size_t sizeofArray = sizeof(equipment_statusArray) / sizeof(equipment_statusArray[0]);
    if (equipment_status < sizeofArray)
        return (char *)equipment_statusArray[equipment_status];
    else
        return (char *)"Unknown";
}

OpenAPI_equipment_status_e OpenAPI_equipment_status_FromString(char* equipment_status)
{
    int stringToReturn = 0;
    const char *equipment_statusArray[] =  { "NULL", "WHITELISTED", "BLACKLISTED", "GREYLISTED" };
    size_t sizeofArray = sizeof(equipment_statusArray) / sizeof(equipment_statusArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(equipment_status, equipment_statusArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

