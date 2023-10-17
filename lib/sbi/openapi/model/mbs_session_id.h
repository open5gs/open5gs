/*
 * mbs_session_id.h
 *
 * MBS Session Identifier
 */

#ifndef _OpenAPI_mbs_session_id_H_
#define _OpenAPI_mbs_session_id_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ssm.h"
#include "tmgi.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_mbs_session_id_s OpenAPI_mbs_session_id_t;
typedef struct OpenAPI_mbs_session_id_s {
    struct OpenAPI_tmgi_s *tmgi;
    struct OpenAPI_ssm_s *ssm;
    char *nid;
} OpenAPI_mbs_session_id_t;

OpenAPI_mbs_session_id_t *OpenAPI_mbs_session_id_create(
    OpenAPI_tmgi_t *tmgi,
    OpenAPI_ssm_t *ssm,
    char *nid
);
void OpenAPI_mbs_session_id_free(OpenAPI_mbs_session_id_t *mbs_session_id);
OpenAPI_mbs_session_id_t *OpenAPI_mbs_session_id_parseFromJSON(cJSON *mbs_session_idJSON);
cJSON *OpenAPI_mbs_session_id_convertToJSON(OpenAPI_mbs_session_id_t *mbs_session_id);
OpenAPI_mbs_session_id_t *OpenAPI_mbs_session_id_copy(OpenAPI_mbs_session_id_t *dst, OpenAPI_mbs_session_id_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_mbs_session_id_H_ */

