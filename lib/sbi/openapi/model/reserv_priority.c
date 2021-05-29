
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "reserv_priority.h"

char* OpenAPI_reserv_priority_ToString(OpenAPI_reserv_priority_e reserv_priority)
{
    const char *reserv_priorityArray[] =  { "NULL", "PRIO_1", "PRIO_2", "PRIO_3", "PRIO_4", "PRIO_5", "PRIO_6", "PRIO_7", "PRIO_8", "PRIO_9", "PRIO_10", "PRIO_11", "PRIO_12", "PRIO_13", "PRIO_14", "PRIO_15", "PRIO_16" };
    size_t sizeofArray = sizeof(reserv_priorityArray) / sizeof(reserv_priorityArray[0]);
    if (reserv_priority < sizeofArray)
        return (char *)reserv_priorityArray[reserv_priority];
    else
        return (char *)"Unknown";
}

OpenAPI_reserv_priority_e OpenAPI_reserv_priority_FromString(char* reserv_priority)
{
    int stringToReturn = 0;
    const char *reserv_priorityArray[] =  { "NULL", "PRIO_1", "PRIO_2", "PRIO_3", "PRIO_4", "PRIO_5", "PRIO_6", "PRIO_7", "PRIO_8", "PRIO_9", "PRIO_10", "PRIO_11", "PRIO_12", "PRIO_13", "PRIO_14", "PRIO_15", "PRIO_16" };
    size_t sizeofArray = sizeof(reserv_priorityArray) / sizeof(reserv_priorityArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(reserv_priority, reserv_priorityArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

