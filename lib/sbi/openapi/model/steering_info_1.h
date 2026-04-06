/*
 * steering_info_1.h
 *
 * Contains a combination of one PLMN identity and zero or more access technologies.
 */

#ifndef _OpenAPI_steering_info_1_H_
#define _OpenAPI_steering_info_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_steering_info_1_s OpenAPI_steering_info_1_t;
#include "access_tech.h"
#include "plmn_id.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_steering_info_1_s {
    struct OpenAPI_plmn_id_s *plmn_id;
    OpenAPI_list_t *access_tech_list;
};

OpenAPI_steering_info_1_t *OpenAPI_steering_info_1_create(
    OpenAPI_plmn_id_t *plmn_id,
    OpenAPI_list_t *access_tech_list
);
void OpenAPI_steering_info_1_free(OpenAPI_steering_info_1_t *steering_info_1);
OpenAPI_steering_info_1_t *OpenAPI_steering_info_1_parseFromJSON(cJSON *steering_info_1JSON);
cJSON *OpenAPI_steering_info_1_convertToJSON(OpenAPI_steering_info_1_t *steering_info_1);
OpenAPI_steering_info_1_t *OpenAPI_steering_info_1_copy(OpenAPI_steering_info_1_t *dst, OpenAPI_steering_info_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_steering_info_1_H_ */

