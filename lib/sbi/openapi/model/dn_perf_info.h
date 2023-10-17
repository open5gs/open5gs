/*
 * dn_perf_info.h
 *
 * Represents DN performance information.
 */

#ifndef _OpenAPI_dn_perf_info_H_
#define _OpenAPI_dn_perf_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "dn_perf.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_dn_perf_info_s OpenAPI_dn_perf_info_t;
typedef struct OpenAPI_dn_perf_info_s {
    char *app_id;
    char *dnn;
    struct OpenAPI_snssai_s *snssai;
    OpenAPI_list_t *dn_perf;
    bool is_confidence;
    int confidence;
} OpenAPI_dn_perf_info_t;

OpenAPI_dn_perf_info_t *OpenAPI_dn_perf_info_create(
    char *app_id,
    char *dnn,
    OpenAPI_snssai_t *snssai,
    OpenAPI_list_t *dn_perf,
    bool is_confidence,
    int confidence
);
void OpenAPI_dn_perf_info_free(OpenAPI_dn_perf_info_t *dn_perf_info);
OpenAPI_dn_perf_info_t *OpenAPI_dn_perf_info_parseFromJSON(cJSON *dn_perf_infoJSON);
cJSON *OpenAPI_dn_perf_info_convertToJSON(OpenAPI_dn_perf_info_t *dn_perf_info);
OpenAPI_dn_perf_info_t *OpenAPI_dn_perf_info_copy(OpenAPI_dn_perf_info_t *dst, OpenAPI_dn_perf_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_dn_perf_info_H_ */

