
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "preemption_control_information.h"

char* OpenAPI_preemption_control_information_ToString(OpenAPI_preemption_control_information_e preemption_control_information)
{
    const char *preemption_control_informationArray[] =  { "NULL", "MOST_RECENT", "LEAST_RECENT", "HIGHEST_BW" };
    size_t sizeofArray = sizeof(preemption_control_informationArray) / sizeof(preemption_control_informationArray[0]);
    if (preemption_control_information < sizeofArray)
        return (char *)preemption_control_informationArray[preemption_control_information];
    else
        return (char *)"Unknown";
}

OpenAPI_preemption_control_information_e OpenAPI_preemption_control_information_FromString(char* preemption_control_information)
{
    int stringToReturn = 0;
    const char *preemption_control_informationArray[] =  { "NULL", "MOST_RECENT", "LEAST_RECENT", "HIGHEST_BW" };
    size_t sizeofArray = sizeof(preemption_control_informationArray) / sizeof(preemption_control_informationArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(preemption_control_information, preemption_control_informationArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

