/*
 * snssai_route_selection_descriptor.h
 *
 * Contains the route selector parameters (DNNs, PDU session types and SSC modes) per SNSSAI
 */

#ifndef _OpenAPI_snssai_route_selection_descriptor_H_
#define _OpenAPI_snssai_route_selection_descriptor_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "dnn_route_selection_descriptor.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_snssai_route_selection_descriptor_s OpenAPI_snssai_route_selection_descriptor_t;
typedef struct OpenAPI_snssai_route_selection_descriptor_s {
    struct OpenAPI_snssai_s *snssai;
    OpenAPI_list_t *dnn_route_sel_descs;
} OpenAPI_snssai_route_selection_descriptor_t;

OpenAPI_snssai_route_selection_descriptor_t *OpenAPI_snssai_route_selection_descriptor_create(
    OpenAPI_snssai_t *snssai,
    OpenAPI_list_t *dnn_route_sel_descs
    );
void OpenAPI_snssai_route_selection_descriptor_free(OpenAPI_snssai_route_selection_descriptor_t *snssai_route_selection_descriptor);
OpenAPI_snssai_route_selection_descriptor_t *OpenAPI_snssai_route_selection_descriptor_parseFromJSON(cJSON *snssai_route_selection_descriptorJSON);
cJSON *OpenAPI_snssai_route_selection_descriptor_convertToJSON(OpenAPI_snssai_route_selection_descriptor_t *snssai_route_selection_descriptor);
OpenAPI_snssai_route_selection_descriptor_t *OpenAPI_snssai_route_selection_descriptor_copy(OpenAPI_snssai_route_selection_descriptor_t *dst, OpenAPI_snssai_route_selection_descriptor_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_snssai_route_selection_descriptor_H_ */

