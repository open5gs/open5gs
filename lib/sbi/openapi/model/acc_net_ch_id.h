/*
 * acc_net_ch_id.h
 *
 * 
 */

#ifndef _OpenAPI_acc_net_ch_id_H_
#define _OpenAPI_acc_net_ch_id_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_acc_net_ch_id_s OpenAPI_acc_net_ch_id_t;
typedef struct OpenAPI_acc_net_ch_id_s {
    int acc_net_cha_id_value;
    OpenAPI_list_t *ref_pcc_rule_ids;
    bool is_session_ch_scope;
    int session_ch_scope;
} OpenAPI_acc_net_ch_id_t;

OpenAPI_acc_net_ch_id_t *OpenAPI_acc_net_ch_id_create(
    int acc_net_cha_id_value,
    OpenAPI_list_t *ref_pcc_rule_ids,
    bool is_session_ch_scope,
    int session_ch_scope
);
void OpenAPI_acc_net_ch_id_free(OpenAPI_acc_net_ch_id_t *acc_net_ch_id);
OpenAPI_acc_net_ch_id_t *OpenAPI_acc_net_ch_id_parseFromJSON(cJSON *acc_net_ch_idJSON);
cJSON *OpenAPI_acc_net_ch_id_convertToJSON(OpenAPI_acc_net_ch_id_t *acc_net_ch_id);
OpenAPI_acc_net_ch_id_t *OpenAPI_acc_net_ch_id_copy(OpenAPI_acc_net_ch_id_t *dst, OpenAPI_acc_net_ch_id_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_acc_net_ch_id_H_ */

