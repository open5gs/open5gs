
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "output_strategy.h"

char* OpenAPI_output_strategy_ToString(OpenAPI_output_strategy_e output_strategy)
{
    const char *output_strategyArray[] =  { "NULL", "BINARY", "GRADIENT" };
    size_t sizeofArray = sizeof(output_strategyArray) / sizeof(output_strategyArray[0]);
    if (output_strategy < sizeofArray)
        return (char *)output_strategyArray[output_strategy];
    else
        return (char *)"Unknown";
}

OpenAPI_output_strategy_e OpenAPI_output_strategy_FromString(char* output_strategy)
{
    int stringToReturn = 0;
    const char *output_strategyArray[] =  { "NULL", "BINARY", "GRADIENT" };
    size_t sizeofArray = sizeof(output_strategyArray) / sizeof(output_strategyArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(output_strategy, output_strategyArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

