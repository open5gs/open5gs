/*
 * tid_type.h
 *
 * Indicates the type of T-ID.
 */

#ifndef _OpenAPI_tid_type_H_
#define _OpenAPI_tid_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_tid_type_NULL = 0, OpenAPI_tid_type_STORED, OpenAPI_tid_type_CONCEALED } OpenAPI_tid_type_e;

char* OpenAPI_tid_type_ToString(OpenAPI_tid_type_e tid_type);

OpenAPI_tid_type_e OpenAPI_tid_type_FromString(char* tid_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_tid_type_H_ */

