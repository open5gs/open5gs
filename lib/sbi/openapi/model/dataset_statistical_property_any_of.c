
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dataset_statistical_property_any_of.h"

char* OpenAPI_dataset_statistical_property_any_of_ToString(OpenAPI_dataset_statistical_property_any_of_e dataset_statistical_property_any_of)
{
    const char *dataset_statistical_property_any_ofArray[] =  { "NULL", "UNIFORM_DIST_DATA", "NO_OUTLIERS" };
    size_t sizeofArray = sizeof(dataset_statistical_property_any_ofArray) / sizeof(dataset_statistical_property_any_ofArray[0]);
    if (dataset_statistical_property_any_of < sizeofArray)
        return (char *)dataset_statistical_property_any_ofArray[dataset_statistical_property_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_dataset_statistical_property_any_of_e OpenAPI_dataset_statistical_property_any_of_FromString(char* dataset_statistical_property_any_of)
{
    int stringToReturn = 0;
    const char *dataset_statistical_property_any_ofArray[] =  { "NULL", "UNIFORM_DIST_DATA", "NO_OUTLIERS" };
    size_t sizeofArray = sizeof(dataset_statistical_property_any_ofArray) / sizeof(dataset_statistical_property_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(dataset_statistical_property_any_of, dataset_statistical_property_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

