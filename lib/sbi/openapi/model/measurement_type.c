
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "measurement_type.h"

char* OpenAPI_measurement_type_ToString(OpenAPI_measurement_type_e measurement_type)
{
    const char *measurement_typeArray[] =  { "NULL", "GTP_DELAYDLPSAUPFUEMEAN_SNSSAI_QFI", "GTP_DELAYULPSAUPFUEMEANEXCD1_SNSSAI_QFI", "GTP_DELAYDLPSAUPFUEMEANINCD1_SNSSAI_QFI", "GTP_DELAYULPSAUPFNGRANMEAN_SNSSAI_QFI", "GTP_DELAYDLPSAUPFNGRANMEAN_SNSSAI_QFI", "GTP_RTTDELAYPSAUPFNGRANMEAN", "GTP_RTTDELAYPSAUPFNGRANMEAN_SNSSAI", "GTP_RTTDELAYPSAUPFNGRANMEAN_QFI", "GTP_RTTDELAYPSAUPFNGRANMEAN_SNSSAI_QFI" };
    size_t sizeofArray = sizeof(measurement_typeArray) / sizeof(measurement_typeArray[0]);
    if (measurement_type < sizeofArray)
        return (char *)measurement_typeArray[measurement_type];
    else
        return (char *)"Unknown";
}

OpenAPI_measurement_type_e OpenAPI_measurement_type_FromString(char* measurement_type)
{
    int stringToReturn = 0;
    const char *measurement_typeArray[] =  { "NULL", "GTP_DELAYDLPSAUPFUEMEAN_SNSSAI_QFI", "GTP_DELAYULPSAUPFUEMEANEXCD1_SNSSAI_QFI", "GTP_DELAYDLPSAUPFUEMEANINCD1_SNSSAI_QFI", "GTP_DELAYULPSAUPFNGRANMEAN_SNSSAI_QFI", "GTP_DELAYDLPSAUPFNGRANMEAN_SNSSAI_QFI", "GTP_RTTDELAYPSAUPFNGRANMEAN", "GTP_RTTDELAYPSAUPFNGRANMEAN_SNSSAI", "GTP_RTTDELAYPSAUPFNGRANMEAN_QFI", "GTP_RTTDELAYPSAUPFNGRANMEAN_SNSSAI_QFI" };
    size_t sizeofArray = sizeof(measurement_typeArray) / sizeof(measurement_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(measurement_type, measurement_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

