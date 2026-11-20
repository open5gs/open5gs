/*
 * allocate_unit_indicator.h
 *
 * 
 */

#ifndef _OpenAPI_allocate_unit_indicator_H_
#define _OpenAPI_allocate_unit_indicator_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_allocate_unit_indicator_NULL = 0, OpenAPI_allocate_unit_indicator_CHF_DETERMINED, OpenAPI_allocate_unit_indicator_CTF_DETERMINED } OpenAPI_allocate_unit_indicator_e;

char* OpenAPI_allocate_unit_indicator_ToString(OpenAPI_allocate_unit_indicator_e allocate_unit_indicator);

OpenAPI_allocate_unit_indicator_e OpenAPI_allocate_unit_indicator_FromString(char* allocate_unit_indicator);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_allocate_unit_indicator_H_ */

