/*
 * nssai_1.h
 *
 * 
 */

#ifndef _OpenAPI_nssai_1_H_
#define _OpenAPI_nssai_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "additional_snssai_data_1.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nssai_1_s OpenAPI_nssai_1_t;
typedef struct OpenAPI_nssai_1_s {
    char *supported_features;
    OpenAPI_list_t *default_single_nssais;
    OpenAPI_list_t *single_nssais;
    char *provisioning_time;
    OpenAPI_list_t* additional_snssai_data;
    bool is_suppress_nssrg_ind;
    int suppress_nssrg_ind;
} OpenAPI_nssai_1_t;

OpenAPI_nssai_1_t *OpenAPI_nssai_1_create(
    char *supported_features,
    OpenAPI_list_t *default_single_nssais,
    OpenAPI_list_t *single_nssais,
    char *provisioning_time,
    OpenAPI_list_t* additional_snssai_data,
    bool is_suppress_nssrg_ind,
    int suppress_nssrg_ind
);
void OpenAPI_nssai_1_free(OpenAPI_nssai_1_t *nssai_1);
OpenAPI_nssai_1_t *OpenAPI_nssai_1_parseFromJSON(cJSON *nssai_1JSON);
cJSON *OpenAPI_nssai_1_convertToJSON(OpenAPI_nssai_1_t *nssai_1);
OpenAPI_nssai_1_t *OpenAPI_nssai_1_copy(OpenAPI_nssai_1_t *dst, OpenAPI_nssai_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nssai_1_H_ */

