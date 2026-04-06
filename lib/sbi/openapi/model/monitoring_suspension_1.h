/*
 * monitoring_suspension_1.h
 *
 * 
 */

#ifndef _OpenAPI_monitoring_suspension_1_H_
#define _OpenAPI_monitoring_suspension_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_monitoring_suspension_1_s OpenAPI_monitoring_suspension_1_t;
#include "plmn_id_nid_1.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_monitoring_suspension_1_s {
    OpenAPI_list_t *suspended_inside_plmn_list;
    OpenAPI_list_t *suspended_outside_plmn_list;
};

OpenAPI_monitoring_suspension_1_t *OpenAPI_monitoring_suspension_1_create(
    OpenAPI_list_t *suspended_inside_plmn_list,
    OpenAPI_list_t *suspended_outside_plmn_list
);
void OpenAPI_monitoring_suspension_1_free(OpenAPI_monitoring_suspension_1_t *monitoring_suspension_1);
OpenAPI_monitoring_suspension_1_t *OpenAPI_monitoring_suspension_1_parseFromJSON(cJSON *monitoring_suspension_1JSON);
cJSON *OpenAPI_monitoring_suspension_1_convertToJSON(OpenAPI_monitoring_suspension_1_t *monitoring_suspension_1);
OpenAPI_monitoring_suspension_1_t *OpenAPI_monitoring_suspension_1_copy(OpenAPI_monitoring_suspension_1_t *dst, OpenAPI_monitoring_suspension_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_monitoring_suspension_1_H_ */

