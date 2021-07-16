/*
 * ue_reg_status_update_req_data.h
 *
 * 
 */

#ifndef _OpenAPI_ue_reg_status_update_req_data_H_
#define _OpenAPI_ue_reg_status_update_req_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "smf_change_info.h"
#include "ue_context_transfer_status.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ue_reg_status_update_req_data_s OpenAPI_ue_reg_status_update_req_data_t;
typedef struct OpenAPI_ue_reg_status_update_req_data_s {
    OpenAPI_ue_context_transfer_status_e transfer_status;
    OpenAPI_list_t *to_release_session_list;
    bool is_pcf_reselected_ind;
    int pcf_reselected_ind;
    OpenAPI_list_t *smf_change_info_list;
} OpenAPI_ue_reg_status_update_req_data_t;

OpenAPI_ue_reg_status_update_req_data_t *OpenAPI_ue_reg_status_update_req_data_create(
    OpenAPI_ue_context_transfer_status_e transfer_status,
    OpenAPI_list_t *to_release_session_list,
    bool is_pcf_reselected_ind,
    int pcf_reselected_ind,
    OpenAPI_list_t *smf_change_info_list
);
void OpenAPI_ue_reg_status_update_req_data_free(OpenAPI_ue_reg_status_update_req_data_t *ue_reg_status_update_req_data);
OpenAPI_ue_reg_status_update_req_data_t *OpenAPI_ue_reg_status_update_req_data_parseFromJSON(cJSON *ue_reg_status_update_req_dataJSON);
cJSON *OpenAPI_ue_reg_status_update_req_data_convertToJSON(OpenAPI_ue_reg_status_update_req_data_t *ue_reg_status_update_req_data);
OpenAPI_ue_reg_status_update_req_data_t *OpenAPI_ue_reg_status_update_req_data_copy(OpenAPI_ue_reg_status_update_req_data_t *dst, OpenAPI_ue_reg_status_update_req_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_reg_status_update_req_data_H_ */

