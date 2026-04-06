/*
 * communication_characteristics_af.h
 *
 * Contains Communication Characteristics per AF
 */

#ifndef _OpenAPI_communication_characteristics_af_H_
#define _OpenAPI_communication_characteristics_af_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_communication_characteristics_af_s OpenAPI_communication_characteristics_af_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_communication_characteristics_af_s {
    bool is_pp_dl_packet_count_null;
    bool is_pp_dl_packet_count;
    int pp_dl_packet_count;
    bool is_maximum_response_time;
    int maximum_response_time;
    bool is_maximum_latency;
    int maximum_latency;
    bool is_eps_applied_ind;
    int eps_applied_ind;
};

OpenAPI_communication_characteristics_af_t *OpenAPI_communication_characteristics_af_create(
    bool is_pp_dl_packet_count_null,
    bool is_pp_dl_packet_count,
    int pp_dl_packet_count,
    bool is_maximum_response_time,
    int maximum_response_time,
    bool is_maximum_latency,
    int maximum_latency,
    bool is_eps_applied_ind,
    int eps_applied_ind
);
void OpenAPI_communication_characteristics_af_free(OpenAPI_communication_characteristics_af_t *communication_characteristics_af);
OpenAPI_communication_characteristics_af_t *OpenAPI_communication_characteristics_af_parseFromJSON(cJSON *communication_characteristics_afJSON);
cJSON *OpenAPI_communication_characteristics_af_convertToJSON(OpenAPI_communication_characteristics_af_t *communication_characteristics_af);
OpenAPI_communication_characteristics_af_t *OpenAPI_communication_characteristics_af_copy(OpenAPI_communication_characteristics_af_t *dst, OpenAPI_communication_characteristics_af_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_communication_characteristics_af_H_ */

