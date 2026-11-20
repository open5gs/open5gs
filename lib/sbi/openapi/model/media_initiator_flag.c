
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "media_initiator_flag.h"

char* OpenAPI_media_initiator_flag_ToString(OpenAPI_media_initiator_flag_e media_initiator_flag)
{
    const char *media_initiator_flagArray[] =  { "NULL", "CALLED_PARTY", "CALLING_PARTY", "UNKNOWN" };
    size_t sizeofArray = sizeof(media_initiator_flagArray) / sizeof(media_initiator_flagArray[0]);
    if (media_initiator_flag < sizeofArray)
        return (char *)media_initiator_flagArray[media_initiator_flag];
    else
        return (char *)"Unknown";
}

OpenAPI_media_initiator_flag_e OpenAPI_media_initiator_flag_FromString(char* media_initiator_flag)
{
    int stringToReturn = 0;
    const char *media_initiator_flagArray[] =  { "NULL", "CALLED_PARTY", "CALLING_PARTY", "UNKNOWN" };
    size_t sizeofArray = sizeof(media_initiator_flagArray) / sizeof(media_initiator_flagArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(media_initiator_flag, media_initiator_flagArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

