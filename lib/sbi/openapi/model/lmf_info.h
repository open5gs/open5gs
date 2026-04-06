/*
 * lmf_info.h
 *
 * Information of an LMF NF Instance
 */

#ifndef _OpenAPI_lmf_info_H_
#define _OpenAPI_lmf_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_lmf_info_s OpenAPI_lmf_info_t;
#include "access_type.h"
#include "an_node_type.h"
#include "external_client_type.h"
#include "pru_existence_info.h"
#include "rat_type.h"
#include "supported_gad_shapes.h"
#include "tai.h"
#include "tai_range.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_lmf_info_s {
    OpenAPI_list_t *serving_client_types;
    char *lmf_id;
    OpenAPI_list_t *serving_access_types;
    OpenAPI_list_t *serving_an_node_types;
    OpenAPI_list_t *serving_rat_types;
    OpenAPI_list_t *tai_list;
    OpenAPI_list_t *tai_range_list;
    OpenAPI_list_t *supported_gad_shapes;
    struct OpenAPI_pru_existence_info_s *pru_existence_info;
    bool is_pru_support_ind;
    int pru_support_ind;
    bool is_rangingslpos_support_ind;
    int rangingslpos_support_ind;
    bool is_up_positioning_ind;
    int up_positioning_ind;
    bool is_aiml_pos_ind;
    int aiml_pos_ind;
};

OpenAPI_lmf_info_t *OpenAPI_lmf_info_create(
    OpenAPI_list_t *serving_client_types,
    char *lmf_id,
    OpenAPI_list_t *serving_access_types,
    OpenAPI_list_t *serving_an_node_types,
    OpenAPI_list_t *serving_rat_types,
    OpenAPI_list_t *tai_list,
    OpenAPI_list_t *tai_range_list,
    OpenAPI_list_t *supported_gad_shapes,
    OpenAPI_pru_existence_info_t *pru_existence_info,
    bool is_pru_support_ind,
    int pru_support_ind,
    bool is_rangingslpos_support_ind,
    int rangingslpos_support_ind,
    bool is_up_positioning_ind,
    int up_positioning_ind,
    bool is_aiml_pos_ind,
    int aiml_pos_ind
);
void OpenAPI_lmf_info_free(OpenAPI_lmf_info_t *lmf_info);
OpenAPI_lmf_info_t *OpenAPI_lmf_info_parseFromJSON(cJSON *lmf_infoJSON);
cJSON *OpenAPI_lmf_info_convertToJSON(OpenAPI_lmf_info_t *lmf_info);
OpenAPI_lmf_info_t *OpenAPI_lmf_info_copy(OpenAPI_lmf_info_t *dst, OpenAPI_lmf_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_lmf_info_H_ */

