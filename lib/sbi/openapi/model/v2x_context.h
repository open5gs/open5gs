/*
 * v2x_context.h
 *
 *
 */

#ifndef _OpenAPI_v2x_context_H_
#define _OpenAPI_v2x_context_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "lte_v2x_auth.h"
#include "nr_v2x_auth.h"
#include "pc5_qo_s_para.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_v2x_context_s OpenAPI_v2x_context_t;
typedef struct OpenAPI_v2x_context_s {
    struct OpenAPI_nr_v2x_auth_s *nr_v2x_services_auth;
    struct OpenAPI_lte_v2x_auth_s *lte_v2x_services_auth;
    char *nr_ue_sidelink_ambr;
    char *lte_ue_sidelink_ambr;
    struct OpenAPI_pc5_qo_s_para_s *pc5_qo_s_para;
} OpenAPI_v2x_context_t;

OpenAPI_v2x_context_t *OpenAPI_v2x_context_create(
    OpenAPI_nr_v2x_auth_t *nr_v2x_services_auth,
    OpenAPI_lte_v2x_auth_t *lte_v2x_services_auth,
    char *nr_ue_sidelink_ambr,
    char *lte_ue_sidelink_ambr,
    OpenAPI_pc5_qo_s_para_t *pc5_qo_s_para
    );
void OpenAPI_v2x_context_free(OpenAPI_v2x_context_t *v2x_context);
OpenAPI_v2x_context_t *OpenAPI_v2x_context_parseFromJSON(cJSON *v2x_contextJSON);
cJSON *OpenAPI_v2x_context_convertToJSON(OpenAPI_v2x_context_t *v2x_context);
OpenAPI_v2x_context_t *OpenAPI_v2x_context_copy(OpenAPI_v2x_context_t *dst, OpenAPI_v2x_context_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_v2x_context_H_ */

