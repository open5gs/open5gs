/*
 * time_sync_service_id.h
 *
 * Time Synchronization Service ID
 */

#ifndef _OpenAPI_time_sync_service_id_H_
#define _OpenAPI_time_sync_service_id_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_time_sync_service_id_s OpenAPI_time_sync_service_id_t;
#include "snssai.h"
#include "tai.h"
#include "temporal_validity.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_time_sync_service_id_s {
    char *dnn;
    struct OpenAPI_snssai_s *s_nssai;
    char *reference;
    OpenAPI_list_t *temp_vals;
    OpenAPI_list_t *coverage_area;
    bool is_uu_time_sync_err_bdgt;
    int uu_time_sync_err_bdgt;
};

OpenAPI_time_sync_service_id_t *OpenAPI_time_sync_service_id_create(
    char *dnn,
    OpenAPI_snssai_t *s_nssai,
    char *reference,
    OpenAPI_list_t *temp_vals,
    OpenAPI_list_t *coverage_area,
    bool is_uu_time_sync_err_bdgt,
    int uu_time_sync_err_bdgt
);
void OpenAPI_time_sync_service_id_free(OpenAPI_time_sync_service_id_t *time_sync_service_id);
OpenAPI_time_sync_service_id_t *OpenAPI_time_sync_service_id_parseFromJSON(cJSON *time_sync_service_idJSON);
cJSON *OpenAPI_time_sync_service_id_convertToJSON(OpenAPI_time_sync_service_id_t *time_sync_service_id);
OpenAPI_time_sync_service_id_t *OpenAPI_time_sync_service_id_copy(OpenAPI_time_sync_service_id_t *dst, OpenAPI_time_sync_service_id_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_time_sync_service_id_H_ */

