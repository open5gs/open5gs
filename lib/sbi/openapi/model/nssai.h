/*
 * nssai.h
 *
 *
 */

#ifndef _OpenAPI_nssai_H_
#define _OpenAPI_nssai_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "additional_snssai_data.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nssai_s OpenAPI_nssai_t;
typedef struct OpenAPI_nssai_s {
    char *supported_features;
    OpenAPI_list_t *default_single_nssais;
    OpenAPI_list_t *single_nssais;
    char *provisioning_time;
    OpenAPI_list_t* additional_snssai_data;
} OpenAPI_nssai_t;

OpenAPI_nssai_t *OpenAPI_nssai_create(
    char *supported_features,
    OpenAPI_list_t *default_single_nssais,
    OpenAPI_list_t *single_nssais,
    char *provisioning_time,
    OpenAPI_list_t* additional_snssai_data
    );
void OpenAPI_nssai_free(OpenAPI_nssai_t *nssai);
OpenAPI_nssai_t *OpenAPI_nssai_parseFromJSON(cJSON *nssaiJSON);
cJSON *OpenAPI_nssai_convertToJSON(OpenAPI_nssai_t *nssai);
OpenAPI_nssai_t *OpenAPI_nssai_copy(OpenAPI_nssai_t *dst, OpenAPI_nssai_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nssai_H_ */

