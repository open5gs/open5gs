
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "direct_discovery_model.h"

char* OpenAPI_direct_discovery_model_ToString(OpenAPI_direct_discovery_model_e direct_discovery_model)
{
    const char *direct_discovery_modelArray[] =  { "NULL", "MODEL_A", "MODEL_B" };
    size_t sizeofArray = sizeof(direct_discovery_modelArray) / sizeof(direct_discovery_modelArray[0]);
    if (direct_discovery_model < sizeofArray)
        return (char *)direct_discovery_modelArray[direct_discovery_model];
    else
        return (char *)"Unknown";
}

OpenAPI_direct_discovery_model_e OpenAPI_direct_discovery_model_FromString(char* direct_discovery_model)
{
    int stringToReturn = 0;
    const char *direct_discovery_modelArray[] =  { "NULL", "MODEL_A", "MODEL_B" };
    size_t sizeofArray = sizeof(direct_discovery_modelArray) / sizeof(direct_discovery_modelArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(direct_discovery_model, direct_discovery_modelArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

