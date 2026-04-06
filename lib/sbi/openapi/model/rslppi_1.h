/*
 * rslppi_1.h
 *
 * Describes Ranging and Sidelink Positioning Privacy Indication.
 */

#ifndef _OpenAPI_rslppi_1_H_
#define _OpenAPI_rslppi_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_rslppi_1_s OpenAPI_rslppi_1_t;
#include "ranging_sl_privacy_ind.h"
#include "valid_time_period_1.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_rslppi_1_s {
    OpenAPI_ranging_sl_privacy_ind_e ranging_sl_privacy_ind;
    struct OpenAPI_valid_time_period_1_s *valid_time_period;
};

OpenAPI_rslppi_1_t *OpenAPI_rslppi_1_create(
    OpenAPI_ranging_sl_privacy_ind_e ranging_sl_privacy_ind,
    OpenAPI_valid_time_period_1_t *valid_time_period
);
void OpenAPI_rslppi_1_free(OpenAPI_rslppi_1_t *rslppi_1);
OpenAPI_rslppi_1_t *OpenAPI_rslppi_1_parseFromJSON(cJSON *rslppi_1JSON);
cJSON *OpenAPI_rslppi_1_convertToJSON(OpenAPI_rslppi_1_t *rslppi_1);
OpenAPI_rslppi_1_t *OpenAPI_rslppi_1_copy(OpenAPI_rslppi_1_t *dst, OpenAPI_rslppi_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_rslppi_1_H_ */

