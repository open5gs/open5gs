/*
 * ce_mode_b_ind.h
 *
 * CE-mode-B Support Indicator.
 */

#ifndef _OpenAPI_ce_mode_b_ind_H_
#define _OpenAPI_ce_mode_b_ind_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ce_mode_b_ind_s OpenAPI_ce_mode_b_ind_t;
typedef struct OpenAPI_ce_mode_b_ind_s {
    int ce_mode_b_support_ind;
} OpenAPI_ce_mode_b_ind_t;

OpenAPI_ce_mode_b_ind_t *OpenAPI_ce_mode_b_ind_create(
    int ce_mode_b_support_ind
);
void OpenAPI_ce_mode_b_ind_free(OpenAPI_ce_mode_b_ind_t *ce_mode_b_ind);
OpenAPI_ce_mode_b_ind_t *OpenAPI_ce_mode_b_ind_parseFromJSON(cJSON *ce_mode_b_indJSON);
cJSON *OpenAPI_ce_mode_b_ind_convertToJSON(OpenAPI_ce_mode_b_ind_t *ce_mode_b_ind);
OpenAPI_ce_mode_b_ind_t *OpenAPI_ce_mode_b_ind_copy(OpenAPI_ce_mode_b_ind_t *dst, OpenAPI_ce_mode_b_ind_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ce_mode_b_ind_H_ */

