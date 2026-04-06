/*
 * af_sfc_requirement.h
 *
 * Describes AF requirements on steering traffic to N6-LAN.
 */

#ifndef _OpenAPI_af_sfc_requirement_H_
#define _OpenAPI_af_sfc_requirement_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_af_sfc_requirement_s OpenAPI_af_sfc_requirement_t;
#include "spatial_validity_rm.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_af_sfc_requirement_s {
    bool is_sfc_id_dl_null;
    char *sfc_id_dl;
    bool is_sfc_id_ul_null;
    char *sfc_id_ul;
    bool is_sp_val_null;
    struct OpenAPI_spatial_validity_rm_s *sp_val;
    bool is_metadata_null;
    char *metadata;
};

OpenAPI_af_sfc_requirement_t *OpenAPI_af_sfc_requirement_create(
    bool is_sfc_id_dl_null,
    char *sfc_id_dl,
    bool is_sfc_id_ul_null,
    char *sfc_id_ul,
    bool is_sp_val_null,
    OpenAPI_spatial_validity_rm_t *sp_val,
    bool is_metadata_null,
    char *metadata
);
void OpenAPI_af_sfc_requirement_free(OpenAPI_af_sfc_requirement_t *af_sfc_requirement);
OpenAPI_af_sfc_requirement_t *OpenAPI_af_sfc_requirement_parseFromJSON(cJSON *af_sfc_requirementJSON);
cJSON *OpenAPI_af_sfc_requirement_convertToJSON(OpenAPI_af_sfc_requirement_t *af_sfc_requirement);
OpenAPI_af_sfc_requirement_t *OpenAPI_af_sfc_requirement_copy(OpenAPI_af_sfc_requirement_t *dst, OpenAPI_af_sfc_requirement_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_af_sfc_requirement_H_ */

