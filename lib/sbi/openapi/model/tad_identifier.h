/*
 * tad_identifier.h
 *
 * 
 */

#ifndef _OpenAPI_tad_identifier_H_
#define _OpenAPI_tad_identifier_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_tad_identifier_NULL = 0, OpenAPI_tad_identifier_CS, OpenAPI_tad_identifier_PS } OpenAPI_tad_identifier_e;

char* OpenAPI_tad_identifier_ToString(OpenAPI_tad_identifier_e tad_identifier);

OpenAPI_tad_identifier_e OpenAPI_tad_identifier_FromString(char* tad_identifier);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_tad_identifier_H_ */

