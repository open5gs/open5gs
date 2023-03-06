/*
 * prose_context.h
 *
 * Represents the ProSe services related parameters.
 */

#ifndef _OpenAPI_prose_context_H_
#define _OpenAPI_prose_context_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "pc5_qo_s_para.h"
#include "ue_auth.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_prose_context_s OpenAPI_prose_context_t;
typedef struct OpenAPI_prose_context_s {
    OpenAPI_ue_auth_e direct_discovery;
    OpenAPI_ue_auth_e direct_comm;
    OpenAPI_ue_auth_e l2_relay;
    OpenAPI_ue_auth_e l3_relay;
    OpenAPI_ue_auth_e l2_remote;
    char *nr_ue_pc5_ambr;
    struct OpenAPI_pc5_qo_s_para_s *pc5_qo_s_para;
} OpenAPI_prose_context_t;

OpenAPI_prose_context_t *OpenAPI_prose_context_create(
    OpenAPI_ue_auth_e direct_discovery,
    OpenAPI_ue_auth_e direct_comm,
    OpenAPI_ue_auth_e l2_relay,
    OpenAPI_ue_auth_e l3_relay,
    OpenAPI_ue_auth_e l2_remote,
    char *nr_ue_pc5_ambr,
    OpenAPI_pc5_qo_s_para_t *pc5_qo_s_para
);
void OpenAPI_prose_context_free(OpenAPI_prose_context_t *prose_context);
OpenAPI_prose_context_t *OpenAPI_prose_context_parseFromJSON(cJSON *prose_contextJSON);
cJSON *OpenAPI_prose_context_convertToJSON(OpenAPI_prose_context_t *prose_context);
OpenAPI_prose_context_t *OpenAPI_prose_context_copy(OpenAPI_prose_context_t *dst, OpenAPI_prose_context_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_prose_context_H_ */

