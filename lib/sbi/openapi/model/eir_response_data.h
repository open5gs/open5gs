/*
 * eir_response_data.h
 *
 * Represents equipment status data provided in an EIR response message.
 */

#ifndef _OpenAPI_eir_response_data_H_
#define _OpenAPI_eir_response_data_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_eir_response_data_s OpenAPI_eir_response_data_t;
#include "equipment_status.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_eir_response_data_s {
    OpenAPI_equipment_status_e status;
};

OpenAPI_eir_response_data_t *OpenAPI_eir_response_data_create(
    OpenAPI_equipment_status_e status
);
void OpenAPI_eir_response_data_free(OpenAPI_eir_response_data_t *eir_response_data);
OpenAPI_eir_response_data_t *OpenAPI_eir_response_data_parseFromJSON(cJSON *eir_response_dataJSON);
cJSON *OpenAPI_eir_response_data_convertToJSON(OpenAPI_eir_response_data_t *eir_response_data);
OpenAPI_eir_response_data_t *OpenAPI_eir_response_data_copy(OpenAPI_eir_response_data_t *dst, OpenAPI_eir_response_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_eir_response_data_H_ */

