/*
 * area_scope.h
 *
 * Contain the area based on Cells or Tracking Areas.
 */

#ifndef _OpenAPI_area_scope_H_
#define _OpenAPI_area_scope_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_area_scope_s OpenAPI_area_scope_t;
#include "cag_info_1.h"
#include "cell_id_nid_info.h"
#include "nid_info.h"
#include "tac_info.h"
#include "tac_nid_info.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_area_scope_s {
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

OpenAPI_area_scope_t *OpenAPI_area_scope_create(
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
void OpenAPI_area_scope_free(OpenAPI_area_scope_t *area_scope);
OpenAPI_area_scope_t *OpenAPI_area_scope_parseFromJSON(cJSON *area_scopeJSON);
cJSON *OpenAPI_area_scope_convertToJSON(OpenAPI_area_scope_t *area_scope);
OpenAPI_area_scope_t *OpenAPI_area_scope_copy(OpenAPI_area_scope_t *dst, OpenAPI_area_scope_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_area_scope_H_ */

