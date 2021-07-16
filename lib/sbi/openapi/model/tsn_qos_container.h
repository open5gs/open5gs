/*
 * tsn_qos_container.h
 *
 * Indicates TSC Traffic QoS.
 */

#ifndef _OpenAPI_tsn_qos_container_H_
#define _OpenAPI_tsn_qos_container_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_tsn_qos_container_s OpenAPI_tsn_qos_container_t;
typedef struct OpenAPI_tsn_qos_container_s {
    bool is_max_tsc_burst_size;
    int max_tsc_burst_size;
    bool is_tsc_pack_delay;
    int tsc_pack_delay;
    bool is_tsc_prio_level;
    int tsc_prio_level;
} OpenAPI_tsn_qos_container_t;

OpenAPI_tsn_qos_container_t *OpenAPI_tsn_qos_container_create(
    bool is_max_tsc_burst_size,
    int max_tsc_burst_size,
    bool is_tsc_pack_delay,
    int tsc_pack_delay,
    bool is_tsc_prio_level,
    int tsc_prio_level
);
void OpenAPI_tsn_qos_container_free(OpenAPI_tsn_qos_container_t *tsn_qos_container);
OpenAPI_tsn_qos_container_t *OpenAPI_tsn_qos_container_parseFromJSON(cJSON *tsn_qos_containerJSON);
cJSON *OpenAPI_tsn_qos_container_convertToJSON(OpenAPI_tsn_qos_container_t *tsn_qos_container);
OpenAPI_tsn_qos_container_t *OpenAPI_tsn_qos_container_copy(OpenAPI_tsn_qos_container_t *dst, OpenAPI_tsn_qos_container_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_tsn_qos_container_H_ */

