/*
 * tsn_charging_information.h
 *
 * 
 */

#ifndef _OpenAPI_tsn_charging_information_H_
#define _OpenAPI_tsn_charging_information_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_tsn_charging_information_s OpenAPI_tsn_charging_information_t;
#include "charging_tsc_assistance_information.h"
#include "model_5_gs_bridge_information.h"
#include "snssai.h"
#include "time_synchronization_information.h"
#include "tsnqo_s_information.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_tsn_charging_information_s {
    char *d_nn;
    struct OpenAPI_snssai_s *s_nssai;
    char *internal_group_identifier;
    OpenAPI_list_t *external_individual_id_list;
    struct OpenAPI_model_5_gs_bridge_information_s *_5_gs_bridge_information;
    struct OpenAPI_tsnqo_s_information_s *t_snqo_s_information;
    struct OpenAPI_charging_tsc_assistance_information_s *t_sc_assistance_information;
    struct OpenAPI_time_synchronization_information_s *time_synchronization_information;
};

OpenAPI_tsn_charging_information_t *OpenAPI_tsn_charging_information_create(
    char *d_nn,
    OpenAPI_snssai_t *s_nssai,
    char *internal_group_identifier,
    OpenAPI_list_t *external_individual_id_list,
    OpenAPI_model_5_gs_bridge_information_t *_5_gs_bridge_information,
    OpenAPI_tsnqo_s_information_t *t_snqo_s_information,
    OpenAPI_charging_tsc_assistance_information_t *t_sc_assistance_information,
    OpenAPI_time_synchronization_information_t *time_synchronization_information
);
void OpenAPI_tsn_charging_information_free(OpenAPI_tsn_charging_information_t *tsn_charging_information);
OpenAPI_tsn_charging_information_t *OpenAPI_tsn_charging_information_parseFromJSON(cJSON *tsn_charging_informationJSON);
cJSON *OpenAPI_tsn_charging_information_convertToJSON(OpenAPI_tsn_charging_information_t *tsn_charging_information);
OpenAPI_tsn_charging_information_t *OpenAPI_tsn_charging_information_copy(OpenAPI_tsn_charging_information_t *dst, OpenAPI_tsn_charging_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_tsn_charging_information_H_ */

