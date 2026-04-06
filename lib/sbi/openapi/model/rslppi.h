/*
 * rslppi.h
 *
 * Describes Ranging and Sidelink Positioning Privacy Indication.
 */

#ifndef _OpenAPI_rslppi_H_
#define _OpenAPI_rslppi_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_rslppi_s OpenAPI_rslppi_t;
#include "ranging_sl_privacy_ind.h"
#include "valid_time_period.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_rslppi_s {
    OpenAPI_ranging_sl_privacy_ind_e ranging_sl_privacy_ind;
    struct OpenAPI_valid_time_period_s *valid_time_period;
};

OpenAPI_rslppi_t *OpenAPI_rslppi_create(
    OpenAPI_ranging_sl_privacy_ind_e ranging_sl_privacy_ind,
    OpenAPI_valid_time_period_t *valid_time_period
);
void OpenAPI_rslppi_free(OpenAPI_rslppi_t *rslppi);
OpenAPI_rslppi_t *OpenAPI_rslppi_parseFromJSON(cJSON *rslppiJSON);
cJSON *OpenAPI_rslppi_convertToJSON(OpenAPI_rslppi_t *rslppi);
OpenAPI_rslppi_t *OpenAPI_rslppi_copy(OpenAPI_rslppi_t *dst, OpenAPI_rslppi_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_rslppi_H_ */

