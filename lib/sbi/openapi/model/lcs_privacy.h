/*
 * lcs_privacy.h
 *
 * Contains LCS Privacy Parameters
 */

#ifndef _OpenAPI_lcs_privacy_H_
#define _OpenAPI_lcs_privacy_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_lcs_privacy_s OpenAPI_lcs_privacy_t;
#include "area_usage_ind.h"
#include "geographic_area.h"
#include "lpi.h"
#include "up_loc_rep_ind_af.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_lcs_privacy_s {
    char *af_instance_id;
    bool is_reference_id;
    int reference_id;
    struct OpenAPI_lpi_s *lpi;
    char *mtc_provider_information;
    struct OpenAPI_geographic_area_s *evt_rpt_expected_area;
    OpenAPI_area_usage_ind_e area_usage_ind;
    OpenAPI_up_loc_rep_ind_af_e up_loc_rep_ind_af;
};

OpenAPI_lcs_privacy_t *OpenAPI_lcs_privacy_create(
    char *af_instance_id,
    bool is_reference_id,
    int reference_id,
    OpenAPI_lpi_t *lpi,
    char *mtc_provider_information,
    OpenAPI_geographic_area_t *evt_rpt_expected_area,
    OpenAPI_area_usage_ind_e area_usage_ind,
    OpenAPI_up_loc_rep_ind_af_e up_loc_rep_ind_af
);
void OpenAPI_lcs_privacy_free(OpenAPI_lcs_privacy_t *lcs_privacy);
OpenAPI_lcs_privacy_t *OpenAPI_lcs_privacy_parseFromJSON(cJSON *lcs_privacyJSON);
cJSON *OpenAPI_lcs_privacy_convertToJSON(OpenAPI_lcs_privacy_t *lcs_privacy);
OpenAPI_lcs_privacy_t *OpenAPI_lcs_privacy_copy(OpenAPI_lcs_privacy_t *dst, OpenAPI_lcs_privacy_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_lcs_privacy_H_ */

