/*
 * adrf_info.h
 *
 * Information of an ADRF NF Instance
 */

#ifndef _OpenAPI_adrf_info_H_
#define _OpenAPI_adrf_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_adrf_info_s OpenAPI_adrf_info_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_adrf_info_s {
    bool is_ml_model_storage_ind;
    int ml_model_storage_ind;
    bool is_data_storage_ind;
    int data_storage_ind;
};

OpenAPI_adrf_info_t *OpenAPI_adrf_info_create(
    bool is_ml_model_storage_ind,
    int ml_model_storage_ind,
    bool is_data_storage_ind,
    int data_storage_ind
);
void OpenAPI_adrf_info_free(OpenAPI_adrf_info_t *adrf_info);
OpenAPI_adrf_info_t *OpenAPI_adrf_info_parseFromJSON(cJSON *adrf_infoJSON);
cJSON *OpenAPI_adrf_info_convertToJSON(OpenAPI_adrf_info_t *adrf_info);
OpenAPI_adrf_info_t *OpenAPI_adrf_info_copy(OpenAPI_adrf_info_t *dst, OpenAPI_adrf_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_adrf_info_H_ */

