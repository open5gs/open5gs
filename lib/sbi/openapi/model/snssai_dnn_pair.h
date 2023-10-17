/*
 * snssai_dnn_pair.h
 *
 * Contains a S-NSSAI and DNN combination.
 */

#ifndef _OpenAPI_snssai_dnn_pair_H_
#define _OpenAPI_snssai_dnn_pair_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_snssai_dnn_pair_s OpenAPI_snssai_dnn_pair_t;
typedef struct OpenAPI_snssai_dnn_pair_s {
    char *dnn;
    struct OpenAPI_snssai_s *snssai;
} OpenAPI_snssai_dnn_pair_t;

OpenAPI_snssai_dnn_pair_t *OpenAPI_snssai_dnn_pair_create(
    char *dnn,
    OpenAPI_snssai_t *snssai
);
void OpenAPI_snssai_dnn_pair_free(OpenAPI_snssai_dnn_pair_t *snssai_dnn_pair);
OpenAPI_snssai_dnn_pair_t *OpenAPI_snssai_dnn_pair_parseFromJSON(cJSON *snssai_dnn_pairJSON);
cJSON *OpenAPI_snssai_dnn_pair_convertToJSON(OpenAPI_snssai_dnn_pair_t *snssai_dnn_pair);
OpenAPI_snssai_dnn_pair_t *OpenAPI_snssai_dnn_pair_copy(OpenAPI_snssai_dnn_pair_t *dst, OpenAPI_snssai_dnn_pair_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_snssai_dnn_pair_H_ */

