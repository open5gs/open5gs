/*
 * dnn_info.h
 *
 *
 */

#ifndef _OpenAPI_dnn_info_H_
#define _OpenAPI_dnn_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_dnn_info_s OpenAPI_dnn_info_t;
typedef struct OpenAPI_dnn_info_s {
    char *dnn;
    int default_dnn_indicator;
    int lbo_roaming_allowed;
    int iwk_eps_ind;
    int dnn_barred;
} OpenAPI_dnn_info_t;

OpenAPI_dnn_info_t *OpenAPI_dnn_info_create(
    char *dnn,
    int default_dnn_indicator,
    int lbo_roaming_allowed,
    int iwk_eps_ind,
    int dnn_barred
    );
void OpenAPI_dnn_info_free(OpenAPI_dnn_info_t *dnn_info);
OpenAPI_dnn_info_t *OpenAPI_dnn_info_parseFromJSON(cJSON *dnn_infoJSON);
cJSON *OpenAPI_dnn_info_convertToJSON(OpenAPI_dnn_info_t *dnn_info);
OpenAPI_dnn_info_t *OpenAPI_dnn_info_copy(OpenAPI_dnn_info_t *dst, OpenAPI_dnn_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_dnn_info_H_ */

