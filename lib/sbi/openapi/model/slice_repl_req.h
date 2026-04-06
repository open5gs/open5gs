/*
 * slice_repl_req.h
 *
 * Represents the requested Network Slice Replacement requirements. 
 */

#ifndef _OpenAPI_slice_repl_req_H_
#define _OpenAPI_slice_repl_req_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_slice_repl_req_s OpenAPI_slice_repl_req_t;
#include "slice_repl_req_info.h"
#include "slice_repl_type.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_slice_repl_req_s {
    OpenAPI_slice_repl_type_e type;
    struct OpenAPI_slice_repl_req_info_s *af_slice_repl_req;
};

OpenAPI_slice_repl_req_t *OpenAPI_slice_repl_req_create(
    OpenAPI_slice_repl_type_e type,
    OpenAPI_slice_repl_req_info_t *af_slice_repl_req
);
void OpenAPI_slice_repl_req_free(OpenAPI_slice_repl_req_t *slice_repl_req);
OpenAPI_slice_repl_req_t *OpenAPI_slice_repl_req_parseFromJSON(cJSON *slice_repl_reqJSON);
cJSON *OpenAPI_slice_repl_req_convertToJSON(OpenAPI_slice_repl_req_t *slice_repl_req);
OpenAPI_slice_repl_req_t *OpenAPI_slice_repl_req_copy(OpenAPI_slice_repl_req_t *dst, OpenAPI_slice_repl_req_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_slice_repl_req_H_ */

