/*
 * qmc_area_scope_1.h
 *
 * This IE contains the area in Cells or Tracking Areas where the QMC data collection shall take place. 
 */

#ifndef _OpenAPI_qmc_area_scope_1_H_
#define _OpenAPI_qmc_area_scope_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_qmc_area_scope_1_s OpenAPI_qmc_area_scope_1_t;
#include "plmn_id.h"
#include "tai_1.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_qmc_area_scope_1_s {
    OpenAPI_list_t *nr_cell_id_list;
    OpenAPI_list_t *tac_list;
    OpenAPI_list_t *tai_list;
    OpenAPI_list_t *plmn_list;
};

OpenAPI_qmc_area_scope_1_t *OpenAPI_qmc_area_scope_1_create(
    OpenAPI_list_t *nr_cell_id_list,
    OpenAPI_list_t *tac_list,
    OpenAPI_list_t *tai_list,
    OpenAPI_list_t *plmn_list
);
void OpenAPI_qmc_area_scope_1_free(OpenAPI_qmc_area_scope_1_t *qmc_area_scope_1);
OpenAPI_qmc_area_scope_1_t *OpenAPI_qmc_area_scope_1_parseFromJSON(cJSON *qmc_area_scope_1JSON);
cJSON *OpenAPI_qmc_area_scope_1_convertToJSON(OpenAPI_qmc_area_scope_1_t *qmc_area_scope_1);
OpenAPI_qmc_area_scope_1_t *OpenAPI_qmc_area_scope_1_copy(OpenAPI_qmc_area_scope_1_t *dst, OpenAPI_qmc_area_scope_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_qmc_area_scope_1_H_ */

