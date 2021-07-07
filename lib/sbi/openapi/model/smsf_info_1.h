/*
 * smsf_info_1.h
 *
 * 
 */

#ifndef _OpenAPI_smsf_info_1_H_
#define _OpenAPI_smsf_info_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "plmn_id_1.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_smsf_info_1_s OpenAPI_smsf_info_1_t;
typedef struct OpenAPI_smsf_info_1_s {
    char *smsf_instance_id;
    struct OpenAPI_plmn_id_1_s *plmn_id;
} OpenAPI_smsf_info_1_t;

OpenAPI_smsf_info_1_t *OpenAPI_smsf_info_1_create(
    char *smsf_instance_id,
    OpenAPI_plmn_id_1_t *plmn_id
);
void OpenAPI_smsf_info_1_free(OpenAPI_smsf_info_1_t *smsf_info_1);
OpenAPI_smsf_info_1_t *OpenAPI_smsf_info_1_parseFromJSON(cJSON *smsf_info_1JSON);
cJSON *OpenAPI_smsf_info_1_convertToJSON(OpenAPI_smsf_info_1_t *smsf_info_1);
OpenAPI_smsf_info_1_t *OpenAPI_smsf_info_1_copy(OpenAPI_smsf_info_1_t *dst, OpenAPI_smsf_info_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_smsf_info_1_H_ */

