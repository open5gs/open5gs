/*
 * traffic_influence_info.h
 *
 * Traffic influence information applicable at the VPLMN for an HR-SBO PDU session
 */

#ifndef _OpenAPI_traffic_influence_info_H_
#define _OpenAPI_traffic_influence_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_traffic_influence_info_s OpenAPI_traffic_influence_info_t;
#include "traffic_control_data.h"
#include "traffic_influence_data.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_traffic_influence_info_s {
    OpenAPI_list_t *traff_influ_data;
    OpenAPI_list_t* traff_cont_decs;
};

OpenAPI_traffic_influence_info_t *OpenAPI_traffic_influence_info_create(
    OpenAPI_list_t *traff_influ_data,
    OpenAPI_list_t* traff_cont_decs
);
void OpenAPI_traffic_influence_info_free(OpenAPI_traffic_influence_info_t *traffic_influence_info);
OpenAPI_traffic_influence_info_t *OpenAPI_traffic_influence_info_parseFromJSON(cJSON *traffic_influence_infoJSON);
cJSON *OpenAPI_traffic_influence_info_convertToJSON(OpenAPI_traffic_influence_info_t *traffic_influence_info);
OpenAPI_traffic_influence_info_t *OpenAPI_traffic_influence_info_copy(OpenAPI_traffic_influence_info_t *dst, OpenAPI_traffic_influence_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_traffic_influence_info_H_ */

