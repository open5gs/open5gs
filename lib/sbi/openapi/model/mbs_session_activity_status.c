
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mbs_session_activity_status.h"

char* OpenAPI_mbs_session_activity_status_ToString(OpenAPI_mbs_session_activity_status_e mbs_session_activity_status)
{
    const char *mbs_session_activity_statusArray[] =  { "NULL", "ACTIVE", "INACTIVE" };
    size_t sizeofArray = sizeof(mbs_session_activity_statusArray) / sizeof(mbs_session_activity_statusArray[0]);
    if (mbs_session_activity_status < sizeofArray)
        return (char *)mbs_session_activity_statusArray[mbs_session_activity_status];
    else
        return (char *)"Unknown";
}

OpenAPI_mbs_session_activity_status_e OpenAPI_mbs_session_activity_status_FromString(char* mbs_session_activity_status)
{
    int stringToReturn = 0;
    const char *mbs_session_activity_statusArray[] =  { "NULL", "ACTIVE", "INACTIVE" };
    size_t sizeofArray = sizeof(mbs_session_activity_statusArray) / sizeof(mbs_session_activity_statusArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(mbs_session_activity_status, mbs_session_activity_statusArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

