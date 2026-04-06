/*
 * upf_functionality_data_1.h
 *
 * UPF Functionality Data
 */

#ifndef _OpenAPI_upf_functionality_data_1_H_
#define _OpenAPI_upf_functionality_data_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_upf_functionality_data_1_s OpenAPI_upf_functionality_data_1_t;
#include "upf_packet_inspection_functionality.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_upf_functionality_data_1_s {
    OpenAPI_upf_packet_inspection_functionality_e packet_insfunc;
    char *op_confupf_capa;
};

OpenAPI_upf_functionality_data_1_t *OpenAPI_upf_functionality_data_1_create(
    OpenAPI_upf_packet_inspection_functionality_e packet_insfunc,
    char *op_confupf_capa
);
void OpenAPI_upf_functionality_data_1_free(OpenAPI_upf_functionality_data_1_t *upf_functionality_data_1);
OpenAPI_upf_functionality_data_1_t *OpenAPI_upf_functionality_data_1_parseFromJSON(cJSON *upf_functionality_data_1JSON);
cJSON *OpenAPI_upf_functionality_data_1_convertToJSON(OpenAPI_upf_functionality_data_1_t *upf_functionality_data_1);
OpenAPI_upf_functionality_data_1_t *OpenAPI_upf_functionality_data_1_copy(OpenAPI_upf_functionality_data_1_t *dst, OpenAPI_upf_functionality_data_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_upf_functionality_data_1_H_ */

