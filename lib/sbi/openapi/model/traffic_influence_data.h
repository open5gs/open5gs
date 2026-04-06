/*
 * traffic_influence_data.h
 *
 * Traffic influence data comprising the Service Data Flow description and the Application Function influence on traffic routing Enforcement Control parameters of a PCC rule. 
 */

#ifndef _OpenAPI_traffic_influence_data_H_
#define _OpenAPI_traffic_influence_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_traffic_influence_data_s OpenAPI_traffic_influence_data_t;
#include "flow_information.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_traffic_influence_data_s {
    OpenAPI_list_t *flow_infos;
    char *app_id;
    bool is_precedence;
    int precedence;
    OpenAPI_list_t *ref_tc_data;
};

OpenAPI_traffic_influence_data_t *OpenAPI_traffic_influence_data_create(
    OpenAPI_list_t *flow_infos,
    char *app_id,
    bool is_precedence,
    int precedence,
    OpenAPI_list_t *ref_tc_data
);
void OpenAPI_traffic_influence_data_free(OpenAPI_traffic_influence_data_t *traffic_influence_data);
OpenAPI_traffic_influence_data_t *OpenAPI_traffic_influence_data_parseFromJSON(cJSON *traffic_influence_dataJSON);
cJSON *OpenAPI_traffic_influence_data_convertToJSON(OpenAPI_traffic_influence_data_t *traffic_influence_data);
OpenAPI_traffic_influence_data_t *OpenAPI_traffic_influence_data_copy(OpenAPI_traffic_influence_data_t *dst, OpenAPI_traffic_influence_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_traffic_influence_data_H_ */

