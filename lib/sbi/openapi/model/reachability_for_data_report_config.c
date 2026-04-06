
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "reachability_for_data_report_config.h"

char* OpenAPI_reachability_for_data_report_config_ToString(OpenAPI_reachability_for_data_report_config_e reachability_for_data_report_config)
{
    const char *reachability_for_data_report_configArray[] =  { "NULL", "DIRECT_REPORT", "INDIRECT_REPORT" };
    size_t sizeofArray = sizeof(reachability_for_data_report_configArray) / sizeof(reachability_for_data_report_configArray[0]);
    if (reachability_for_data_report_config < sizeofArray)
        return (char *)reachability_for_data_report_configArray[reachability_for_data_report_config];
    else
        return (char *)"Unknown";
}

OpenAPI_reachability_for_data_report_config_e OpenAPI_reachability_for_data_report_config_FromString(char* reachability_for_data_report_config)
{
    int stringToReturn = 0;
    const char *reachability_for_data_report_configArray[] =  { "NULL", "DIRECT_REPORT", "INDIRECT_REPORT" };
    size_t sizeofArray = sizeof(reachability_for_data_report_configArray) / sizeof(reachability_for_data_report_configArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(reachability_for_data_report_config, reachability_for_data_report_configArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

