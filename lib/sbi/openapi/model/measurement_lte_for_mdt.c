
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "measurement_lte_for_mdt.h"

char* OpenAPI_measurement_lte_for_mdt_ToString(OpenAPI_measurement_lte_for_mdt_e measurement_lte_for_mdt)
{
    const char *measurement_lte_for_mdtArray[] =  { "NULL", "M1", "M2", "M3", "M4_DL", "M4_UL", "M5_DL", "M5_UL", "M6_DL", "M6_UL", "M7_DL", "M7_UL", "M8", "M9" };
    size_t sizeofArray = sizeof(measurement_lte_for_mdtArray) / sizeof(measurement_lte_for_mdtArray[0]);
    if (measurement_lte_for_mdt < sizeofArray)
        return (char *)measurement_lte_for_mdtArray[measurement_lte_for_mdt];
    else
        return (char *)"Unknown";
}

OpenAPI_measurement_lte_for_mdt_e OpenAPI_measurement_lte_for_mdt_FromString(char* measurement_lte_for_mdt)
{
    int stringToReturn = 0;
    const char *measurement_lte_for_mdtArray[] =  { "NULL", "M1", "M2", "M3", "M4_DL", "M4_UL", "M5_DL", "M5_UL", "M6_DL", "M6_UL", "M7_DL", "M7_UL", "M8", "M9" };
    size_t sizeofArray = sizeof(measurement_lte_for_mdtArray) / sizeof(measurement_lte_for_mdtArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(measurement_lte_for_mdt, measurement_lte_for_mdtArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

