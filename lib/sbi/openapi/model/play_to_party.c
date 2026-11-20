
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "play_to_party.h"

char* OpenAPI_play_to_party_ToString(OpenAPI_play_to_party_e play_to_party)
{
    const char *play_to_partyArray[] =  { "NULL", "SERVED", "REMOTE" };
    size_t sizeofArray = sizeof(play_to_partyArray) / sizeof(play_to_partyArray[0]);
    if (play_to_party < sizeofArray)
        return (char *)play_to_partyArray[play_to_party];
    else
        return (char *)"Unknown";
}

OpenAPI_play_to_party_e OpenAPI_play_to_party_FromString(char* play_to_party)
{
    int stringToReturn = 0;
    const char *play_to_partyArray[] =  { "NULL", "SERVED", "REMOTE" };
    size_t sizeofArray = sizeof(play_to_partyArray) / sizeof(play_to_partyArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(play_to_party, play_to_partyArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

