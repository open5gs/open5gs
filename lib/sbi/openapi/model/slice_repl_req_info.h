/*
 * slice_repl_req_info.h
 *
 * Represents the AF requested Network Slice replacement information.
 */

#ifndef _OpenAPI_slice_repl_req_info_H_
#define _OpenAPI_slice_repl_req_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_slice_repl_req_info_s OpenAPI_slice_repl_req_info_t;
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_slice_repl_req_info_s {
    struct OpenAPI_snssai_s *snssai;
    struct OpenAPI_snssai_s *alt_snssai;
};

OpenAPI_slice_repl_req_info_t *OpenAPI_slice_repl_req_info_create(
    OpenAPI_snssai_t *snssai,
    OpenAPI_snssai_t *alt_snssai
);
void OpenAPI_slice_repl_req_info_free(OpenAPI_slice_repl_req_info_t *slice_repl_req_info);
OpenAPI_slice_repl_req_info_t *OpenAPI_slice_repl_req_info_parseFromJSON(cJSON *slice_repl_req_infoJSON);
cJSON *OpenAPI_slice_repl_req_info_convertToJSON(OpenAPI_slice_repl_req_info_t *slice_repl_req_info);
OpenAPI_slice_repl_req_info_t *OpenAPI_slice_repl_req_info_copy(OpenAPI_slice_repl_req_info_t *dst, OpenAPI_slice_repl_req_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_slice_repl_req_info_H_ */

