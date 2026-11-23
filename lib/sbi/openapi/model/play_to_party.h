/*
 * play_to_party.h
 *
 * 
 */

#ifndef _OpenAPI_play_to_party_H_
#define _OpenAPI_play_to_party_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_play_to_party_NULL = 0, OpenAPI_play_to_party_SERVED, OpenAPI_play_to_party_REMOTE } OpenAPI_play_to_party_e;

char* OpenAPI_play_to_party_ToString(OpenAPI_play_to_party_e play_to_party);

OpenAPI_play_to_party_e OpenAPI_play_to_party_FromString(char* play_to_party);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_play_to_party_H_ */

