/*
 * pc5_flow_bit_rates.h
 *
 * it shall represent the PC5 Flow Bit Rates
 */

#ifndef _OpenAPI_pc5_flow_bit_rates_H_
#define _OpenAPI_pc5_flow_bit_rates_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pc5_flow_bit_rates_s OpenAPI_pc5_flow_bit_rates_t;
typedef struct OpenAPI_pc5_flow_bit_rates_s {
    char *gua_fbr;
    char *max_fbr;
} OpenAPI_pc5_flow_bit_rates_t;

OpenAPI_pc5_flow_bit_rates_t *OpenAPI_pc5_flow_bit_rates_create(
    char *gua_fbr,
    char *max_fbr
);
void OpenAPI_pc5_flow_bit_rates_free(OpenAPI_pc5_flow_bit_rates_t *pc5_flow_bit_rates);
OpenAPI_pc5_flow_bit_rates_t *OpenAPI_pc5_flow_bit_rates_parseFromJSON(cJSON *pc5_flow_bit_ratesJSON);
cJSON *OpenAPI_pc5_flow_bit_rates_convertToJSON(OpenAPI_pc5_flow_bit_rates_t *pc5_flow_bit_rates);
OpenAPI_pc5_flow_bit_rates_t *OpenAPI_pc5_flow_bit_rates_copy(OpenAPI_pc5_flow_bit_rates_t *dst, OpenAPI_pc5_flow_bit_rates_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pc5_flow_bit_rates_H_ */

