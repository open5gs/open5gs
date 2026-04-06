/*
 * ecn_marking_congestion_info_req.h
 *
 * ECN Marking or Congestion Information Request
 */

#ifndef _OpenAPI_ecn_marking_congestion_info_req_H_
#define _OpenAPI_ecn_marking_congestion_info_req_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_ecn_marking_congestion_info_req_s OpenAPI_ecn_marking_congestion_info_req_t;
#include "congestion_info_req.h"
#include "ecn_marking_req.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_ecn_marking_congestion_info_req_s {
    OpenAPI_ecn_marking_req_e ecn_marking_ran_req;
    OpenAPI_ecn_marking_req_e ecn_marking_upf_req;
    OpenAPI_congestion_info_req_e congestion_info_req;
};

OpenAPI_ecn_marking_congestion_info_req_t *OpenAPI_ecn_marking_congestion_info_req_create(
    OpenAPI_ecn_marking_req_e ecn_marking_ran_req,
    OpenAPI_ecn_marking_req_e ecn_marking_upf_req,
    OpenAPI_congestion_info_req_e congestion_info_req
);
void OpenAPI_ecn_marking_congestion_info_req_free(OpenAPI_ecn_marking_congestion_info_req_t *ecn_marking_congestion_info_req);
OpenAPI_ecn_marking_congestion_info_req_t *OpenAPI_ecn_marking_congestion_info_req_parseFromJSON(cJSON *ecn_marking_congestion_info_reqJSON);
cJSON *OpenAPI_ecn_marking_congestion_info_req_convertToJSON(OpenAPI_ecn_marking_congestion_info_req_t *ecn_marking_congestion_info_req);
OpenAPI_ecn_marking_congestion_info_req_t *OpenAPI_ecn_marking_congestion_info_req_copy(OpenAPI_ecn_marking_congestion_info_req_t *dst, OpenAPI_ecn_marking_congestion_info_req_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ecn_marking_congestion_info_req_H_ */

