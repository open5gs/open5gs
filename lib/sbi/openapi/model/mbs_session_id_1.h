/*
 * mbs_session_id_1.h
 *
 * MBS Session Identifier
 */

#ifndef _OpenAPI_mbs_session_id_1_H_
#define _OpenAPI_mbs_session_id_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ssm_1.h"
#include "tmgi_1.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_mbs_session_id_1_s OpenAPI_mbs_session_id_1_t;
typedef struct OpenAPI_mbs_session_id_1_s {
    struct OpenAPI_tmgi_1_s *tmgi;
    struct OpenAPI_ssm_1_s *ssm;
    char *nid;
} OpenAPI_mbs_session_id_1_t;

OpenAPI_mbs_session_id_1_t *OpenAPI_mbs_session_id_1_create(
    OpenAPI_tmgi_1_t *tmgi,
    OpenAPI_ssm_1_t *ssm,
    char *nid
);
void OpenAPI_mbs_session_id_1_free(OpenAPI_mbs_session_id_1_t *mbs_session_id_1);
OpenAPI_mbs_session_id_1_t *OpenAPI_mbs_session_id_1_parseFromJSON(cJSON *mbs_session_id_1JSON);
cJSON *OpenAPI_mbs_session_id_1_convertToJSON(OpenAPI_mbs_session_id_1_t *mbs_session_id_1);
OpenAPI_mbs_session_id_1_t *OpenAPI_mbs_session_id_1_copy(OpenAPI_mbs_session_id_1_t *dst, OpenAPI_mbs_session_id_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_mbs_session_id_1_H_ */

