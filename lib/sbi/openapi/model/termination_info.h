/*
 * termination_info.h
 *
 * Indicates the cause for requesting the deletion of the Individual Application Session Context resource.
 */

#ifndef _OpenAPI_termination_info_H_
#define _OpenAPI_termination_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "termination_cause.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_termination_info_s OpenAPI_termination_info_t;
typedef struct OpenAPI_termination_info_s {
    OpenAPI_termination_cause_e term_cause;
    char *res_uri;
} OpenAPI_termination_info_t;

OpenAPI_termination_info_t *OpenAPI_termination_info_create(
    OpenAPI_termination_cause_e term_cause,
    char *res_uri
);
void OpenAPI_termination_info_free(OpenAPI_termination_info_t *termination_info);
OpenAPI_termination_info_t *OpenAPI_termination_info_parseFromJSON(cJSON *termination_infoJSON);
cJSON *OpenAPI_termination_info_convertToJSON(OpenAPI_termination_info_t *termination_info);
OpenAPI_termination_info_t *OpenAPI_termination_info_copy(OpenAPI_termination_info_t *dst, OpenAPI_termination_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_termination_info_H_ */

