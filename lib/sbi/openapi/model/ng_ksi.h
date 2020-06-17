/*
 * ng_ksi.h
 *
 *
 */

#ifndef _OpenAPI_ng_ksi_H_
#define _OpenAPI_ng_ksi_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "sc_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ng_ksi_s OpenAPI_ng_ksi_t;
typedef struct OpenAPI_ng_ksi_s {
    OpenAPI_sc_type_e tsc;
    int ksi;
} OpenAPI_ng_ksi_t;

OpenAPI_ng_ksi_t *OpenAPI_ng_ksi_create(
    OpenAPI_sc_type_e tsc,
    int ksi
    );
void OpenAPI_ng_ksi_free(OpenAPI_ng_ksi_t *ng_ksi);
OpenAPI_ng_ksi_t *OpenAPI_ng_ksi_parseFromJSON(cJSON *ng_ksiJSON);
cJSON *OpenAPI_ng_ksi_convertToJSON(OpenAPI_ng_ksi_t *ng_ksi);
OpenAPI_ng_ksi_t *OpenAPI_ng_ksi_copy(OpenAPI_ng_ksi_t *dst, OpenAPI_ng_ksi_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ng_ksi_H_ */

