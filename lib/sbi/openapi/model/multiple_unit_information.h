/*
 * multiple_unit_information.h
 *
 * 
 */

#ifndef _OpenAPI_multiple_unit_information_H_
#define _OpenAPI_multiple_unit_information_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_multiple_unit_information_s OpenAPI_multiple_unit_information_t;
#include "allocated_unit.h"
#include "announcement_information.h"
#include "charging_trigger.h"
#include "final_unit_indication.h"
#include "granted_unit.h"
#include "result_code.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_multiple_unit_information_s {
    OpenAPI_result_code_e result_code;
    int rating_group;
    struct OpenAPI_granted_unit_s *granted_unit;
    struct OpenAPI_allocated_unit_s *allocated_unit;
    OpenAPI_list_t *triggers;
    bool is_validity_time;
    int validity_time;
    bool is_quota_holding_time;
    int quota_holding_time;
    struct OpenAPI_final_unit_indication_s *final_unit_indication;
    bool is_time_quota_threshold;
    int time_quota_threshold;
    bool is_volume_quota_threshold;
    long volume_quota_threshold;
    bool is_unit_quota_threshold;
    int unit_quota_threshold;
    char *u_pfid;
    struct OpenAPI_announcement_information_s *announcement_information;
    char *m_bupfid;
};

OpenAPI_multiple_unit_information_t *OpenAPI_multiple_unit_information_create(
    OpenAPI_result_code_e result_code,
    int rating_group,
    OpenAPI_granted_unit_t *granted_unit,
    OpenAPI_allocated_unit_t *allocated_unit,
    OpenAPI_list_t *triggers,
    bool is_validity_time,
    int validity_time,
    bool is_quota_holding_time,
    int quota_holding_time,
    OpenAPI_final_unit_indication_t *final_unit_indication,
    bool is_time_quota_threshold,
    int time_quota_threshold,
    bool is_volume_quota_threshold,
    long volume_quota_threshold,
    bool is_unit_quota_threshold,
    int unit_quota_threshold,
    char *u_pfid,
    OpenAPI_announcement_information_t *announcement_information,
    char *m_bupfid
);
void OpenAPI_multiple_unit_information_free(OpenAPI_multiple_unit_information_t *multiple_unit_information);
OpenAPI_multiple_unit_information_t *OpenAPI_multiple_unit_information_parseFromJSON(cJSON *multiple_unit_informationJSON);
cJSON *OpenAPI_multiple_unit_information_convertToJSON(OpenAPI_multiple_unit_information_t *multiple_unit_information);
OpenAPI_multiple_unit_information_t *OpenAPI_multiple_unit_information_copy(OpenAPI_multiple_unit_information_t *dst, OpenAPI_multiple_unit_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_multiple_unit_information_H_ */

