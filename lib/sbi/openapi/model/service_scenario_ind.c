
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "service_scenario_ind.h"

char* OpenAPI_service_scenario_ind_ToString(OpenAPI_service_scenario_ind_e service_scenario_ind)
{
    const char *service_scenario_indArray[] =  { "NULL", "VOICE_SERVICE", "DATA_SERVICE", "VOICE_AND_DATA_SERVICE", "IOT", "NPN" };
    size_t sizeofArray = sizeof(service_scenario_indArray) / sizeof(service_scenario_indArray[0]);
    if (service_scenario_ind < sizeofArray)
        return (char *)service_scenario_indArray[service_scenario_ind];
    else
        return (char *)"Unknown";
}

OpenAPI_service_scenario_ind_e OpenAPI_service_scenario_ind_FromString(char* service_scenario_ind)
{
    int stringToReturn = 0;
    const char *service_scenario_indArray[] =  { "NULL", "VOICE_SERVICE", "DATA_SERVICE", "VOICE_AND_DATA_SERVICE", "IOT", "NPN" };
    size_t sizeofArray = sizeof(service_scenario_indArray) / sizeof(service_scenario_indArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(service_scenario_ind, service_scenario_indArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

