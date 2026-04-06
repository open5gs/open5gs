/*
 * smf_registration_modification.h
 *
 * Contains attributes of SmfRegistration that can be modified using PATCH
 */

#ifndef _OpenAPI_smf_registration_modification_H_
#define _OpenAPI_smf_registration_modification_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_smf_registration_modification_s OpenAPI_smf_registration_modification_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_smf_registration_modification_s {
    char *smf_instance_id;
    char *smf_set_id;
    char *pgw_fqdn;
};

OpenAPI_smf_registration_modification_t *OpenAPI_smf_registration_modification_create(
    char *smf_instance_id,
    char *smf_set_id,
    char *pgw_fqdn
);
void OpenAPI_smf_registration_modification_free(OpenAPI_smf_registration_modification_t *smf_registration_modification);
OpenAPI_smf_registration_modification_t *OpenAPI_smf_registration_modification_parseFromJSON(cJSON *smf_registration_modificationJSON);
cJSON *OpenAPI_smf_registration_modification_convertToJSON(OpenAPI_smf_registration_modification_t *smf_registration_modification);
OpenAPI_smf_registration_modification_t *OpenAPI_smf_registration_modification_copy(OpenAPI_smf_registration_modification_t *dst, OpenAPI_smf_registration_modification_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_smf_registration_modification_H_ */

