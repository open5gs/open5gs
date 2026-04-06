/*
 * tss_rsp_per_ngran.h
 *
 * Represents a TSS related N2 information data part
 */

#ifndef _OpenAPI_tss_rsp_per_ngran_H_
#define _OpenAPI_tss_rsp_per_ngran_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_tss_rsp_per_ngran_s OpenAPI_tss_rsp_per_ngran_t;
#include "global_ran_node_id.h"
#include "n2_info_content.h"
#include "ngran_failure_info.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_tss_rsp_per_ngran_s {
    struct OpenAPI_global_ran_node_id_s *ngran_id;
    OpenAPI_ngran_failure_info_e ngran_failure_info;
    struct OpenAPI_n2_info_content_s *tss_container;
};

OpenAPI_tss_rsp_per_ngran_t *OpenAPI_tss_rsp_per_ngran_create(
    OpenAPI_global_ran_node_id_t *ngran_id,
    OpenAPI_ngran_failure_info_e ngran_failure_info,
    OpenAPI_n2_info_content_t *tss_container
);
void OpenAPI_tss_rsp_per_ngran_free(OpenAPI_tss_rsp_per_ngran_t *tss_rsp_per_ngran);
OpenAPI_tss_rsp_per_ngran_t *OpenAPI_tss_rsp_per_ngran_parseFromJSON(cJSON *tss_rsp_per_ngranJSON);
cJSON *OpenAPI_tss_rsp_per_ngran_convertToJSON(OpenAPI_tss_rsp_per_ngran_t *tss_rsp_per_ngran);
OpenAPI_tss_rsp_per_ngran_t *OpenAPI_tss_rsp_per_ngran_copy(OpenAPI_tss_rsp_per_ngran_t *dst, OpenAPI_tss_rsp_per_ngran_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_tss_rsp_per_ngran_H_ */

