/*
 * release_session_info.h
 *
 * PDU session Id(s) and the cause for triggering the release.
 */

#ifndef _OpenAPI_release_session_info_H_
#define _OpenAPI_release_session_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "release_cause.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_release_session_info_s OpenAPI_release_session_info_t;
typedef struct OpenAPI_release_session_info_s {
    OpenAPI_list_t *release_session_list;
    OpenAPI_release_cause_e release_cause;
} OpenAPI_release_session_info_t;

OpenAPI_release_session_info_t *OpenAPI_release_session_info_create(
    OpenAPI_list_t *release_session_list,
    OpenAPI_release_cause_e release_cause
);
void OpenAPI_release_session_info_free(OpenAPI_release_session_info_t *release_session_info);
OpenAPI_release_session_info_t *OpenAPI_release_session_info_parseFromJSON(cJSON *release_session_infoJSON);
cJSON *OpenAPI_release_session_info_convertToJSON(OpenAPI_release_session_info_t *release_session_info);
OpenAPI_release_session_info_t *OpenAPI_release_session_info_copy(OpenAPI_release_session_info_t *dst, OpenAPI_release_session_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_release_session_info_H_ */

