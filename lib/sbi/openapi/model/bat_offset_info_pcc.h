/*
 * bat_offset_info_pcc.h
 *
 * Indicates the offset of the BAT and the optionally adjusted periodicity. 
 */

#ifndef _OpenAPI_bat_offset_info_pcc_H_
#define _OpenAPI_bat_offset_info_pcc_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_bat_offset_info_pcc_s OpenAPI_bat_offset_info_pcc_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_bat_offset_info_pcc_s {
    int ran_bat_offset_notif;
    bool is_adj_period;
    int adj_period;
    OpenAPI_list_t *ref_pcc_rule_ids;
};

OpenAPI_bat_offset_info_pcc_t *OpenAPI_bat_offset_info_pcc_create(
    int ran_bat_offset_notif,
    bool is_adj_period,
    int adj_period,
    OpenAPI_list_t *ref_pcc_rule_ids
);
void OpenAPI_bat_offset_info_pcc_free(OpenAPI_bat_offset_info_pcc_t *bat_offset_info_pcc);
OpenAPI_bat_offset_info_pcc_t *OpenAPI_bat_offset_info_pcc_parseFromJSON(cJSON *bat_offset_info_pccJSON);
cJSON *OpenAPI_bat_offset_info_pcc_convertToJSON(OpenAPI_bat_offset_info_pcc_t *bat_offset_info_pcc);
OpenAPI_bat_offset_info_pcc_t *OpenAPI_bat_offset_info_pcc_copy(OpenAPI_bat_offset_info_pcc_t *dst, OpenAPI_bat_offset_info_pcc_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_bat_offset_info_pcc_H_ */

