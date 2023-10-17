/*
 * mbs_session.h
 *
 * MBS Session currently served by an MB-SMF
 */

#ifndef _OpenAPI_mbs_session_H_
#define _OpenAPI_mbs_session_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "mbs_service_area_info.h"
#include "mbs_session_id.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_mbs_session_s OpenAPI_mbs_session_t;
typedef struct OpenAPI_mbs_session_s {
    struct OpenAPI_mbs_session_id_s *mbs_session_id;
    OpenAPI_list_t* mbs_area_sessions;
} OpenAPI_mbs_session_t;

OpenAPI_mbs_session_t *OpenAPI_mbs_session_create(
    OpenAPI_mbs_session_id_t *mbs_session_id,
    OpenAPI_list_t* mbs_area_sessions
);
void OpenAPI_mbs_session_free(OpenAPI_mbs_session_t *mbs_session);
OpenAPI_mbs_session_t *OpenAPI_mbs_session_parseFromJSON(cJSON *mbs_sessionJSON);
cJSON *OpenAPI_mbs_session_convertToJSON(OpenAPI_mbs_session_t *mbs_session);
OpenAPI_mbs_session_t *OpenAPI_mbs_session_copy(OpenAPI_mbs_session_t *dst, OpenAPI_mbs_session_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_mbs_session_H_ */

