/*
 * slice_repl_type.h
 *
 * 
 */

#ifndef _OpenAPI_slice_repl_type_H_
#define _OpenAPI_slice_repl_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_slice_repl_type_NULL = 0, OpenAPI_slice_repl_type_INITIATE_AF_REQ_SLICE_REP, OpenAPI_slice_repl_type_TERMINATE_AF_REQ_SLICE_REP } OpenAPI_slice_repl_type_e;

char* OpenAPI_slice_repl_type_ToString(OpenAPI_slice_repl_type_e slice_repl_type);

OpenAPI_slice_repl_type_e OpenAPI_slice_repl_type_FromString(char* slice_repl_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_slice_repl_type_H_ */

