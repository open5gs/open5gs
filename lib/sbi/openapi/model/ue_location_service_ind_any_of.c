
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_location_service_ind_any_of.h"

char* OpenAPI_ue_location_service_ind_any_of_ToString(OpenAPI_ue_location_service_ind_any_of_e ue_location_service_ind_any_of)
{
    const char *ue_location_service_ind_any_ofArray[] =  { "NULL", "LOCATION_ESTIMATE", "LOCATION_ASSISTANCE_DATA" };
    size_t sizeofArray = sizeof(ue_location_service_ind_any_ofArray) / sizeof(ue_location_service_ind_any_ofArray[0]);
    if (ue_location_service_ind_any_of < sizeofArray)
        return (char *)ue_location_service_ind_any_ofArray[ue_location_service_ind_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_ue_location_service_ind_any_of_e OpenAPI_ue_location_service_ind_any_of_FromString(char* ue_location_service_ind_any_of)
{
    int stringToReturn = 0;
    const char *ue_location_service_ind_any_ofArray[] =  { "NULL", "LOCATION_ESTIMATE", "LOCATION_ASSISTANCE_DATA" };
    size_t sizeofArray = sizeof(ue_location_service_ind_any_ofArray) / sizeof(ue_location_service_ind_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(ue_location_service_ind_any_of, ue_location_service_ind_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

