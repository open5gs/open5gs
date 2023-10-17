/*
 * dnn_snssai_information.h
 *
 * Represents a (DNN, SNSSAI) combination.
 */

#ifndef _OpenAPI_dnn_snssai_information_H_
#define _OpenAPI_dnn_snssai_information_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_dnn_snssai_information_s OpenAPI_dnn_snssai_information_t;
typedef struct OpenAPI_dnn_snssai_information_s {
    char *dnn;
    struct OpenAPI_snssai_s *snssai;
} OpenAPI_dnn_snssai_information_t;

OpenAPI_dnn_snssai_information_t *OpenAPI_dnn_snssai_information_create(
    char *dnn,
    OpenAPI_snssai_t *snssai
);
void OpenAPI_dnn_snssai_information_free(OpenAPI_dnn_snssai_information_t *dnn_snssai_information);
OpenAPI_dnn_snssai_information_t *OpenAPI_dnn_snssai_information_parseFromJSON(cJSON *dnn_snssai_informationJSON);
cJSON *OpenAPI_dnn_snssai_information_convertToJSON(OpenAPI_dnn_snssai_information_t *dnn_snssai_information);
OpenAPI_dnn_snssai_information_t *OpenAPI_dnn_snssai_information_copy(OpenAPI_dnn_snssai_information_t *dst, OpenAPI_dnn_snssai_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_dnn_snssai_information_H_ */

