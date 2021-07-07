/*
 * tsn_qos_container_rm.h
 *
 * Indicates removable TSC Traffic QoS.
 */

#ifndef _OpenAPI_tsn_qos_container_rm_H_
#define _OpenAPI_tsn_qos_container_rm_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_tsn_qos_container_rm_s OpenAPI_tsn_qos_container_rm_t;
typedef struct OpenAPI_tsn_qos_container_rm_s {
    int max_tsc_burst_size;
    int tsc_pack_delay;
    int tsc_prio_level;
} OpenAPI_tsn_qos_container_rm_t;

OpenAPI_tsn_qos_container_rm_t *OpenAPI_tsn_qos_container_rm_create(
    int max_tsc_burst_size,
    int tsc_pack_delay,
    int tsc_prio_level
);
void OpenAPI_tsn_qos_container_rm_free(OpenAPI_tsn_qos_container_rm_t *tsn_qos_container_rm);
OpenAPI_tsn_qos_container_rm_t *OpenAPI_tsn_qos_container_rm_parseFromJSON(cJSON *tsn_qos_container_rmJSON);
cJSON *OpenAPI_tsn_qos_container_rm_convertToJSON(OpenAPI_tsn_qos_container_rm_t *tsn_qos_container_rm);
OpenAPI_tsn_qos_container_rm_t *OpenAPI_tsn_qos_container_rm_copy(OpenAPI_tsn_qos_container_rm_t *dst, OpenAPI_tsn_qos_container_rm_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_tsn_qos_container_rm_H_ */

