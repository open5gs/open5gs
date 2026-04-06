/*
 * deregistration_resp_data.h
 *
 * Represents the required information to be sent to UDM in case of successful UECM deregistration. 
 */

#ifndef _OpenAPI_deregistration_resp_data_H_
#define _OpenAPI_deregistration_resp_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_deregistration_resp_data_s OpenAPI_deregistration_resp_data_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_deregistration_resp_data_s {
    bool is_smf_event_removed_ind;
    int smf_event_removed_ind;
};

OpenAPI_deregistration_resp_data_t *OpenAPI_deregistration_resp_data_create(
    bool is_smf_event_removed_ind,
    int smf_event_removed_ind
);
void OpenAPI_deregistration_resp_data_free(OpenAPI_deregistration_resp_data_t *deregistration_resp_data);
OpenAPI_deregistration_resp_data_t *OpenAPI_deregistration_resp_data_parseFromJSON(cJSON *deregistration_resp_dataJSON);
cJSON *OpenAPI_deregistration_resp_data_convertToJSON(OpenAPI_deregistration_resp_data_t *deregistration_resp_data);
OpenAPI_deregistration_resp_data_t *OpenAPI_deregistration_resp_data_copy(OpenAPI_deregistration_resp_data_t *dst, OpenAPI_deregistration_resp_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_deregistration_resp_data_H_ */

