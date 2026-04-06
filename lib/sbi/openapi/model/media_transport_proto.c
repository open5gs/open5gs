
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "media_transport_proto.h"

char* OpenAPI_media_transport_proto_ToString(OpenAPI_media_transport_proto_e media_transport_proto)
{
    const char *media_transport_protoArray[] =  { "NULL", "RTP", "SRTP", "MOQT" };
    size_t sizeofArray = sizeof(media_transport_protoArray) / sizeof(media_transport_protoArray[0]);
    if (media_transport_proto < sizeofArray)
        return (char *)media_transport_protoArray[media_transport_proto];
    else
        return (char *)"Unknown";
}

OpenAPI_media_transport_proto_e OpenAPI_media_transport_proto_FromString(char* media_transport_proto)
{
    int stringToReturn = 0;
    const char *media_transport_protoArray[] =  { "NULL", "RTP", "SRTP", "MOQT" };
    size_t sizeofArray = sizeof(media_transport_protoArray) / sizeof(media_transport_protoArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(media_transport_proto, media_transport_protoArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

