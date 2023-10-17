/*
 * alternative_qos_profile.h
 *
 * Alternative QoS Profile
 */

#ifndef _OpenAPI_alternative_qos_profile_H_
#define _OpenAPI_alternative_qos_profile_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_alternative_qos_profile_s OpenAPI_alternative_qos_profile_t;
typedef struct OpenAPI_alternative_qos_profile_s {
    int index;
    char *gua_fbr_dl;
    char *gua_fbr_ul;
    bool is_packet_delay_budget;
    int packet_delay_budget;
    char *packet_err_rate;
} OpenAPI_alternative_qos_profile_t;

OpenAPI_alternative_qos_profile_t *OpenAPI_alternative_qos_profile_create(
    int index,
    char *gua_fbr_dl,
    char *gua_fbr_ul,
    bool is_packet_delay_budget,
    int packet_delay_budget,
    char *packet_err_rate
);
void OpenAPI_alternative_qos_profile_free(OpenAPI_alternative_qos_profile_t *alternative_qos_profile);
OpenAPI_alternative_qos_profile_t *OpenAPI_alternative_qos_profile_parseFromJSON(cJSON *alternative_qos_profileJSON);
cJSON *OpenAPI_alternative_qos_profile_convertToJSON(OpenAPI_alternative_qos_profile_t *alternative_qos_profile);
OpenAPI_alternative_qos_profile_t *OpenAPI_alternative_qos_profile_copy(OpenAPI_alternative_qos_profile_t *dst, OpenAPI_alternative_qos_profile_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_alternative_qos_profile_H_ */

