/*
 * smsf_registration_modification.h
 *
 * Contains attributes of SmsfRegistration that can be modified using PATCH
 */

#ifndef _OpenAPI_smsf_registration_modification_H_
#define _OpenAPI_smsf_registration_modification_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_smsf_registration_modification_s OpenAPI_smsf_registration_modification_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_smsf_registration_modification_s {
    char *smsf_instance_id;
    char *smsf_set_id;
    bool is_ue_memory_available_ind;
    int ue_memory_available_ind;
};

OpenAPI_smsf_registration_modification_t *OpenAPI_smsf_registration_modification_create(
    char *smsf_instance_id,
    char *smsf_set_id,
    bool is_ue_memory_available_ind,
    int ue_memory_available_ind
);
void OpenAPI_smsf_registration_modification_free(OpenAPI_smsf_registration_modification_t *smsf_registration_modification);
OpenAPI_smsf_registration_modification_t *OpenAPI_smsf_registration_modification_parseFromJSON(cJSON *smsf_registration_modificationJSON);
cJSON *OpenAPI_smsf_registration_modification_convertToJSON(OpenAPI_smsf_registration_modification_t *smsf_registration_modification);
OpenAPI_smsf_registration_modification_t *OpenAPI_smsf_registration_modification_copy(OpenAPI_smsf_registration_modification_t *dst, OpenAPI_smsf_registration_modification_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_smsf_registration_modification_H_ */

