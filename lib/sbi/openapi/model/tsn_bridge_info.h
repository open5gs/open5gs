/*
 * tsn_bridge_info.h
 *
 * 
 */

#ifndef _OpenAPI_tsn_bridge_info_H_
#define _OpenAPI_tsn_bridge_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_tsn_bridge_info_s OpenAPI_tsn_bridge_info_t;
typedef struct OpenAPI_tsn_bridge_info_s {
    bool is_bridge_id;
    int bridge_id;
    char *dstt_addr;
    bool is_dstt_port_num;
    int dstt_port_num;
    bool is_dstt_resid_time;
    int dstt_resid_time;
} OpenAPI_tsn_bridge_info_t;

OpenAPI_tsn_bridge_info_t *OpenAPI_tsn_bridge_info_create(
    bool is_bridge_id,
    int bridge_id,
    char *dstt_addr,
    bool is_dstt_port_num,
    int dstt_port_num,
    bool is_dstt_resid_time,
    int dstt_resid_time
);
void OpenAPI_tsn_bridge_info_free(OpenAPI_tsn_bridge_info_t *tsn_bridge_info);
OpenAPI_tsn_bridge_info_t *OpenAPI_tsn_bridge_info_parseFromJSON(cJSON *tsn_bridge_infoJSON);
cJSON *OpenAPI_tsn_bridge_info_convertToJSON(OpenAPI_tsn_bridge_info_t *tsn_bridge_info);
OpenAPI_tsn_bridge_info_t *OpenAPI_tsn_bridge_info_copy(OpenAPI_tsn_bridge_info_t *dst, OpenAPI_tsn_bridge_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_tsn_bridge_info_H_ */

