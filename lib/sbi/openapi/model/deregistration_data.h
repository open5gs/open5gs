/*
 * deregistration_data.h
 *
 * This datatype is supported by the POST HTTP method, and it includes the deregistration data. 
 */

#ifndef _OpenAPI_deregistration_data_H_
#define _OpenAPI_deregistration_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_deregistration_data_s OpenAPI_deregistration_data_t;
#include "access_type.h"
#include "deregistration_reason.h"
#include "guami.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_deregistration_data_s {
    OpenAPI_deregistration_reason_e dereg_reason;
    OpenAPI_access_type_e access_type;
    bool is_pdu_session_id;
    int pdu_session_id;
    char *new_smf_instance_id;
    struct OpenAPI_guami_s *old_guami;
};

OpenAPI_deregistration_data_t *OpenAPI_deregistration_data_create(
    OpenAPI_deregistration_reason_e dereg_reason,
    OpenAPI_access_type_e access_type,
    bool is_pdu_session_id,
    int pdu_session_id,
    char *new_smf_instance_id,
    OpenAPI_guami_t *old_guami
);
void OpenAPI_deregistration_data_free(OpenAPI_deregistration_data_t *deregistration_data);
OpenAPI_deregistration_data_t *OpenAPI_deregistration_data_parseFromJSON(cJSON *deregistration_dataJSON);
cJSON *OpenAPI_deregistration_data_convertToJSON(OpenAPI_deregistration_data_t *deregistration_data);
OpenAPI_deregistration_data_t *OpenAPI_deregistration_data_copy(OpenAPI_deregistration_data_t *dst, OpenAPI_deregistration_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_deregistration_data_H_ */

