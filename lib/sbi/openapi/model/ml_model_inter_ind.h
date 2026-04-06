/*
 * ml_model_inter_ind.h
 *
 * ML Model Interoperability Indicator per Analytics Id
 */

#ifndef _OpenAPI_ml_model_inter_ind_H_
#define _OpenAPI_ml_model_inter_ind_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_ml_model_inter_ind_s OpenAPI_ml_model_inter_ind_t;
#include "nwdaf_event.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_ml_model_inter_ind_s {
    OpenAPI_nwdaf_event_e analytics_id;
    OpenAPI_list_t *vendor_list;
};

OpenAPI_ml_model_inter_ind_t *OpenAPI_ml_model_inter_ind_create(
    OpenAPI_nwdaf_event_e analytics_id,
    OpenAPI_list_t *vendor_list
);
void OpenAPI_ml_model_inter_ind_free(OpenAPI_ml_model_inter_ind_t *ml_model_inter_ind);
OpenAPI_ml_model_inter_ind_t *OpenAPI_ml_model_inter_ind_parseFromJSON(cJSON *ml_model_inter_indJSON);
cJSON *OpenAPI_ml_model_inter_ind_convertToJSON(OpenAPI_ml_model_inter_ind_t *ml_model_inter_ind);
OpenAPI_ml_model_inter_ind_t *OpenAPI_ml_model_inter_ind_copy(OpenAPI_ml_model_inter_ind_t *dst, OpenAPI_ml_model_inter_ind_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ml_model_inter_ind_H_ */

