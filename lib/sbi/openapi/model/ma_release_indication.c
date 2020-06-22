
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ma_release_indication.h"

char* OpenAPI_ma_release_indication_ToString(OpenAPI_ma_release_indication_e ma_release_indication)
{
    const char *ma_release_indicationArray[] =  { "NULL", "REL_MAPDU_OVER_3GPP", "REL_MAPDU_OVER_N3GPP" };
    size_t sizeofArray = sizeof(ma_release_indicationArray) / sizeof(ma_release_indicationArray[0]);
    if (ma_release_indication < sizeofArray)
        return (char *)ma_release_indicationArray[ma_release_indication];
    else
        return (char *)"Unknown";
}

OpenAPI_ma_release_indication_e OpenAPI_ma_release_indication_FromString(char* ma_release_indication)
{
    int stringToReturn = 0;
    const char *ma_release_indicationArray[] =  { "NULL", "REL_MAPDU_OVER_3GPP", "REL_MAPDU_OVER_N3GPP" };
    size_t sizeofArray = sizeof(ma_release_indicationArray) / sizeof(ma_release_indicationArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(ma_release_indication, ma_release_indicationArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

