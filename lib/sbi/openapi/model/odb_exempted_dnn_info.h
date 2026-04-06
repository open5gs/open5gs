/*
 * odb_exempted_dnn_info.h
 *
 * ODB Exempted DNN Information
 */

#ifndef _OpenAPI_odb_exempted_dnn_info_H_
#define _OpenAPI_odb_exempted_dnn_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_odb_exempted_dnn_info_s OpenAPI_odb_exempted_dnn_info_t;
#include "dnn_configuration.h"
#include "odb_exempted_conditions.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_odb_exempted_dnn_info_s {
    int odb_exempted_ind;
    struct OpenAPI_dnn_configuration_s *dnn_configuration;
    OpenAPI_list_t* odb_exempted_conditions_data;
};

OpenAPI_odb_exempted_dnn_info_t *OpenAPI_odb_exempted_dnn_info_create(
    int odb_exempted_ind,
    OpenAPI_dnn_configuration_t *dnn_configuration,
    OpenAPI_list_t* odb_exempted_conditions_data
);
void OpenAPI_odb_exempted_dnn_info_free(OpenAPI_odb_exempted_dnn_info_t *odb_exempted_dnn_info);
OpenAPI_odb_exempted_dnn_info_t *OpenAPI_odb_exempted_dnn_info_parseFromJSON(cJSON *odb_exempted_dnn_infoJSON);
cJSON *OpenAPI_odb_exempted_dnn_info_convertToJSON(OpenAPI_odb_exempted_dnn_info_t *odb_exempted_dnn_info);
OpenAPI_odb_exempted_dnn_info_t *OpenAPI_odb_exempted_dnn_info_copy(OpenAPI_odb_exempted_dnn_info_t *dst, OpenAPI_odb_exempted_dnn_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_odb_exempted_dnn_info_H_ */

