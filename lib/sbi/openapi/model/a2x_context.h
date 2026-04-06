/*
 * a2x_context.h
 *
 * Represents the A2X services related parameters
 */

#ifndef _OpenAPI_a2x_context_H_
#define _OpenAPI_a2x_context_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_a2x_context_s OpenAPI_a2x_context_t;
#include "lte_a2x_auth.h"
#include "nr_a2x_auth.h"
#include "pc5_qo_s_para.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_a2x_context_s {
    struct OpenAPI_nr_a2x_auth_s *nr_a2x_services_auth;
    struct OpenAPI_lte_a2x_auth_s *lte_a2x_services_auth;
    char *nr_ue_sidelink_ambr;
    char *lte_ue_sidelink_ambr;
    struct OpenAPI_pc5_qo_s_para_s *pc5_qo_s_para;
};

OpenAPI_a2x_context_t *OpenAPI_a2x_context_create(
    OpenAPI_nr_a2x_auth_t *nr_a2x_services_auth,
    OpenAPI_lte_a2x_auth_t *lte_a2x_services_auth,
    char *nr_ue_sidelink_ambr,
    char *lte_ue_sidelink_ambr,
    OpenAPI_pc5_qo_s_para_t *pc5_qo_s_para
);
void OpenAPI_a2x_context_free(OpenAPI_a2x_context_t *a2x_context);
OpenAPI_a2x_context_t *OpenAPI_a2x_context_parseFromJSON(cJSON *a2x_contextJSON);
cJSON *OpenAPI_a2x_context_convertToJSON(OpenAPI_a2x_context_t *a2x_context);
OpenAPI_a2x_context_t *OpenAPI_a2x_context_copy(OpenAPI_a2x_context_t *dst, OpenAPI_a2x_context_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_a2x_context_H_ */

