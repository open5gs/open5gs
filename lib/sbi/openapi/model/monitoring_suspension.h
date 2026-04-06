/*
 * monitoring_suspension.h
 *
 * 
 */

#ifndef _OpenAPI_monitoring_suspension_H_
#define _OpenAPI_monitoring_suspension_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_monitoring_suspension_s OpenAPI_monitoring_suspension_t;
#include "plmn_id_nid.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_monitoring_suspension_s {
    OpenAPI_list_t *suspended_inside_plmn_list;
    OpenAPI_list_t *suspended_outside_plmn_list;
};

OpenAPI_monitoring_suspension_t *OpenAPI_monitoring_suspension_create(
    OpenAPI_list_t *suspended_inside_plmn_list,
    OpenAPI_list_t *suspended_outside_plmn_list
);
void OpenAPI_monitoring_suspension_free(OpenAPI_monitoring_suspension_t *monitoring_suspension);
OpenAPI_monitoring_suspension_t *OpenAPI_monitoring_suspension_parseFromJSON(cJSON *monitoring_suspensionJSON);
cJSON *OpenAPI_monitoring_suspension_convertToJSON(OpenAPI_monitoring_suspension_t *monitoring_suspension);
OpenAPI_monitoring_suspension_t *OpenAPI_monitoring_suspension_copy(OpenAPI_monitoring_suspension_t *dst, OpenAPI_monitoring_suspension_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_monitoring_suspension_H_ */

