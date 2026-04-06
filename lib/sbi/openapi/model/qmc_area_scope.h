/*
 * qmc_area_scope.h
 *
 * This IE contains the area in Cells or Tracking Areas where the QMC data collection shall take place. 
 */

#ifndef _OpenAPI_qmc_area_scope_H_
#define _OpenAPI_qmc_area_scope_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_qmc_area_scope_s OpenAPI_qmc_area_scope_t;
#include "plmn_id.h"
#include "tai.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_qmc_area_scope_s {
    OpenAPI_list_t *nr_cell_id_list;
    OpenAPI_list_t *tac_list;
    OpenAPI_list_t *tai_list;
    OpenAPI_list_t *plmn_list;
};

OpenAPI_qmc_area_scope_t *OpenAPI_qmc_area_scope_create(
    OpenAPI_list_t *nr_cell_id_list,
    OpenAPI_list_t *tac_list,
    OpenAPI_list_t *tai_list,
    OpenAPI_list_t *plmn_list
);
void OpenAPI_qmc_area_scope_free(OpenAPI_qmc_area_scope_t *qmc_area_scope);
OpenAPI_qmc_area_scope_t *OpenAPI_qmc_area_scope_parseFromJSON(cJSON *qmc_area_scopeJSON);
cJSON *OpenAPI_qmc_area_scope_convertToJSON(OpenAPI_qmc_area_scope_t *qmc_area_scope);
OpenAPI_qmc_area_scope_t *OpenAPI_qmc_area_scope_copy(OpenAPI_qmc_area_scope_t *dst, OpenAPI_qmc_area_scope_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_qmc_area_scope_H_ */

