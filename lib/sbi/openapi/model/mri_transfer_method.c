
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mri_transfer_method.h"

char* OpenAPI_mri_transfer_method_ToString(OpenAPI_mri_transfer_method_e mri_transfer_method)
{
    const char *mri_transfer_methodArray[] =  { "NULL", "UDP_OPTION" };
    size_t sizeofArray = sizeof(mri_transfer_methodArray) / sizeof(mri_transfer_methodArray[0]);
    if (mri_transfer_method < sizeofArray)
        return (char *)mri_transfer_methodArray[mri_transfer_method];
    else
        return (char *)"Unknown";
}

OpenAPI_mri_transfer_method_e OpenAPI_mri_transfer_method_FromString(char* mri_transfer_method)
{
    int stringToReturn = 0;
    const char *mri_transfer_methodArray[] =  { "NULL", "UDP_OPTION" };
    size_t sizeofArray = sizeof(mri_transfer_methodArray) / sizeof(mri_transfer_methodArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(mri_transfer_method, mri_transfer_methodArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

