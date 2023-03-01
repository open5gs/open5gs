/*
 * app_list_for_ue_comm.h
 *
 * Represents the analytics of the application list used by UE.
 */

#ifndef _OpenAPI_app_list_for_ue_comm_H_
#define _OpenAPI_app_list_for_ue_comm_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "network_area_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_app_list_for_ue_comm_s OpenAPI_app_list_for_ue_comm_t;
typedef struct OpenAPI_app_list_for_ue_comm_s {
    char *app_id;
    char *start_time;
    bool is_app_dur;
    int app_dur;
    bool is_occur_ratio;
    int occur_ratio;
    struct OpenAPI_network_area_info_s *spatial_validity;
} OpenAPI_app_list_for_ue_comm_t;

OpenAPI_app_list_for_ue_comm_t *OpenAPI_app_list_for_ue_comm_create(
    char *app_id,
    char *start_time,
    bool is_app_dur,
    int app_dur,
    bool is_occur_ratio,
    int occur_ratio,
    OpenAPI_network_area_info_t *spatial_validity
);
void OpenAPI_app_list_for_ue_comm_free(OpenAPI_app_list_for_ue_comm_t *app_list_for_ue_comm);
OpenAPI_app_list_for_ue_comm_t *OpenAPI_app_list_for_ue_comm_parseFromJSON(cJSON *app_list_for_ue_commJSON);
cJSON *OpenAPI_app_list_for_ue_comm_convertToJSON(OpenAPI_app_list_for_ue_comm_t *app_list_for_ue_comm);
OpenAPI_app_list_for_ue_comm_t *OpenAPI_app_list_for_ue_comm_copy(OpenAPI_app_list_for_ue_comm_t *dst, OpenAPI_app_list_for_ue_comm_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_app_list_for_ue_comm_H_ */

