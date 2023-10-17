
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "collection_period_rmm_lte_mdt.h"

char* OpenAPI_collection_period_rmm_lte_mdt_ToString(OpenAPI_collection_period_rmm_lte_mdt_e collection_period_rmm_lte_mdt)
{
    const char *collection_period_rmm_lte_mdtArray[] =  { "NULL", "1024", "1280", "2048", "2560", "5120", "10240", "60000" };
    size_t sizeofArray = sizeof(collection_period_rmm_lte_mdtArray) / sizeof(collection_period_rmm_lte_mdtArray[0]);
    if (collection_period_rmm_lte_mdt < sizeofArray)
        return (char *)collection_period_rmm_lte_mdtArray[collection_period_rmm_lte_mdt];
    else
        return (char *)"Unknown";
}

OpenAPI_collection_period_rmm_lte_mdt_e OpenAPI_collection_period_rmm_lte_mdt_FromString(char* collection_period_rmm_lte_mdt)
{
    int stringToReturn = 0;
    const char *collection_period_rmm_lte_mdtArray[] =  { "NULL", "1024", "1280", "2048", "2560", "5120", "10240", "60000" };
    size_t sizeofArray = sizeof(collection_period_rmm_lte_mdtArray) / sizeof(collection_period_rmm_lte_mdtArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(collection_period_rmm_lte_mdt, collection_period_rmm_lte_mdtArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

