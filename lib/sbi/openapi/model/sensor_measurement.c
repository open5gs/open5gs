
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sensor_measurement.h"

char* OpenAPI_sensor_measurement_ToString(OpenAPI_sensor_measurement_e sensor_measurement)
{
    const char *sensor_measurementArray[] =  { "NULL", "BAROMETRIC_PRESSURE", "UE_SPEED", "UE_ORIENTATION" };
    size_t sizeofArray = sizeof(sensor_measurementArray) / sizeof(sensor_measurementArray[0]);
    if (sensor_measurement < sizeofArray)
        return (char *)sensor_measurementArray[sensor_measurement];
    else
        return (char *)"Unknown";
}

OpenAPI_sensor_measurement_e OpenAPI_sensor_measurement_FromString(char* sensor_measurement)
{
    int stringToReturn = 0;
    const char *sensor_measurementArray[] =  { "NULL", "BAROMETRIC_PRESSURE", "UE_SPEED", "UE_ORIENTATION" };
    size_t sizeofArray = sizeof(sensor_measurementArray) / sizeof(sensor_measurementArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(sensor_measurement, sensor_measurementArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

