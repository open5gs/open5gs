/*
 * odb_exempted_dnn_info_1.h
 *
 * ODB Exempted DNN Information
 */

#ifndef _OpenAPI_odb_exempted_dnn_info_1_H_
#define _OpenAPI_odb_exempted_dnn_info_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_odb_exempted_dnn_info_1_s OpenAPI_odb_exempted_dnn_info_1_t;
#include "dnn_configuration_1.h"
#include "odb_exempted_conditions_1.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_odb_exempted_dnn_info_1_s {
    int odb_exempted_ind;
    struct OpenAPI_dnn_configuration_1_s *dnn_configuration;
    OpenAPI_list_t* odb_exempted_conditions_data;
};

OpenAPI_odb_exempted_dnn_info_1_t *OpenAPI_odb_exempted_dnn_info_1_create(
    int odb_exempted_ind,
    OpenAPI_dnn_configuration_1_t *dnn_configuration,
    OpenAPI_list_t* odb_exempted_conditions_data
);
void OpenAPI_odb_exempted_dnn_info_1_free(OpenAPI_odb_exempted_dnn_info_1_t *odb_exempted_dnn_info_1);
OpenAPI_odb_exempted_dnn_info_1_t *OpenAPI_odb_exempted_dnn_info_1_parseFromJSON(cJSON *odb_exempted_dnn_info_1JSON);
cJSON *OpenAPI_odb_exempted_dnn_info_1_convertToJSON(OpenAPI_odb_exempted_dnn_info_1_t *odb_exempted_dnn_info_1);
OpenAPI_odb_exempted_dnn_info_1_t *OpenAPI_odb_exempted_dnn_info_1_copy(OpenAPI_odb_exempted_dnn_info_1_t *dst, OpenAPI_odb_exempted_dnn_info_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_odb_exempted_dnn_info_1_H_ */

