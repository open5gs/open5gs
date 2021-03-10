
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "collection_period_rmm_nr_mdt.h"

char* OpenAPI_collection_period_rmm_nr_mdt_ToString(OpenAPI_collection_period_rmm_nr_mdt_e collection_period_rmm_nr_mdt)
{
    const char *collection_period_rmm_nr_mdtArray[] =  { "NULL", "_1024", "_2048", "_5120", "_10240", "_60000" };
    size_t sizeofArray = sizeof(collection_period_rmm_nr_mdtArray) / sizeof(collection_period_rmm_nr_mdtArray[0]);
    if (collection_period_rmm_nr_mdt < sizeofArray)
        return (char *)collection_period_rmm_nr_mdtArray[collection_period_rmm_nr_mdt];
    else
        return (char *)"Unknown";
}

OpenAPI_collection_period_rmm_nr_mdt_e OpenAPI_collection_period_rmm_nr_mdt_FromString(char* collection_period_rmm_nr_mdt)
{
    int stringToReturn = 0;
    const char *collection_period_rmm_nr_mdtArray[] =  { "NULL", "_1024", "_2048", "_5120", "_10240", "_60000" };
    size_t sizeofArray = sizeof(collection_period_rmm_nr_mdtArray) / sizeof(collection_period_rmm_nr_mdtArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(collection_period_rmm_nr_mdt, collection_period_rmm_nr_mdtArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

