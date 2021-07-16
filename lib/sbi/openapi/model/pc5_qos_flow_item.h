/*
 * pc5_qos_flow_item.h
 *
 * 
 */

#ifndef _OpenAPI_pc5_qos_flow_item_H_
#define _OpenAPI_pc5_qos_flow_item_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "pc5_flow_bit_rates.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pc5_qos_flow_item_s OpenAPI_pc5_qos_flow_item_t;
typedef struct OpenAPI_pc5_qos_flow_item_s {
    int pqi;
    struct OpenAPI_pc5_flow_bit_rates_s *pc5_flow_bit_rates;
    bool is_range;
    int range;
} OpenAPI_pc5_qos_flow_item_t;

OpenAPI_pc5_qos_flow_item_t *OpenAPI_pc5_qos_flow_item_create(
    int pqi,
    OpenAPI_pc5_flow_bit_rates_t *pc5_flow_bit_rates,
    bool is_range,
    int range
);
void OpenAPI_pc5_qos_flow_item_free(OpenAPI_pc5_qos_flow_item_t *pc5_qos_flow_item);
OpenAPI_pc5_qos_flow_item_t *OpenAPI_pc5_qos_flow_item_parseFromJSON(cJSON *pc5_qos_flow_itemJSON);
cJSON *OpenAPI_pc5_qos_flow_item_convertToJSON(OpenAPI_pc5_qos_flow_item_t *pc5_qos_flow_item);
OpenAPI_pc5_qos_flow_item_t *OpenAPI_pc5_qos_flow_item_copy(OpenAPI_pc5_qos_flow_item_t *dst, OpenAPI_pc5_qos_flow_item_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pc5_qos_flow_item_H_ */

