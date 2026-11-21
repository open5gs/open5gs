
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ims_node_functionality.h"

char* OpenAPI_ims_node_functionality_ToString(OpenAPI_ims_node_functionality_e ims_node_functionality)
{
    const char *ims_node_functionalityArray[] =  { "NULL", "S_CSCF", "P_CSCF", "I_CSCF", "MRFC", "MGCF", "BGCF", "AS", "IBCF", "S-GW", "P-GW", "HSGW", "E-CSCF", "MME", "TRF", "TF", "ATCF", "PROXY", "EPDG", "TDF", "TWAG", "SCEF", "IWK_SCEF", "IMS_GWF", "DCSF" };
    size_t sizeofArray = sizeof(ims_node_functionalityArray) / sizeof(ims_node_functionalityArray[0]);
    if (ims_node_functionality < sizeofArray)
        return (char *)ims_node_functionalityArray[ims_node_functionality];
    else
        return (char *)"Unknown";
}

OpenAPI_ims_node_functionality_e OpenAPI_ims_node_functionality_FromString(char* ims_node_functionality)
{
    int stringToReturn = 0;
    const char *ims_node_functionalityArray[] =  { "NULL", "S_CSCF", "P_CSCF", "I_CSCF", "MRFC", "MGCF", "BGCF", "AS", "IBCF", "S-GW", "P-GW", "HSGW", "E-CSCF", "MME", "TRF", "TF", "ATCF", "PROXY", "EPDG", "TDF", "TWAG", "SCEF", "IWK_SCEF", "IMS_GWF", "DCSF" };
    size_t sizeofArray = sizeof(ims_node_functionalityArray) / sizeof(ims_node_functionalityArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(ims_node_functionality, ims_node_functionalityArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

