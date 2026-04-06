
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "xr_device_with2_rx.h"

char* OpenAPI_xr_device_with2_rx_ToString(OpenAPI_xr_device_with2_rx_e xr_device_with2_rx)
{
    const char *xr_device_with2_rxArray[] =  { "NULL", "true" };
    size_t sizeofArray = sizeof(xr_device_with2_rxArray) / sizeof(xr_device_with2_rxArray[0]);
    if (xr_device_with2_rx < sizeofArray)
        return (char *)xr_device_with2_rxArray[xr_device_with2_rx];
    else
        return (char *)"Unknown";
}

OpenAPI_xr_device_with2_rx_e OpenAPI_xr_device_with2_rx_FromString(char* xr_device_with2_rx)
{
    int stringToReturn = 0;
    const char *xr_device_with2_rxArray[] =  { "NULL", "true" };
    size_t sizeofArray = sizeof(xr_device_with2_rxArray) / sizeof(xr_device_with2_rxArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(xr_device_with2_rx, xr_device_with2_rxArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

