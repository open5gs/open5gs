/*
 * model_5_gs_bridge_information.h
 *
 * 
 */

#ifndef _OpenAPI_model_5_gs_bridge_information_H_
#define _OpenAPI_model_5_gs_bridge_information_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_model_5_gs_bridge_information_s OpenAPI_model_5_gs_bridge_information_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_model_5_gs_bridge_information_s {
    long bridge_id;
    bool is_n_wtt_port_number;
    int n_wtt_port_number;
    bool is_d_stt_port_number;
    int d_stt_port_number;
};

OpenAPI_model_5_gs_bridge_information_t *OpenAPI_model_5_gs_bridge_information_create(
    long bridge_id,
    bool is_n_wtt_port_number,
    int n_wtt_port_number,
    bool is_d_stt_port_number,
    int d_stt_port_number
);
void OpenAPI_model_5_gs_bridge_information_free(OpenAPI_model_5_gs_bridge_information_t *model_5_gs_bridge_information);
OpenAPI_model_5_gs_bridge_information_t *OpenAPI_model_5_gs_bridge_information_parseFromJSON(cJSON *model_5_gs_bridge_informationJSON);
cJSON *OpenAPI_model_5_gs_bridge_information_convertToJSON(OpenAPI_model_5_gs_bridge_information_t *model_5_gs_bridge_information);
OpenAPI_model_5_gs_bridge_information_t *OpenAPI_model_5_gs_bridge_information_copy(OpenAPI_model_5_gs_bridge_information_t *dst, OpenAPI_model_5_gs_bridge_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_model_5_gs_bridge_information_H_ */

