/*
 * cag_ack_data.h
 *
 * 
 */

#ifndef _OpenAPI_cag_ack_data_H_
#define _OpenAPI_cag_ack_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ue_update_status.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_cag_ack_data_s OpenAPI_cag_ack_data_t;
typedef struct OpenAPI_cag_ack_data_s {
    char *provisioning_time;
    OpenAPI_ue_update_status_e ue_update_status;
} OpenAPI_cag_ack_data_t;

OpenAPI_cag_ack_data_t *OpenAPI_cag_ack_data_create(
    char *provisioning_time,
    OpenAPI_ue_update_status_e ue_update_status
);
void OpenAPI_cag_ack_data_free(OpenAPI_cag_ack_data_t *cag_ack_data);
OpenAPI_cag_ack_data_t *OpenAPI_cag_ack_data_parseFromJSON(cJSON *cag_ack_dataJSON);
cJSON *OpenAPI_cag_ack_data_convertToJSON(OpenAPI_cag_ack_data_t *cag_ack_data);
OpenAPI_cag_ack_data_t *OpenAPI_cag_ack_data_copy(OpenAPI_cag_ack_data_t *dst, OpenAPI_cag_ack_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_cag_ack_data_H_ */

