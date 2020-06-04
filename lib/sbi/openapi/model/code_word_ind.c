
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "code_word_ind.h"

char* OpenAPI_code_word_ind_ToString(OpenAPI_code_word_ind_e code_word_ind)
{
    const char *code_word_indArray[] =  { "NULL", "CODEWORD_CHECK_IN_UE", "CODEWORD_CHECK_IN_GMLC" };
    size_t sizeofArray = sizeof(code_word_indArray) / sizeof(code_word_indArray[0]);
    if (code_word_ind < sizeofArray)
        return (char *)code_word_indArray[code_word_ind];
    else
        return (char *)"Unknown";
}

OpenAPI_code_word_ind_e OpenAPI_code_word_ind_FromString(char* code_word_ind)
{
    int stringToReturn = 0;
    const char *code_word_indArray[] =  { "NULL", "CODEWORD_CHECK_IN_UE", "CODEWORD_CHECK_IN_GMLC" };
    size_t sizeofArray = sizeof(code_word_indArray) / sizeof(code_word_indArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(code_word_ind, code_word_indArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

