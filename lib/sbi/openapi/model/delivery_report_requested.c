
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "delivery_report_requested.h"

char* OpenAPI_delivery_report_requested_ToString(OpenAPI_delivery_report_requested_e delivery_report_requested)
{
    const char *delivery_report_requestedArray[] =  { "NULL", "true", "false" };
    size_t sizeofArray = sizeof(delivery_report_requestedArray) / sizeof(delivery_report_requestedArray[0]);
    if (delivery_report_requested < sizeofArray)
        return (char *)delivery_report_requestedArray[delivery_report_requested];
    else
        return (char *)"Unknown";
}

OpenAPI_delivery_report_requested_e OpenAPI_delivery_report_requested_FromString(char* delivery_report_requested)
{
    int stringToReturn = 0;
    const char *delivery_report_requestedArray[] =  { "NULL", "true", "false" };
    size_t sizeofArray = sizeof(delivery_report_requestedArray) / sizeof(delivery_report_requestedArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(delivery_report_requested, delivery_report_requestedArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

