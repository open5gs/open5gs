/*
 * bat_offset_info.h
 *
 * Indicates the offset of the BAT and the optionally adjusted periodicity. 
 */

#ifndef _OpenAPI_bat_offset_info_H_
#define _OpenAPI_bat_offset_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_bat_offset_info_s OpenAPI_bat_offset_info_t;
#include "flows.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_bat_offset_info_s {
    int ran_bat_offset_notif;
    bool is_adj_period;
    int adj_period;
    OpenAPI_list_t *flows;
};

OpenAPI_bat_offset_info_t *OpenAPI_bat_offset_info_create(
    int ran_bat_offset_notif,
    bool is_adj_period,
    int adj_period,
    OpenAPI_list_t *flows
);
void OpenAPI_bat_offset_info_free(OpenAPI_bat_offset_info_t *bat_offset_info);
OpenAPI_bat_offset_info_t *OpenAPI_bat_offset_info_parseFromJSON(cJSON *bat_offset_infoJSON);
cJSON *OpenAPI_bat_offset_info_convertToJSON(OpenAPI_bat_offset_info_t *bat_offset_info);
OpenAPI_bat_offset_info_t *OpenAPI_bat_offset_info_copy(OpenAPI_bat_offset_info_t *dst, OpenAPI_bat_offset_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_bat_offset_info_H_ */

