/*
 * service_experience_info.h
 *
 * Represents service experience information.
 */

#ifndef _OpenAPI_service_experience_info_H_
#define _OpenAPI_service_experience_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "addr_fqdn.h"
#include "location_info.h"
#include "network_area_info.h"
#include "rat_freq_information.h"
#include "service_experience_type.h"
#include "snssai.h"
#include "svc_experience.h"
#include "upf_information.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_service_experience_info_s OpenAPI_service_experience_info_t;
typedef struct OpenAPI_service_experience_info_s {
    struct OpenAPI_svc_experience_s *svc_exprc;
    bool is_svc_exprc_variance;
    float svc_exprc_variance;
    OpenAPI_list_t *supis;
    struct OpenAPI_snssai_s *snssai;
    char *app_id;
    struct OpenAPI_service_experience_type_s *srv_expc_type;
    OpenAPI_list_t *ue_locs;
    struct OpenAPI_upf_information_s *upf_info;
    char *dnai;
    struct OpenAPI_addr_fqdn_s *app_server_inst;
    bool is_confidence;
    int confidence;
    char *dnn;
    struct OpenAPI_network_area_info_s *network_area;
    char *nsi_id;
    bool is_ratio;
    int ratio;
    struct OpenAPI_rat_freq_information_s *rat_freq;
} OpenAPI_service_experience_info_t;

OpenAPI_service_experience_info_t *OpenAPI_service_experience_info_create(
    OpenAPI_svc_experience_t *svc_exprc,
    bool is_svc_exprc_variance,
    float svc_exprc_variance,
    OpenAPI_list_t *supis,
    OpenAPI_snssai_t *snssai,
    char *app_id,
    OpenAPI_service_experience_type_t *srv_expc_type,
    OpenAPI_list_t *ue_locs,
    OpenAPI_upf_information_t *upf_info,
    char *dnai,
    OpenAPI_addr_fqdn_t *app_server_inst,
    bool is_confidence,
    int confidence,
    char *dnn,
    OpenAPI_network_area_info_t *network_area,
    char *nsi_id,
    bool is_ratio,
    int ratio,
    OpenAPI_rat_freq_information_t *rat_freq
);
void OpenAPI_service_experience_info_free(OpenAPI_service_experience_info_t *service_experience_info);
OpenAPI_service_experience_info_t *OpenAPI_service_experience_info_parseFromJSON(cJSON *service_experience_infoJSON);
cJSON *OpenAPI_service_experience_info_convertToJSON(OpenAPI_service_experience_info_t *service_experience_info);
OpenAPI_service_experience_info_t *OpenAPI_service_experience_info_copy(OpenAPI_service_experience_info_t *dst, OpenAPI_service_experience_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_service_experience_info_H_ */

