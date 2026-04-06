/*
 * dereg_inact_timer_info.h
 *
 * Network Slice Deregistration Inactivity Timer Information
 */

#ifndef _OpenAPI_dereg_inact_timer_info_H_
#define _OpenAPI_dereg_inact_timer_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_dereg_inact_timer_info_s OpenAPI_dereg_inact_timer_info_t;
#include "nssaa_status.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_dereg_inact_timer_info_s {
    struct OpenAPI_nssaa_status_s *dereg_inact_expiry_time;
};

OpenAPI_dereg_inact_timer_info_t *OpenAPI_dereg_inact_timer_info_create(
    OpenAPI_nssaa_status_t *dereg_inact_expiry_time
);
void OpenAPI_dereg_inact_timer_info_free(OpenAPI_dereg_inact_timer_info_t *dereg_inact_timer_info);
OpenAPI_dereg_inact_timer_info_t *OpenAPI_dereg_inact_timer_info_parseFromJSON(cJSON *dereg_inact_timer_infoJSON);
cJSON *OpenAPI_dereg_inact_timer_info_convertToJSON(OpenAPI_dereg_inact_timer_info_t *dereg_inact_timer_info);
OpenAPI_dereg_inact_timer_info_t *OpenAPI_dereg_inact_timer_info_copy(OpenAPI_dereg_inact_timer_info_t *dst, OpenAPI_dereg_inact_timer_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_dereg_inact_timer_info_H_ */

