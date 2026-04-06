/*
 * area_scope_1.h
 *
 * Contain the area based on Cells or Tracking Areas.
 */

#ifndef _OpenAPI_area_scope_1_H_
#define _OpenAPI_area_scope_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_area_scope_1_s OpenAPI_area_scope_1_t;
#include "cag_info_2.h"
#include "cell_id_nid_info.h"
#include "nid_info.h"
#include "tac_info.h"
#include "tac_nid_info.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_area_scope_1_s {
    OpenAPI_list_t *eutra_cell_id_list;
    OpenAPI_list_t *nr_cell_id_list;
    OpenAPI_list_t *tac_list;
    OpenAPI_list_t* tac_info_per_plmn;
    OpenAPI_list_t* cag_info_per_plmn;
    OpenAPI_list_t* nid_info_per_plmn;
    OpenAPI_list_t* cell_id_nid_info_per_plmn;
    OpenAPI_list_t* tac_nid_info_per_plmn;
    OpenAPI_list_t *cag_list;
};

OpenAPI_area_scope_1_t *OpenAPI_area_scope_1_create(
    OpenAPI_list_t *eutra_cell_id_list,
    OpenAPI_list_t *nr_cell_id_list,
    OpenAPI_list_t *tac_list,
    OpenAPI_list_t* tac_info_per_plmn,
    OpenAPI_list_t* cag_info_per_plmn,
    OpenAPI_list_t* nid_info_per_plmn,
    OpenAPI_list_t* cell_id_nid_info_per_plmn,
    OpenAPI_list_t* tac_nid_info_per_plmn,
    OpenAPI_list_t *cag_list
);
void OpenAPI_area_scope_1_free(OpenAPI_area_scope_1_t *area_scope_1);
OpenAPI_area_scope_1_t *OpenAPI_area_scope_1_parseFromJSON(cJSON *area_scope_1JSON);
cJSON *OpenAPI_area_scope_1_convertToJSON(OpenAPI_area_scope_1_t *area_scope_1);
OpenAPI_area_scope_1_t *OpenAPI_area_scope_1_copy(OpenAPI_area_scope_1_t *dst, OpenAPI_area_scope_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_area_scope_1_H_ */

