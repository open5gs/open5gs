/*
 * ranging_sl_privacy_data_1.h
 *
 * Contains Ranging and Sidelink Positioning Privacy Subscription Data.
 */

#ifndef _OpenAPI_ranging_sl_privacy_data_1_H_
#define _OpenAPI_ranging_sl_privacy_data_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_ranging_sl_privacy_data_1_s OpenAPI_ranging_sl_privacy_data_1_t;
#include "geographic_area.h"
#include "ranging_sl_plmn_operator_class_1.h"
#include "ranging_sl_unrelated_class_1.h"
#include "rslppi_1.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_ranging_sl_privacy_data_1_s {
    struct OpenAPI_rslppi_1_s *rslppi;
    struct OpenAPI_ranging_sl_unrelated_class_1_s *ranging_sl_unrelated_class;
    OpenAPI_list_t *ranging_sl_plmn_operator_classes;
    struct OpenAPI_geographic_area_s *ranging_sl_evt_rpt_expected_area;
};

OpenAPI_ranging_sl_privacy_data_1_t *OpenAPI_ranging_sl_privacy_data_1_create(
    OpenAPI_rslppi_1_t *rslppi,
    OpenAPI_ranging_sl_unrelated_class_1_t *ranging_sl_unrelated_class,
    OpenAPI_list_t *ranging_sl_plmn_operator_classes,
    OpenAPI_geographic_area_t *ranging_sl_evt_rpt_expected_area
);
void OpenAPI_ranging_sl_privacy_data_1_free(OpenAPI_ranging_sl_privacy_data_1_t *ranging_sl_privacy_data_1);
OpenAPI_ranging_sl_privacy_data_1_t *OpenAPI_ranging_sl_privacy_data_1_parseFromJSON(cJSON *ranging_sl_privacy_data_1JSON);
cJSON *OpenAPI_ranging_sl_privacy_data_1_convertToJSON(OpenAPI_ranging_sl_privacy_data_1_t *ranging_sl_privacy_data_1);
OpenAPI_ranging_sl_privacy_data_1_t *OpenAPI_ranging_sl_privacy_data_1_copy(OpenAPI_ranging_sl_privacy_data_1_t *dst, OpenAPI_ranging_sl_privacy_data_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ranging_sl_privacy_data_1_H_ */

