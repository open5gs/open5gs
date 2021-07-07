/*
 * steering_info.h
 *
 * 
 */

#ifndef _OpenAPI_steering_info_H_
#define _OpenAPI_steering_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "access_tech.h"
#include "plmn_id_1.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_steering_info_s OpenAPI_steering_info_t;
typedef struct OpenAPI_steering_info_s {
    struct OpenAPI_plmn_id_1_s *plmn_id;
    OpenAPI_list_t *access_tech_list;
} OpenAPI_steering_info_t;

OpenAPI_steering_info_t *OpenAPI_steering_info_create(
    OpenAPI_plmn_id_1_t *plmn_id,
    OpenAPI_list_t *access_tech_list
);
void OpenAPI_steering_info_free(OpenAPI_steering_info_t *steering_info);
OpenAPI_steering_info_t *OpenAPI_steering_info_parseFromJSON(cJSON *steering_infoJSON);
cJSON *OpenAPI_steering_info_convertToJSON(OpenAPI_steering_info_t *steering_info);
OpenAPI_steering_info_t *OpenAPI_steering_info_copy(OpenAPI_steering_info_t *dst, OpenAPI_steering_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_steering_info_H_ */

