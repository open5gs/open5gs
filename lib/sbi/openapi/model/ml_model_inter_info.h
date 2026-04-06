/*
 * ml_model_inter_info.h
 *
 * ML Model Interoperability Information
 */

#ifndef _OpenAPI_ml_model_inter_info_H_
#define _OpenAPI_ml_model_inter_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_ml_model_inter_info_s OpenAPI_ml_model_inter_info_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_ml_model_inter_info_s {
    OpenAPI_list_t *vendor_list;
};

OpenAPI_ml_model_inter_info_t *OpenAPI_ml_model_inter_info_create(
    OpenAPI_list_t *vendor_list
);
void OpenAPI_ml_model_inter_info_free(OpenAPI_ml_model_inter_info_t *ml_model_inter_info);
OpenAPI_ml_model_inter_info_t *OpenAPI_ml_model_inter_info_parseFromJSON(cJSON *ml_model_inter_infoJSON);
cJSON *OpenAPI_ml_model_inter_info_convertToJSON(OpenAPI_ml_model_inter_info_t *ml_model_inter_info);
OpenAPI_ml_model_inter_info_t *OpenAPI_ml_model_inter_info_copy(OpenAPI_ml_model_inter_info_t *dst, OpenAPI_ml_model_inter_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ml_model_inter_info_H_ */

