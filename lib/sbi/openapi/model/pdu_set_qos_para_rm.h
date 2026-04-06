/*
 * pdu_set_qos_para_rm.h
 *
 * PduSetQosPara contains removable attributes
 */

#ifndef _OpenAPI_pdu_set_qos_para_rm_H_
#define _OpenAPI_pdu_set_qos_para_rm_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_pdu_set_qos_para_rm_s OpenAPI_pdu_set_qos_para_rm_t;
#include "pdu_set_handling_info.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_pdu_set_qos_para_rm_s {
    bool is_pdu_set_delay_budget_null;
    bool is_pdu_set_delay_budget;
    int pdu_set_delay_budget;
    bool is_pdu_set_err_rate_null;
    char *pdu_set_err_rate;
    OpenAPI_pdu_set_handling_info_e pdu_set_handling_info;
};

OpenAPI_pdu_set_qos_para_rm_t *OpenAPI_pdu_set_qos_para_rm_create(
    bool is_pdu_set_delay_budget_null,
    bool is_pdu_set_delay_budget,
    int pdu_set_delay_budget,
    bool is_pdu_set_err_rate_null,
    char *pdu_set_err_rate,
    OpenAPI_pdu_set_handling_info_e pdu_set_handling_info
);
void OpenAPI_pdu_set_qos_para_rm_free(OpenAPI_pdu_set_qos_para_rm_t *pdu_set_qos_para_rm);
OpenAPI_pdu_set_qos_para_rm_t *OpenAPI_pdu_set_qos_para_rm_parseFromJSON(cJSON *pdu_set_qos_para_rmJSON);
cJSON *OpenAPI_pdu_set_qos_para_rm_convertToJSON(OpenAPI_pdu_set_qos_para_rm_t *pdu_set_qos_para_rm);
OpenAPI_pdu_set_qos_para_rm_t *OpenAPI_pdu_set_qos_para_rm_copy(OpenAPI_pdu_set_qos_para_rm_t *dst, OpenAPI_pdu_set_qos_para_rm_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pdu_set_qos_para_rm_H_ */

