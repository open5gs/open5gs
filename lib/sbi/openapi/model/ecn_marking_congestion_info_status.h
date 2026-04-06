/*
 * ecn_marking_congestion_info_status.h
 *
 * ECN Marking or Congestion Information Status
 */

#ifndef _OpenAPI_ecn_marking_congestion_info_status_H_
#define _OpenAPI_ecn_marking_congestion_info_status_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_ecn_marking_congestion_info_status_s OpenAPI_ecn_marking_congestion_info_status_t;
#include "activation_status.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_ecn_marking_congestion_info_status_s {
    int qfi;
    OpenAPI_activation_status_e activation_status;
};

OpenAPI_ecn_marking_congestion_info_status_t *OpenAPI_ecn_marking_congestion_info_status_create(
    int qfi,
    OpenAPI_activation_status_e activation_status
);
void OpenAPI_ecn_marking_congestion_info_status_free(OpenAPI_ecn_marking_congestion_info_status_t *ecn_marking_congestion_info_status);
OpenAPI_ecn_marking_congestion_info_status_t *OpenAPI_ecn_marking_congestion_info_status_parseFromJSON(cJSON *ecn_marking_congestion_info_statusJSON);
cJSON *OpenAPI_ecn_marking_congestion_info_status_convertToJSON(OpenAPI_ecn_marking_congestion_info_status_t *ecn_marking_congestion_info_status);
OpenAPI_ecn_marking_congestion_info_status_t *OpenAPI_ecn_marking_congestion_info_status_copy(OpenAPI_ecn_marking_congestion_info_status_t *dst, OpenAPI_ecn_marking_congestion_info_status_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ecn_marking_congestion_info_status_H_ */

