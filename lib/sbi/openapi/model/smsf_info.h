/*
 * smsf_info.h
 *
 *
 */

#ifndef _OpenAPI_smsf_info_H_
#define _OpenAPI_smsf_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "plmn_id.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_smsf_info_s OpenAPI_smsf_info_t;
typedef struct OpenAPI_smsf_info_s {
    char *smsf_instance_id;
    struct OpenAPI_plmn_id_s *plmn_id;
} OpenAPI_smsf_info_t;

OpenAPI_smsf_info_t *OpenAPI_smsf_info_create(
    char *smsf_instance_id,
    OpenAPI_plmn_id_t *plmn_id
    );
void OpenAPI_smsf_info_free(OpenAPI_smsf_info_t *smsf_info);
OpenAPI_smsf_info_t *OpenAPI_smsf_info_parseFromJSON(cJSON *smsf_infoJSON);
cJSON *OpenAPI_smsf_info_convertToJSON(OpenAPI_smsf_info_t *smsf_info);
OpenAPI_smsf_info_t *OpenAPI_smsf_info_copy(OpenAPI_smsf_info_t *dst, OpenAPI_smsf_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_smsf_info_H_ */

