/*
 * pdu_ses_traffic_req.h
 *
 * Represents the PDU Session traffic analytics requirements.
 */

#ifndef _OpenAPI_pdu_ses_traffic_req_H_
#define _OpenAPI_pdu_ses_traffic_req_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_pdu_ses_traffic_req_s OpenAPI_pdu_ses_traffic_req_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_pdu_ses_traffic_req_s {
    OpenAPI_list_t *flow_descs;
    char *app_id;
    OpenAPI_list_t *domain_descs;
};

OpenAPI_pdu_ses_traffic_req_t *OpenAPI_pdu_ses_traffic_req_create(
    OpenAPI_list_t *flow_descs,
    char *app_id,
    OpenAPI_list_t *domain_descs
);
void OpenAPI_pdu_ses_traffic_req_free(OpenAPI_pdu_ses_traffic_req_t *pdu_ses_traffic_req);
OpenAPI_pdu_ses_traffic_req_t *OpenAPI_pdu_ses_traffic_req_parseFromJSON(cJSON *pdu_ses_traffic_reqJSON);
cJSON *OpenAPI_pdu_ses_traffic_req_convertToJSON(OpenAPI_pdu_ses_traffic_req_t *pdu_ses_traffic_req);
OpenAPI_pdu_ses_traffic_req_t *OpenAPI_pdu_ses_traffic_req_copy(OpenAPI_pdu_ses_traffic_req_t *dst, OpenAPI_pdu_ses_traffic_req_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pdu_ses_traffic_req_H_ */

