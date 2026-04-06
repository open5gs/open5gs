
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "expected_ue_behaviour_dataset.h"

char* OpenAPI_expected_ue_behaviour_dataset_ToString(OpenAPI_expected_ue_behaviour_dataset_e expected_ue_behaviour_dataset)
{
    const char *expected_ue_behaviour_datasetArray[] =  { "NULL", "STATIONARY_INDICATION", "COMMUNICATION_DURATION_TIME", "PERIODIC_TIME", "SCHEDULED_COMMUNICATION_TIME", "SCHEDULED_COMMUNICATION_TYPE", "EXPECTED_UMTS", "TRAFFIC_PROFILE", "BATTERY_INDICATION", "EXPECTED_INACTIVITY_TIME" };
    size_t sizeofArray = sizeof(expected_ue_behaviour_datasetArray) / sizeof(expected_ue_behaviour_datasetArray[0]);
    if (expected_ue_behaviour_dataset < sizeofArray)
        return (char *)expected_ue_behaviour_datasetArray[expected_ue_behaviour_dataset];
    else
        return (char *)"Unknown";
}

OpenAPI_expected_ue_behaviour_dataset_e OpenAPI_expected_ue_behaviour_dataset_FromString(char* expected_ue_behaviour_dataset)
{
    int stringToReturn = 0;
    const char *expected_ue_behaviour_datasetArray[] =  { "NULL", "STATIONARY_INDICATION", "COMMUNICATION_DURATION_TIME", "PERIODIC_TIME", "SCHEDULED_COMMUNICATION_TIME", "SCHEDULED_COMMUNICATION_TYPE", "EXPECTED_UMTS", "TRAFFIC_PROFILE", "BATTERY_INDICATION", "EXPECTED_INACTIVITY_TIME" };
    size_t sizeofArray = sizeof(expected_ue_behaviour_datasetArray) / sizeof(expected_ue_behaviour_datasetArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(expected_ue_behaviour_dataset, expected_ue_behaviour_datasetArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

