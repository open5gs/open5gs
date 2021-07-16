/*
 * release_data.h
 *
 * 
 */

#ifndef _OpenAPI_release_data_H_
#define _OpenAPI_release_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "cause.h"
#include "n4_information.h"
#include "ng_ap_cause.h"
#include "secondary_rat_usage_info.h"
#include "secondary_rat_usage_report.h"
#include "user_location.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_release_data_s OpenAPI_release_data_t;
typedef struct OpenAPI_release_data_s {
    OpenAPI_cause_e cause;
    struct OpenAPI_ng_ap_cause_s *ng_ap_cause;
    bool is__5g_mm_cause_value;
    int _5g_mm_cause_value;
    struct OpenAPI_user_location_s *ue_location;
    char *ue_time_zone;
    struct OpenAPI_user_location_s *add_ue_location;
    OpenAPI_list_t *secondary_rat_usage_report;
    OpenAPI_list_t *secondary_rat_usage_info;
    struct OpenAPI_n4_information_s *n4_info;
    struct OpenAPI_n4_information_s *n4_info_ext1;
    struct OpenAPI_n4_information_s *n4_info_ext2;
} OpenAPI_release_data_t;

OpenAPI_release_data_t *OpenAPI_release_data_create(
    OpenAPI_cause_e cause,
    OpenAPI_ng_ap_cause_t *ng_ap_cause,
    bool is__5g_mm_cause_value,
    int _5g_mm_cause_value,
    OpenAPI_user_location_t *ue_location,
    char *ue_time_zone,
    OpenAPI_user_location_t *add_ue_location,
    OpenAPI_list_t *secondary_rat_usage_report,
    OpenAPI_list_t *secondary_rat_usage_info,
    OpenAPI_n4_information_t *n4_info,
    OpenAPI_n4_information_t *n4_info_ext1,
    OpenAPI_n4_information_t *n4_info_ext2
);
void OpenAPI_release_data_free(OpenAPI_release_data_t *release_data);
OpenAPI_release_data_t *OpenAPI_release_data_parseFromJSON(cJSON *release_dataJSON);
cJSON *OpenAPI_release_data_convertToJSON(OpenAPI_release_data_t *release_data);
OpenAPI_release_data_t *OpenAPI_release_data_copy(OpenAPI_release_data_t *dst, OpenAPI_release_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_release_data_H_ */

