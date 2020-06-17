/*
 * communication_characteristics.h
 *
 *
 */

#ifndef _OpenAPI_communication_characteristics_H_
#define _OpenAPI_communication_characteristics_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "pp_active_time.h"
#include "pp_dl_packet_count_ext.h"
#include "pp_maximum_latency.h"
#include "pp_maximum_response_time.h"
#include "pp_subs_reg_timer.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_communication_characteristics_s OpenAPI_communication_characteristics_t;
typedef struct OpenAPI_communication_characteristics_s {
    struct OpenAPI_pp_subs_reg_timer_s *pp_subs_reg_timer;
    struct OpenAPI_pp_active_time_s *pp_active_time;
    int pp_dl_packet_count;
    struct OpenAPI_pp_dl_packet_count_ext_s *pp_dl_packet_count_ext;
    struct OpenAPI_pp_maximum_response_time_s *pp_maximum_response_time;
    struct OpenAPI_pp_maximum_latency_s *pp_maximum_latency;
} OpenAPI_communication_characteristics_t;

OpenAPI_communication_characteristics_t *OpenAPI_communication_characteristics_create(
    OpenAPI_pp_subs_reg_timer_t *pp_subs_reg_timer,
    OpenAPI_pp_active_time_t *pp_active_time,
    int pp_dl_packet_count,
    OpenAPI_pp_dl_packet_count_ext_t *pp_dl_packet_count_ext,
    OpenAPI_pp_maximum_response_time_t *pp_maximum_response_time,
    OpenAPI_pp_maximum_latency_t *pp_maximum_latency
    );
void OpenAPI_communication_characteristics_free(OpenAPI_communication_characteristics_t *communication_characteristics);
OpenAPI_communication_characteristics_t *OpenAPI_communication_characteristics_parseFromJSON(cJSON *communication_characteristicsJSON);
cJSON *OpenAPI_communication_characteristics_convertToJSON(OpenAPI_communication_characteristics_t *communication_characteristics);
OpenAPI_communication_characteristics_t *OpenAPI_communication_characteristics_copy(OpenAPI_communication_characteristics_t *dst, OpenAPI_communication_characteristics_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_communication_characteristics_H_ */

