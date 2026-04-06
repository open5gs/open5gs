/*
 * pdu_ses_traffic_info.h
 *
 * 
 */

#ifndef _OpenAPI_pdu_ses_traffic_info_H_
#define _OpenAPI_pdu_ses_traffic_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_pdu_ses_traffic_info_s OpenAPI_pdu_ses_traffic_info_t;
#include "snssai.h"
#include "td_traffic.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_pdu_ses_traffic_info_s {
    OpenAPI_list_t *supis;
    char *dnn;
    struct OpenAPI_snssai_s *snssai;
    OpenAPI_list_t *td_match_trafs;
    OpenAPI_list_t *td_unmatch_trafs;
};

OpenAPI_pdu_ses_traffic_info_t *OpenAPI_pdu_ses_traffic_info_create(
    OpenAPI_list_t *supis,
    char *dnn,
    OpenAPI_snssai_t *snssai,
    OpenAPI_list_t *td_match_trafs,
    OpenAPI_list_t *td_unmatch_trafs
);
void OpenAPI_pdu_ses_traffic_info_free(OpenAPI_pdu_ses_traffic_info_t *pdu_ses_traffic_info);
OpenAPI_pdu_ses_traffic_info_t *OpenAPI_pdu_ses_traffic_info_parseFromJSON(cJSON *pdu_ses_traffic_infoJSON);
cJSON *OpenAPI_pdu_ses_traffic_info_convertToJSON(OpenAPI_pdu_ses_traffic_info_t *pdu_ses_traffic_info);
OpenAPI_pdu_ses_traffic_info_t *OpenAPI_pdu_ses_traffic_info_copy(OpenAPI_pdu_ses_traffic_info_t *dst, OpenAPI_pdu_ses_traffic_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pdu_ses_traffic_info_H_ */

