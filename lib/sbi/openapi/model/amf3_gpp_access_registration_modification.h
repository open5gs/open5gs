/*
 * amf3_gpp_access_registration_modification.h
 *
 *
 */

#ifndef _OpenAPI_amf3_gpp_access_registration_modification_H_
#define _OpenAPI_amf3_gpp_access_registration_modification_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "backup_amf_info.h"
#include "eps_interworking_info.h"
#include "guami.h"
#include "ims_vo_ps.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_amf3_gpp_access_registration_modification_s OpenAPI_amf3_gpp_access_registration_modification_t;
typedef struct OpenAPI_amf3_gpp_access_registration_modification_s {
    struct OpenAPI_guami_s *guami;
    int purge_flag;
    char *pei;
    struct OpenAPI_ims_vo_ps_s *ims_vo_ps;
    OpenAPI_list_t *backup_amf_info;
    struct OpenAPI_eps_interworking_info_s *eps_interworking_info;
    int ue_srvcc_capability;
} OpenAPI_amf3_gpp_access_registration_modification_t;

OpenAPI_amf3_gpp_access_registration_modification_t *OpenAPI_amf3_gpp_access_registration_modification_create(
    OpenAPI_guami_t *guami,
    int purge_flag,
    char *pei,
    OpenAPI_ims_vo_ps_t *ims_vo_ps,
    OpenAPI_list_t *backup_amf_info,
    OpenAPI_eps_interworking_info_t *eps_interworking_info,
    int ue_srvcc_capability
    );
void OpenAPI_amf3_gpp_access_registration_modification_free(OpenAPI_amf3_gpp_access_registration_modification_t *amf3_gpp_access_registration_modification);
OpenAPI_amf3_gpp_access_registration_modification_t *OpenAPI_amf3_gpp_access_registration_modification_parseFromJSON(cJSON *amf3_gpp_access_registration_modificationJSON);
cJSON *OpenAPI_amf3_gpp_access_registration_modification_convertToJSON(OpenAPI_amf3_gpp_access_registration_modification_t *amf3_gpp_access_registration_modification);
OpenAPI_amf3_gpp_access_registration_modification_t *OpenAPI_amf3_gpp_access_registration_modification_copy(OpenAPI_amf3_gpp_access_registration_modification_t *dst, OpenAPI_amf3_gpp_access_registration_modification_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_amf3_gpp_access_registration_modification_H_ */

