/*
 * plmn_restriction_1.h
 *
 * 
 */

#ifndef _OpenAPI_plmn_restriction_1_H_
#define _OpenAPI_plmn_restriction_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "area_1.h"
#include "core_network_type.h"
#include "rat_type.h"
#include "service_area_restriction_1.h"
#include "set.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_plmn_restriction_1_s OpenAPI_plmn_restriction_1_t;
typedef struct OpenAPI_plmn_restriction_1_s {
    OpenAPI_set_t *rat_restrictions;
    OpenAPI_list_t *forbidden_areas;
    struct OpenAPI_service_area_restriction_1_s *service_area_restriction;
    OpenAPI_list_t *core_network_type_restrictions;
    OpenAPI_set_t *primary_rat_restrictions;
    OpenAPI_set_t *secondary_rat_restrictions;
} OpenAPI_plmn_restriction_1_t;

OpenAPI_plmn_restriction_1_t *OpenAPI_plmn_restriction_1_create(
    OpenAPI_set_t *rat_restrictions,
    OpenAPI_list_t *forbidden_areas,
    OpenAPI_service_area_restriction_1_t *service_area_restriction,
    OpenAPI_list_t *core_network_type_restrictions,
    OpenAPI_set_t *primary_rat_restrictions,
    OpenAPI_set_t *secondary_rat_restrictions
);
void OpenAPI_plmn_restriction_1_free(OpenAPI_plmn_restriction_1_t *plmn_restriction_1);
OpenAPI_plmn_restriction_1_t *OpenAPI_plmn_restriction_1_parseFromJSON(cJSON *plmn_restriction_1JSON);
cJSON *OpenAPI_plmn_restriction_1_convertToJSON(OpenAPI_plmn_restriction_1_t *plmn_restriction_1);
OpenAPI_plmn_restriction_1_t *OpenAPI_plmn_restriction_1_copy(OpenAPI_plmn_restriction_1_t *dst, OpenAPI_plmn_restriction_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_plmn_restriction_1_H_ */

