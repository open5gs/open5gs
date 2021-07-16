/*
 * exemption_ind.h
 *
 * 
 */

#ifndef _OpenAPI_exemption_ind_H_
#define _OpenAPI_exemption_ind_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_exemption_ind_s OpenAPI_exemption_ind_t;
typedef struct OpenAPI_exemption_ind_s {
    bool is_dnn_congestion;
    int dnn_congestion;
    bool is_snssai_only_congestion;
    int snssai_only_congestion;
    bool is_snssai_dnn_congestion;
    int snssai_dnn_congestion;
} OpenAPI_exemption_ind_t;

OpenAPI_exemption_ind_t *OpenAPI_exemption_ind_create(
    bool is_dnn_congestion,
    int dnn_congestion,
    bool is_snssai_only_congestion,
    int snssai_only_congestion,
    bool is_snssai_dnn_congestion,
    int snssai_dnn_congestion
);
void OpenAPI_exemption_ind_free(OpenAPI_exemption_ind_t *exemption_ind);
OpenAPI_exemption_ind_t *OpenAPI_exemption_ind_parseFromJSON(cJSON *exemption_indJSON);
cJSON *OpenAPI_exemption_ind_convertToJSON(OpenAPI_exemption_ind_t *exemption_ind);
OpenAPI_exemption_ind_t *OpenAPI_exemption_ind_copy(OpenAPI_exemption_ind_t *dst, OpenAPI_exemption_ind_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_exemption_ind_H_ */

