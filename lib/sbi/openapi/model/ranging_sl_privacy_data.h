/*
 * ranging_sl_privacy_data.h
 *
 * Contains Ranging and Sidelink Positioning Privacy Subscription Data.
 */

#ifndef _OpenAPI_ranging_sl_privacy_data_H_
#define _OpenAPI_ranging_sl_privacy_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_ranging_sl_privacy_data_s OpenAPI_ranging_sl_privacy_data_t;
#include "geographic_area.h"
#include "ranging_sl_plmn_operator_class.h"
#include "ranging_sl_unrelated_class.h"
#include "rslppi.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_ranging_sl_privacy_data_s {
    struct OpenAPI_rslppi_s *rslppi;
    struct OpenAPI_ranging_sl_unrelated_class_s *ranging_sl_unrelated_class;
    OpenAPI_list_t *ranging_sl_plmn_operator_classes;
    struct OpenAPI_geographic_area_s *ranging_sl_evt_rpt_expected_area;
};

OpenAPI_ranging_sl_privacy_data_t *OpenAPI_ranging_sl_privacy_data_create(
    OpenAPI_rslppi_t *rslppi,
    OpenAPI_ranging_sl_unrelated_class_t *ranging_sl_unrelated_class,
    OpenAPI_list_t *ranging_sl_plmn_operator_classes,
    OpenAPI_geographic_area_t *ranging_sl_evt_rpt_expected_area
);
void OpenAPI_ranging_sl_privacy_data_free(OpenAPI_ranging_sl_privacy_data_t *ranging_sl_privacy_data);
OpenAPI_ranging_sl_privacy_data_t *OpenAPI_ranging_sl_privacy_data_parseFromJSON(cJSON *ranging_sl_privacy_dataJSON);
cJSON *OpenAPI_ranging_sl_privacy_data_convertToJSON(OpenAPI_ranging_sl_privacy_data_t *ranging_sl_privacy_data);
OpenAPI_ranging_sl_privacy_data_t *OpenAPI_ranging_sl_privacy_data_copy(OpenAPI_ranging_sl_privacy_data_t *dst, OpenAPI_ranging_sl_privacy_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ranging_sl_privacy_data_H_ */

