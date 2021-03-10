
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dl_data_delivery_status.h"

char* OpenAPI_dl_data_delivery_status_ToString(OpenAPI_dl_data_delivery_status_e dl_data_delivery_status)
{
    const char *dl_data_delivery_statusArray[] =  { "NULL", "BUFFERED", "TRANSMITTED", "DISCARDED" };
    size_t sizeofArray = sizeof(dl_data_delivery_statusArray) / sizeof(dl_data_delivery_statusArray[0]);
    if (dl_data_delivery_status < sizeofArray)
        return (char *)dl_data_delivery_statusArray[dl_data_delivery_status];
    else
        return (char *)"Unknown";
}

OpenAPI_dl_data_delivery_status_e OpenAPI_dl_data_delivery_status_FromString(char* dl_data_delivery_status)
{
    int stringToReturn = 0;
    const char *dl_data_delivery_statusArray[] =  { "NULL", "BUFFERED", "TRANSMITTED", "DISCARDED" };
    size_t sizeofArray = sizeof(dl_data_delivery_statusArray) / sizeof(dl_data_delivery_statusArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(dl_data_delivery_status, dl_data_delivery_statusArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

