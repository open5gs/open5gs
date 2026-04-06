/*
 * amf_dereg_info.h
 *
 * This data type contains the reason of deregistration.
 */

#ifndef _OpenAPI_amf_dereg_info_H_
#define _OpenAPI_amf_dereg_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_amf_dereg_info_s OpenAPI_amf_dereg_info_t;
#include "deregistration_reason.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_amf_dereg_info_s {
    OpenAPI_deregistration_reason_e dereg_reason;
};

OpenAPI_amf_dereg_info_t *OpenAPI_amf_dereg_info_create(
    OpenAPI_deregistration_reason_e dereg_reason
);
void OpenAPI_amf_dereg_info_free(OpenAPI_amf_dereg_info_t *amf_dereg_info);
OpenAPI_amf_dereg_info_t *OpenAPI_amf_dereg_info_parseFromJSON(cJSON *amf_dereg_infoJSON);
cJSON *OpenAPI_amf_dereg_info_convertToJSON(OpenAPI_amf_dereg_info_t *amf_dereg_info);
OpenAPI_amf_dereg_info_t *OpenAPI_amf_dereg_info_copy(OpenAPI_amf_dereg_info_t *dst, OpenAPI_amf_dereg_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_amf_dereg_info_H_ */

