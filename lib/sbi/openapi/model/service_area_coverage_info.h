/*
 * service_area_coverage_info.h
 *
 * It represents a list of Tracking Areas within a serving network.
 */

#ifndef _OpenAPI_service_area_coverage_info_H_
#define _OpenAPI_service_area_coverage_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "plmn_id_nid_1.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_service_area_coverage_info_s OpenAPI_service_area_coverage_info_t;
typedef struct OpenAPI_service_area_coverage_info_s {
    OpenAPI_list_t *tac_list;
    struct OpenAPI_plmn_id_nid_1_s *serving_network;
} OpenAPI_service_area_coverage_info_t;

OpenAPI_service_area_coverage_info_t *OpenAPI_service_area_coverage_info_create(
    OpenAPI_list_t *tac_list,
    OpenAPI_plmn_id_nid_1_t *serving_network
);
void OpenAPI_service_area_coverage_info_free(OpenAPI_service_area_coverage_info_t *service_area_coverage_info);
OpenAPI_service_area_coverage_info_t *OpenAPI_service_area_coverage_info_parseFromJSON(cJSON *service_area_coverage_infoJSON);
cJSON *OpenAPI_service_area_coverage_info_convertToJSON(OpenAPI_service_area_coverage_info_t *service_area_coverage_info);
OpenAPI_service_area_coverage_info_t *OpenAPI_service_area_coverage_info_copy(OpenAPI_service_area_coverage_info_t *dst, OpenAPI_service_area_coverage_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_service_area_coverage_info_H_ */

