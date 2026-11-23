
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "charging_characteristics_selection_mode.h"

char* OpenAPI_charging_characteristics_selection_mode_ToString(OpenAPI_charging_characteristics_selection_mode_e charging_characteristics_selection_mode)
{
    const char *charging_characteristics_selection_modeArray[] =  { "NULL", "HOME_DEFAULT", "ROAMING_DEFAULT", "VISITING_DEFAULT" };
    size_t sizeofArray = sizeof(charging_characteristics_selection_modeArray) / sizeof(charging_characteristics_selection_modeArray[0]);
    if (charging_characteristics_selection_mode < sizeofArray)
        return (char *)charging_characteristics_selection_modeArray[charging_characteristics_selection_mode];
    else
        return (char *)"Unknown";
}

OpenAPI_charging_characteristics_selection_mode_e OpenAPI_charging_characteristics_selection_mode_FromString(char* charging_characteristics_selection_mode)
{
    int stringToReturn = 0;
    const char *charging_characteristics_selection_modeArray[] =  { "NULL", "HOME_DEFAULT", "ROAMING_DEFAULT", "VISITING_DEFAULT" };
    size_t sizeofArray = sizeof(charging_characteristics_selection_modeArray) / sizeof(charging_characteristics_selection_modeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(charging_characteristics_selection_mode, charging_characteristics_selection_modeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

