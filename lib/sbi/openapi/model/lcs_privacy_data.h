/*
 * lcs_privacy_data.h
 *
 * Contains LCS Privacy Data
 */

#ifndef _OpenAPI_lcs_privacy_data_H_
#define _OpenAPI_lcs_privacy_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_lcs_privacy_data_s OpenAPI_lcs_privacy_data_t;
#include "area_usage_ind.h"
#include "geographic_area.h"
#include "lpi.h"
#include "plmn_operator_class.h"
#include "unrelated_class.h"
#include "up_loc_rep_ind_af.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_lcs_privacy_data_s {
    struct OpenAPI_lpi_s *lpi;
    struct OpenAPI_unrelated_class_s *unrelated_class;
    OpenAPI_list_t *plmn_operator_classes;
    struct OpenAPI_geographic_area_s *evt_rpt_expected_area;
    OpenAPI_area_usage_ind_e area_usage_ind;
    OpenAPI_up_loc_rep_ind_af_e up_loc_rep_ind_af;
};

OpenAPI_lcs_privacy_data_t *OpenAPI_lcs_privacy_data_create(
    OpenAPI_lpi_t *lpi,
    OpenAPI_unrelated_class_t *unrelated_class,
    OpenAPI_list_t *plmn_operator_classes,
    OpenAPI_geographic_area_t *evt_rpt_expected_area,
    OpenAPI_area_usage_ind_e area_usage_ind,
    OpenAPI_up_loc_rep_ind_af_e up_loc_rep_ind_af
);
void OpenAPI_lcs_privacy_data_free(OpenAPI_lcs_privacy_data_t *lcs_privacy_data);
OpenAPI_lcs_privacy_data_t *OpenAPI_lcs_privacy_data_parseFromJSON(cJSON *lcs_privacy_dataJSON);
cJSON *OpenAPI_lcs_privacy_data_convertToJSON(OpenAPI_lcs_privacy_data_t *lcs_privacy_data);
OpenAPI_lcs_privacy_data_t *OpenAPI_lcs_privacy_data_copy(OpenAPI_lcs_privacy_data_t *dst, OpenAPI_lcs_privacy_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_lcs_privacy_data_H_ */

