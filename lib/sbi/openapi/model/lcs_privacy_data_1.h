/*
 * lcs_privacy_data_1.h
 *
 * Contains LCS Privacy Data
 */

#ifndef _OpenAPI_lcs_privacy_data_1_H_
#define _OpenAPI_lcs_privacy_data_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_lcs_privacy_data_1_s OpenAPI_lcs_privacy_data_1_t;
#include "area_usage_ind.h"
#include "geographic_area.h"
#include "lpi_1.h"
#include "plmn_operator_class_1.h"
#include "unrelated_class_1.h"
#include "up_loc_rep_ind_af.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_lcs_privacy_data_1_s {
    struct OpenAPI_lpi_1_s *lpi;
    struct OpenAPI_unrelated_class_1_s *unrelated_class;
    OpenAPI_list_t *plmn_operator_classes;
    struct OpenAPI_geographic_area_s *evt_rpt_expected_area;
    OpenAPI_area_usage_ind_e area_usage_ind;
    OpenAPI_up_loc_rep_ind_af_e up_loc_rep_ind_af;
};

OpenAPI_lcs_privacy_data_1_t *OpenAPI_lcs_privacy_data_1_create(
    OpenAPI_lpi_1_t *lpi,
    OpenAPI_unrelated_class_1_t *unrelated_class,
    OpenAPI_list_t *plmn_operator_classes,
    OpenAPI_geographic_area_t *evt_rpt_expected_area,
    OpenAPI_area_usage_ind_e area_usage_ind,
    OpenAPI_up_loc_rep_ind_af_e up_loc_rep_ind_af
);
void OpenAPI_lcs_privacy_data_1_free(OpenAPI_lcs_privacy_data_1_t *lcs_privacy_data_1);
OpenAPI_lcs_privacy_data_1_t *OpenAPI_lcs_privacy_data_1_parseFromJSON(cJSON *lcs_privacy_data_1JSON);
cJSON *OpenAPI_lcs_privacy_data_1_convertToJSON(OpenAPI_lcs_privacy_data_1_t *lcs_privacy_data_1);
OpenAPI_lcs_privacy_data_1_t *OpenAPI_lcs_privacy_data_1_copy(OpenAPI_lcs_privacy_data_1_t *dst, OpenAPI_lcs_privacy_data_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_lcs_privacy_data_1_H_ */

