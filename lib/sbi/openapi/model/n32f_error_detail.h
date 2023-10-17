/*
 * n32f_error_detail.h
 *
 * Details about the N32f error
 */

#ifndef _OpenAPI_n32f_error_detail_H_
#define _OpenAPI_n32f_error_detail_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "failure_reason.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_n32f_error_detail_s OpenAPI_n32f_error_detail_t;
typedef struct OpenAPI_n32f_error_detail_s {
    char *attribute;
    OpenAPI_failure_reason_e msg_reconstruct_fail_reason;
} OpenAPI_n32f_error_detail_t;

OpenAPI_n32f_error_detail_t *OpenAPI_n32f_error_detail_create(
    char *attribute,
    OpenAPI_failure_reason_e msg_reconstruct_fail_reason
);
void OpenAPI_n32f_error_detail_free(OpenAPI_n32f_error_detail_t *n32f_error_detail);
OpenAPI_n32f_error_detail_t *OpenAPI_n32f_error_detail_parseFromJSON(cJSON *n32f_error_detailJSON);
cJSON *OpenAPI_n32f_error_detail_convertToJSON(OpenAPI_n32f_error_detail_t *n32f_error_detail);
OpenAPI_n32f_error_detail_t *OpenAPI_n32f_error_detail_copy(OpenAPI_n32f_error_detail_t *dst, OpenAPI_n32f_error_detail_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_n32f_error_detail_H_ */

