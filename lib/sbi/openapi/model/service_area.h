/*
 * service_area.h
 *
 * 
 */

#ifndef _OpenAPI_service_area_H_
#define _OpenAPI_service_area_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_service_area_s OpenAPI_service_area_t;
#include "global_ran_node_id.h"
#include "mbs_service_area.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_service_area_s {
    bool is_m_bs_service_area_null;
    struct OpenAPI_mbs_service_area_s *m_bs_service_area;
    OpenAPI_list_t *u_pfids;
    OpenAPI_list_t *ran_node_ids;
};

OpenAPI_service_area_t *OpenAPI_service_area_create(
    bool is_m_bs_service_area_null,
    OpenAPI_mbs_service_area_t *m_bs_service_area,
    OpenAPI_list_t *u_pfids,
    OpenAPI_list_t *ran_node_ids
);
void OpenAPI_service_area_free(OpenAPI_service_area_t *service_area);
OpenAPI_service_area_t *OpenAPI_service_area_parseFromJSON(cJSON *service_areaJSON);
cJSON *OpenAPI_service_area_convertToJSON(OpenAPI_service_area_t *service_area);
OpenAPI_service_area_t *OpenAPI_service_area_copy(OpenAPI_service_area_t *dst, OpenAPI_service_area_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_service_area_H_ */

