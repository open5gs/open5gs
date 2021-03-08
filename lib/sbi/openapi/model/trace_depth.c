
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "trace_depth.h"

char* OpenAPI_trace_depth_ToString(OpenAPI_trace_depth_e trace_depth)
{
    const char *trace_depthArray[] =  { "NULL", "MINIMUM", "MEDIUM", "MAXIMUM", "MINIMUM_WO_VENDOR_EXTENSION", "MEDIUM_WO_VENDOR_EXTENSION", "MAXIMUM_WO_VENDOR_EXTENSION" };
    size_t sizeofArray = sizeof(trace_depthArray) / sizeof(trace_depthArray[0]);
    if (trace_depth < sizeofArray)
        return (char *)trace_depthArray[trace_depth];
    else
        return (char *)"Unknown";
}

OpenAPI_trace_depth_e OpenAPI_trace_depth_FromString(char* trace_depth)
{
    int stringToReturn = 0;
    const char *trace_depthArray[] =  { "NULL", "MINIMUM", "MEDIUM", "MAXIMUM", "MINIMUM_WO_VENDOR_EXTENSION", "MEDIUM_WO_VENDOR_EXTENSION", "MAXIMUM_WO_VENDOR_EXTENSION" };
    size_t sizeofArray = sizeof(trace_depthArray) / sizeof(trace_depthArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(trace_depth, trace_depthArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

