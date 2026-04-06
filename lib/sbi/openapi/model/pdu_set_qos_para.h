/*
 * pdu_set_qos_para.h
 *
 * Represents the PDU Set level QoS parameters.
 */

#ifndef _OpenAPI_pdu_set_qos_para_H_
#define _OpenAPI_pdu_set_qos_para_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_pdu_set_qos_para_s OpenAPI_pdu_set_qos_para_t;
#include "pdu_set_handling_info.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_pdu_set_qos_para_s {
    bool is_pdu_set_delay_budget;
    int pdu_set_delay_budget;
    char *pdu_set_err_rate;
    OpenAPI_pdu_set_handling_info_e pdu_set_handling_info;
};

OpenAPI_pdu_set_qos_para_t *OpenAPI_pdu_set_qos_para_create(
    bool is_pdu_set_delay_budget,
    int pdu_set_delay_budget,
    char *pdu_set_err_rate,
    OpenAPI_pdu_set_handling_info_e pdu_set_handling_info
);
void OpenAPI_pdu_set_qos_para_free(OpenAPI_pdu_set_qos_para_t *pdu_set_qos_para);
OpenAPI_pdu_set_qos_para_t *OpenAPI_pdu_set_qos_para_parseFromJSON(cJSON *pdu_set_qos_paraJSON);
cJSON *OpenAPI_pdu_set_qos_para_convertToJSON(OpenAPI_pdu_set_qos_para_t *pdu_set_qos_para);
OpenAPI_pdu_set_qos_para_t *OpenAPI_pdu_set_qos_para_copy(OpenAPI_pdu_set_qos_para_t *dst, OpenAPI_pdu_set_qos_para_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pdu_set_qos_para_H_ */

