/*
 * dnn_route_selection_descriptor.h
 *
 * Contains the route selector parameters (PDU session types, SSC modes and ATSSS information) per DNN
 */

#ifndef _OpenAPI_dnn_route_selection_descriptor_H_
#define _OpenAPI_dnn_route_selection_descriptor_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "pdu_session_type.h"
#include "ssc_mode.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_dnn_route_selection_descriptor_s OpenAPI_dnn_route_selection_descriptor_t;
typedef struct OpenAPI_dnn_route_selection_descriptor_s {
    char *dnn;
    OpenAPI_list_t *ssc_modes;
    OpenAPI_list_t *pdu_sess_types;
    bool is_atsss_info;
    int atsss_info;
} OpenAPI_dnn_route_selection_descriptor_t;

OpenAPI_dnn_route_selection_descriptor_t *OpenAPI_dnn_route_selection_descriptor_create(
    char *dnn,
    OpenAPI_list_t *ssc_modes,
    OpenAPI_list_t *pdu_sess_types,
    bool is_atsss_info,
    int atsss_info
);
void OpenAPI_dnn_route_selection_descriptor_free(OpenAPI_dnn_route_selection_descriptor_t *dnn_route_selection_descriptor);
OpenAPI_dnn_route_selection_descriptor_t *OpenAPI_dnn_route_selection_descriptor_parseFromJSON(cJSON *dnn_route_selection_descriptorJSON);
cJSON *OpenAPI_dnn_route_selection_descriptor_convertToJSON(OpenAPI_dnn_route_selection_descriptor_t *dnn_route_selection_descriptor);
OpenAPI_dnn_route_selection_descriptor_t *OpenAPI_dnn_route_selection_descriptor_copy(OpenAPI_dnn_route_selection_descriptor_t *dst, OpenAPI_dnn_route_selection_descriptor_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_dnn_route_selection_descriptor_H_ */

