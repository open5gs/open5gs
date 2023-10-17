/*
 * plmn_restriction.h
 *
 * 
 */

#ifndef _OpenAPI_plmn_restriction_H_
#define _OpenAPI_plmn_restriction_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "area.h"
#include "core_network_type.h"
#include "rat_type.h"
#include "service_area_restriction.h"
#include "set.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_plmn_restriction_s OpenAPI_plmn_restriction_t;
typedef struct OpenAPI_plmn_restriction_s {
    OpenAPI_set_t *rat_restrictions;
    OpenAPI_list_t *forbidden_areas;
    struct OpenAPI_service_area_restriction_s *service_area_restriction;
    OpenAPI_list_t *core_network_type_restrictions;
    OpenAPI_set_t *primary_rat_restrictions;
    OpenAPI_set_t *secondary_rat_restrictions;
} OpenAPI_plmn_restriction_t;

OpenAPI_plmn_restriction_t *OpenAPI_plmn_restriction_create(
    OpenAPI_set_t *rat_restrictions,
    OpenAPI_list_t *forbidden_areas,
    OpenAPI_service_area_restriction_t *service_area_restriction,
    OpenAPI_list_t *core_network_type_restrictions,
    OpenAPI_set_t *primary_rat_restrictions,
    OpenAPI_set_t *secondary_rat_restrictions
);
void OpenAPI_plmn_restriction_free(OpenAPI_plmn_restriction_t *plmn_restriction);
OpenAPI_plmn_restriction_t *OpenAPI_plmn_restriction_parseFromJSON(cJSON *plmn_restrictionJSON);
cJSON *OpenAPI_plmn_restriction_convertToJSON(OpenAPI_plmn_restriction_t *plmn_restriction);
OpenAPI_plmn_restriction_t *OpenAPI_plmn_restriction_copy(OpenAPI_plmn_restriction_t *dst, OpenAPI_plmn_restriction_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_plmn_restriction_H_ */

