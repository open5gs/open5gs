
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "node_functionality.h"

char* OpenAPI_node_functionality_ToString(OpenAPI_node_functionality_e node_functionality)
{
    const char *node_functionalityArray[] =  { "NULL", "AMF", "SMF", "SMS", "SMSF", "PGW_C_SMF", "NEFF", "SGW", "I_SMF", "ePDG", "CEF", "NEF", "MnS_Producer", "SGSN", "V_SMF", "5G_DDNMF", "IMS_Node", "MMS_Node", "EES", "PCF", "UDM", "UPF", "TSN_AF", "TSCTSF", "MB_SMF", "CHF", "GMLC", "AIOTF", "CCF", "NWDAF" };
    size_t sizeofArray = sizeof(node_functionalityArray) / sizeof(node_functionalityArray[0]);
    if (node_functionality < sizeofArray)
        return (char *)node_functionalityArray[node_functionality];
    else
        return (char *)"Unknown";
}

OpenAPI_node_functionality_e OpenAPI_node_functionality_FromString(char* node_functionality)
{
    int stringToReturn = 0;
    const char *node_functionalityArray[] =  { "NULL", "AMF", "SMF", "SMS", "SMSF", "PGW_C_SMF", "NEFF", "SGW", "I_SMF", "ePDG", "CEF", "NEF", "MnS_Producer", "SGSN", "V_SMF", "5G_DDNMF", "IMS_Node", "MMS_Node", "EES", "PCF", "UDM", "UPF", "TSN_AF", "TSCTSF", "MB_SMF", "CHF", "GMLC", "AIOTF", "CCF", "NWDAF" };
    size_t sizeofArray = sizeof(node_functionalityArray) / sizeof(node_functionalityArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(node_functionality, node_functionalityArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

