/*
 * dcsf_info.h
 *
 * Information of a DCSF NF Instance
 */

#ifndef _OpenAPI_dcsf_info_H_
#define _OpenAPI_dcsf_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_dcsf_info_s OpenAPI_dcsf_info_t;
#include "identity_range.h"
#include "imsi_range.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_dcsf_info_s {
    OpenAPI_list_t *ims_domain_name_list;
    OpenAPI_list_t *imsi_ranges;
    OpenAPI_list_t *ims_private_identity_ranges;
    OpenAPI_list_t *ims_public_identity_ranges;
    OpenAPI_list_t *msisdn_ranges;
};

OpenAPI_dcsf_info_t *OpenAPI_dcsf_info_create(
    OpenAPI_list_t *ims_domain_name_list,
    OpenAPI_list_t *imsi_ranges,
    OpenAPI_list_t *ims_private_identity_ranges,
    OpenAPI_list_t *ims_public_identity_ranges,
    OpenAPI_list_t *msisdn_ranges
);
void OpenAPI_dcsf_info_free(OpenAPI_dcsf_info_t *dcsf_info);
OpenAPI_dcsf_info_t *OpenAPI_dcsf_info_parseFromJSON(cJSON *dcsf_infoJSON);
cJSON *OpenAPI_dcsf_info_convertToJSON(OpenAPI_dcsf_info_t *dcsf_info);
OpenAPI_dcsf_info_t *OpenAPI_dcsf_info_copy(OpenAPI_dcsf_info_t *dst, OpenAPI_dcsf_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_dcsf_info_H_ */

