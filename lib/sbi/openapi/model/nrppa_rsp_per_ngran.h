/*
 * nrppa_rsp_per_ngran.h
 *
 * NRPPA Information related to failed NG-RAN
 */

#ifndef _OpenAPI_nrppa_rsp_per_ngran_H_
#define _OpenAPI_nrppa_rsp_per_ngran_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_nrppa_rsp_per_ngran_s OpenAPI_nrppa_rsp_per_ngran_t;
#include "global_ran_node_id.h"
#include "nrppa_trans_failure.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_nrppa_rsp_per_ngran_s {
    struct OpenAPI_global_ran_node_id_s *ngran_id;
    OpenAPI_nrppa_trans_failure_e cause;
};

OpenAPI_nrppa_rsp_per_ngran_t *OpenAPI_nrppa_rsp_per_ngran_create(
    OpenAPI_global_ran_node_id_t *ngran_id,
    OpenAPI_nrppa_trans_failure_e cause
);
void OpenAPI_nrppa_rsp_per_ngran_free(OpenAPI_nrppa_rsp_per_ngran_t *nrppa_rsp_per_ngran);
OpenAPI_nrppa_rsp_per_ngran_t *OpenAPI_nrppa_rsp_per_ngran_parseFromJSON(cJSON *nrppa_rsp_per_ngranJSON);
cJSON *OpenAPI_nrppa_rsp_per_ngran_convertToJSON(OpenAPI_nrppa_rsp_per_ngran_t *nrppa_rsp_per_ngran);
OpenAPI_nrppa_rsp_per_ngran_t *OpenAPI_nrppa_rsp_per_ngran_copy(OpenAPI_nrppa_rsp_per_ngran_t *dst, OpenAPI_nrppa_rsp_per_ngran_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nrppa_rsp_per_ngran_H_ */

