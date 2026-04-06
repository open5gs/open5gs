/*
 * upf_functionality_data.h
 *
 * UPF Functionality Data
 */

#ifndef _OpenAPI_upf_functionality_data_H_
#define _OpenAPI_upf_functionality_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_upf_functionality_data_s OpenAPI_upf_functionality_data_t;
#include "upf_packet_inspection_functionality.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_upf_functionality_data_s {
    OpenAPI_upf_packet_inspection_functionality_e packet_insfunc;
    char *op_confupf_capa;
};

OpenAPI_upf_functionality_data_t *OpenAPI_upf_functionality_data_create(
    OpenAPI_upf_packet_inspection_functionality_e packet_insfunc,
    char *op_confupf_capa
);
void OpenAPI_upf_functionality_data_free(OpenAPI_upf_functionality_data_t *upf_functionality_data);
OpenAPI_upf_functionality_data_t *OpenAPI_upf_functionality_data_parseFromJSON(cJSON *upf_functionality_dataJSON);
cJSON *OpenAPI_upf_functionality_data_convertToJSON(OpenAPI_upf_functionality_data_t *upf_functionality_data);
OpenAPI_upf_functionality_data_t *OpenAPI_upf_functionality_data_copy(OpenAPI_upf_functionality_data_t *dst, OpenAPI_upf_functionality_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_upf_functionality_data_H_ */

