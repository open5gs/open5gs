/*
 * af_req_default_qo_s.h
 *
 * Contains AF-requested default QoS
 */

#ifndef _OpenAPI_af_req_default_qo_s_H_
#define _OpenAPI_af_req_default_qo_s_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_af_req_default_qo_s_s OpenAPI_af_req_default_qo_s_t;
#include "arp.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_af_req_default_qo_s_s {
    int _5qi;
    struct OpenAPI_arp_s *arp;
    bool is_priority_level;
    int priority_level;
};

OpenAPI_af_req_default_qo_s_t *OpenAPI_af_req_default_qo_s_create(
    int _5qi,
    OpenAPI_arp_t *arp,
    bool is_priority_level,
    int priority_level
);
void OpenAPI_af_req_default_qo_s_free(OpenAPI_af_req_default_qo_s_t *af_req_default_qo_s);
OpenAPI_af_req_default_qo_s_t *OpenAPI_af_req_default_qo_s_parseFromJSON(cJSON *af_req_default_qo_sJSON);
cJSON *OpenAPI_af_req_default_qo_s_convertToJSON(OpenAPI_af_req_default_qo_s_t *af_req_default_qo_s);
OpenAPI_af_req_default_qo_s_t *OpenAPI_af_req_default_qo_s_copy(OpenAPI_af_req_default_qo_s_t *dst, OpenAPI_af_req_default_qo_s_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_af_req_default_qo_s_H_ */

