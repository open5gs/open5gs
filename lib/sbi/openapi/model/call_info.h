/*
 * call_info.h
 *
 * Identifies the caller and callee information.
 */

#ifndef _OpenAPI_call_info_H_
#define _OpenAPI_call_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_call_info_s OpenAPI_call_info_t;
#include "callee_info.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_call_info_s {
    OpenAPI_list_t *calling_party_addrs;
    bool is_callee_info_null;
    struct OpenAPI_callee_info_s *callee_info;
};

OpenAPI_call_info_t *OpenAPI_call_info_create(
    OpenAPI_list_t *calling_party_addrs,
    bool is_callee_info_null,
    OpenAPI_callee_info_t *callee_info
);
void OpenAPI_call_info_free(OpenAPI_call_info_t *call_info);
OpenAPI_call_info_t *OpenAPI_call_info_parseFromJSON(cJSON *call_infoJSON);
cJSON *OpenAPI_call_info_convertToJSON(OpenAPI_call_info_t *call_info);
OpenAPI_call_info_t *OpenAPI_call_info_copy(OpenAPI_call_info_t *dst, OpenAPI_call_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_call_info_H_ */

