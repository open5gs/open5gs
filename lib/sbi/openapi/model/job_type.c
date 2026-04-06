
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "job_type.h"

char* OpenAPI_job_type_ToString(OpenAPI_job_type_e job_type)
{
    const char *job_typeArray[] =  { "NULL", "IMMEDIATE_MDT_ONLY", "LOGGED_MDT_ONLY", "TRACE_ONLY", "IMMEDIATE_MDT_AND_TRACE", "LOGGED_MBSFN_MDT", "5GC_UE_LEVEL_MEASUREMENTS_ONLY", "TRACE_AND_5GC_UE_LEVEL_MEASUREMENTS_ONLY", "IMMEDIATE_MDT_AND_5GC_UE_LEVEL_MEASUREMENTS", "TRACE_IMMEDIATE_MDT_AND_5GC_UE_LEVEL_MEASUREMENTS" };
    size_t sizeofArray = sizeof(job_typeArray) / sizeof(job_typeArray[0]);
    if (job_type < sizeofArray)
        return (char *)job_typeArray[job_type];
    else
        return (char *)"Unknown";
}

OpenAPI_job_type_e OpenAPI_job_type_FromString(char* job_type)
{
    int stringToReturn = 0;
    const char *job_typeArray[] =  { "NULL", "IMMEDIATE_MDT_ONLY", "LOGGED_MDT_ONLY", "TRACE_ONLY", "IMMEDIATE_MDT_AND_TRACE", "LOGGED_MBSFN_MDT", "5GC_UE_LEVEL_MEASUREMENTS_ONLY", "TRACE_AND_5GC_UE_LEVEL_MEASUREMENTS_ONLY", "IMMEDIATE_MDT_AND_5GC_UE_LEVEL_MEASUREMENTS", "TRACE_IMMEDIATE_MDT_AND_5GC_UE_LEVEL_MEASUREMENTS" };
    size_t sizeofArray = sizeof(job_typeArray) / sizeof(job_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(job_type, job_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

