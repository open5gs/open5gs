/*
 * qoe_target_1.h
 *
 * This parameter specifies the target object (individual UE) for the QMC in case of signalling based QMC. It shall be able to carry an IMSI or a SUPI. 
 */

#ifndef _OpenAPI_qoe_target_1_H_
#define _OpenAPI_qoe_target_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_qoe_target_1_s OpenAPI_qoe_target_1_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_qoe_target_1_s {
    char *supi;
    char *imsi;
};

OpenAPI_qoe_target_1_t *OpenAPI_qoe_target_1_create(
    char *supi,
    char *imsi
);
void OpenAPI_qoe_target_1_free(OpenAPI_qoe_target_1_t *qoe_target_1);
OpenAPI_qoe_target_1_t *OpenAPI_qoe_target_1_parseFromJSON(cJSON *qoe_target_1JSON);
cJSON *OpenAPI_qoe_target_1_convertToJSON(OpenAPI_qoe_target_1_t *qoe_target_1);
OpenAPI_qoe_target_1_t *OpenAPI_qoe_target_1_copy(OpenAPI_qoe_target_1_t *dst, OpenAPI_qoe_target_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_qoe_target_1_H_ */

