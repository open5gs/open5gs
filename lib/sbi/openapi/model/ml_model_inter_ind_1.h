/*
 * ml_model_inter_ind_1.h
 *
 * ML Model Interoperability Indicator per Analytics Id
 */

#ifndef _OpenAPI_ml_model_inter_ind_1_H_
#define _OpenAPI_ml_model_inter_ind_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_ml_model_inter_ind_1_s OpenAPI_ml_model_inter_ind_1_t;
#include "nwdaf_event.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_ml_model_inter_ind_1_s {
    OpenAPI_nwdaf_event_e analytics_id;
    OpenAPI_list_t *vendor_list;
};

OpenAPI_ml_model_inter_ind_1_t *OpenAPI_ml_model_inter_ind_1_create(
    OpenAPI_nwdaf_event_e analytics_id,
    OpenAPI_list_t *vendor_list
);
void OpenAPI_ml_model_inter_ind_1_free(OpenAPI_ml_model_inter_ind_1_t *ml_model_inter_ind_1);
OpenAPI_ml_model_inter_ind_1_t *OpenAPI_ml_model_inter_ind_1_parseFromJSON(cJSON *ml_model_inter_ind_1JSON);
cJSON *OpenAPI_ml_model_inter_ind_1_convertToJSON(OpenAPI_ml_model_inter_ind_1_t *ml_model_inter_ind_1);
OpenAPI_ml_model_inter_ind_1_t *OpenAPI_ml_model_inter_ind_1_copy(OpenAPI_ml_model_inter_ind_1_t *dst, OpenAPI_ml_model_inter_ind_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ml_model_inter_ind_1_H_ */

