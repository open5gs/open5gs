
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "shared_data_treatment_instruction.h"

char* OpenAPI_shared_data_treatment_instruction_ToString(OpenAPI_shared_data_treatment_instruction_e shared_data_treatment_instruction)
{
    const char *shared_data_treatment_instructionArray[] =  { "NULL", "USE_IF_NO_CLASH", "OVERWRITE", "MAX", "MIN" };
    size_t sizeofArray = sizeof(shared_data_treatment_instructionArray) / sizeof(shared_data_treatment_instructionArray[0]);
    if (shared_data_treatment_instruction < sizeofArray)
        return (char *)shared_data_treatment_instructionArray[shared_data_treatment_instruction];
    else
        return (char *)"Unknown";
}

OpenAPI_shared_data_treatment_instruction_e OpenAPI_shared_data_treatment_instruction_FromString(char* shared_data_treatment_instruction)
{
    int stringToReturn = 0;
    const char *shared_data_treatment_instructionArray[] =  { "NULL", "USE_IF_NO_CLASH", "OVERWRITE", "MAX", "MIN" };
    size_t sizeofArray = sizeof(shared_data_treatment_instructionArray) / sizeof(shared_data_treatment_instructionArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(shared_data_treatment_instruction, shared_data_treatment_instructionArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

