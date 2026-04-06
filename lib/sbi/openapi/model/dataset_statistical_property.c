
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dataset_statistical_property.h"

char* OpenAPI_dataset_statistical_property_ToString(OpenAPI_dataset_statistical_property_e dataset_statistical_property)
{
    const char *dataset_statistical_propertyArray[] =  { "NULL", "UNIFORM_DIST_DATA", "NO_OUTLIERS" };
    size_t sizeofArray = sizeof(dataset_statistical_propertyArray) / sizeof(dataset_statistical_propertyArray[0]);
    if (dataset_statistical_property < sizeofArray)
        return (char *)dataset_statistical_propertyArray[dataset_statistical_property];
    else
        return (char *)"Unknown";
}

OpenAPI_dataset_statistical_property_e OpenAPI_dataset_statistical_property_FromString(char* dataset_statistical_property)
{
    int stringToReturn = 0;
    const char *dataset_statistical_propertyArray[] =  { "NULL", "UNIFORM_DIST_DATA", "NO_OUTLIERS" };
    size_t sizeofArray = sizeof(dataset_statistical_propertyArray) / sizeof(dataset_statistical_propertyArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(dataset_statistical_property, dataset_statistical_propertyArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

