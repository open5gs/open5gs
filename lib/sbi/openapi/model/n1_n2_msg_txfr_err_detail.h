/*
 * n1_n2_msg_txfr_err_detail.h
 *
 * 
 */

#ifndef _OpenAPI_n1_n2_msg_txfr_err_detail_H_
#define _OpenAPI_n1_n2_msg_txfr_err_detail_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "arp.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_n1_n2_msg_txfr_err_detail_s OpenAPI_n1_n2_msg_txfr_err_detail_t;
typedef struct OpenAPI_n1_n2_msg_txfr_err_detail_s {
    bool is_retry_after;
    int retry_after;
    struct OpenAPI_arp_s *highest_prio_arp;
    bool is_max_waiting_time;
    int max_waiting_time;
} OpenAPI_n1_n2_msg_txfr_err_detail_t;

OpenAPI_n1_n2_msg_txfr_err_detail_t *OpenAPI_n1_n2_msg_txfr_err_detail_create(
    bool is_retry_after,
    int retry_after,
    OpenAPI_arp_t *highest_prio_arp,
    bool is_max_waiting_time,
    int max_waiting_time
);
void OpenAPI_n1_n2_msg_txfr_err_detail_free(OpenAPI_n1_n2_msg_txfr_err_detail_t *n1_n2_msg_txfr_err_detail);
OpenAPI_n1_n2_msg_txfr_err_detail_t *OpenAPI_n1_n2_msg_txfr_err_detail_parseFromJSON(cJSON *n1_n2_msg_txfr_err_detailJSON);
cJSON *OpenAPI_n1_n2_msg_txfr_err_detail_convertToJSON(OpenAPI_n1_n2_msg_txfr_err_detail_t *n1_n2_msg_txfr_err_detail);
OpenAPI_n1_n2_msg_txfr_err_detail_t *OpenAPI_n1_n2_msg_txfr_err_detail_copy(OpenAPI_n1_n2_msg_txfr_err_detail_t *dst, OpenAPI_n1_n2_msg_txfr_err_detail_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_n1_n2_msg_txfr_err_detail_H_ */

