/*
 * mbs_session_charging_information.h
 *
 * 
 */

#ifndef _OpenAPI_mbs_session_charging_information_H_
#define _OpenAPI_mbs_session_charging_information_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_mbs_session_charging_information_s OpenAPI_mbs_session_charging_information_t;
#include "mbs_service_type.h"
#include "mbs_session_activity_status.h"
#include "mbs_session_id.h"
#include "service_area.h"
#include "serving_network_function_id.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_mbs_session_charging_information_s {
    struct OpenAPI_mbs_session_id_s *m_bs_session_id;
    OpenAPI_mbs_service_type_e m_bs_service_type;
    struct OpenAPI_service_area_s *service_area;
    char *m_bs_start_time;
    char *m_bs_end_time;
    OpenAPI_mbs_session_activity_status_e m_bs_service_activity_status;
    struct OpenAPI_serving_network_function_id_s *serving_network_function_id;
};

OpenAPI_mbs_session_charging_information_t *OpenAPI_mbs_session_charging_information_create(
    OpenAPI_mbs_session_id_t *m_bs_session_id,
    OpenAPI_mbs_service_type_e m_bs_service_type,
    OpenAPI_service_area_t *service_area,
    char *m_bs_start_time,
    char *m_bs_end_time,
    OpenAPI_mbs_session_activity_status_e m_bs_service_activity_status,
    OpenAPI_serving_network_function_id_t *serving_network_function_id
);
void OpenAPI_mbs_session_charging_information_free(OpenAPI_mbs_session_charging_information_t *mbs_session_charging_information);
OpenAPI_mbs_session_charging_information_t *OpenAPI_mbs_session_charging_information_parseFromJSON(cJSON *mbs_session_charging_informationJSON);
cJSON *OpenAPI_mbs_session_charging_information_convertToJSON(OpenAPI_mbs_session_charging_information_t *mbs_session_charging_information);
OpenAPI_mbs_session_charging_information_t *OpenAPI_mbs_session_charging_information_copy(OpenAPI_mbs_session_charging_information_t *dst, OpenAPI_mbs_session_charging_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_mbs_session_charging_information_H_ */

