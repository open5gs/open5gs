/*
 * ue_reg_status_update_rsp_data.h
 *
 * 
 */

#ifndef _OpenAPI_ue_reg_status_update_rsp_data_H_
#define _OpenAPI_ue_reg_status_update_rsp_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ue_reg_status_update_rsp_data_s OpenAPI_ue_reg_status_update_rsp_data_t;
typedef struct OpenAPI_ue_reg_status_update_rsp_data_s {
    int reg_status_transfer_complete;
} OpenAPI_ue_reg_status_update_rsp_data_t;

OpenAPI_ue_reg_status_update_rsp_data_t *OpenAPI_ue_reg_status_update_rsp_data_create(
    int reg_status_transfer_complete
);
void OpenAPI_ue_reg_status_update_rsp_data_free(OpenAPI_ue_reg_status_update_rsp_data_t *ue_reg_status_update_rsp_data);
OpenAPI_ue_reg_status_update_rsp_data_t *OpenAPI_ue_reg_status_update_rsp_data_parseFromJSON(cJSON *ue_reg_status_update_rsp_dataJSON);
cJSON *OpenAPI_ue_reg_status_update_rsp_data_convertToJSON(OpenAPI_ue_reg_status_update_rsp_data_t *ue_reg_status_update_rsp_data);
OpenAPI_ue_reg_status_update_rsp_data_t *OpenAPI_ue_reg_status_update_rsp_data_copy(OpenAPI_ue_reg_status_update_rsp_data_t *dst, OpenAPI_ue_reg_status_update_rsp_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_reg_status_update_rsp_data_H_ */

