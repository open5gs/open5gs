/*
 * slice_policy_data.h
 *
 * Contains the network slice specific policy control information.
 */

#ifndef _OpenAPI_slice_policy_data_H_
#define _OpenAPI_slice_policy_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_slice_policy_data_s OpenAPI_slice_policy_data_t;
typedef struct OpenAPI_slice_policy_data_s {
    char *mbr_ul;
    char *mbr_dl;
    char *remain_mbr_ul;
    char *remain_mbr_dl;
    char *supp_feat;
    OpenAPI_list_t *reset_ids;
} OpenAPI_slice_policy_data_t;

OpenAPI_slice_policy_data_t *OpenAPI_slice_policy_data_create(
    char *mbr_ul,
    char *mbr_dl,
    char *remain_mbr_ul,
    char *remain_mbr_dl,
    char *supp_feat,
    OpenAPI_list_t *reset_ids
);
void OpenAPI_slice_policy_data_free(OpenAPI_slice_policy_data_t *slice_policy_data);
OpenAPI_slice_policy_data_t *OpenAPI_slice_policy_data_parseFromJSON(cJSON *slice_policy_dataJSON);
cJSON *OpenAPI_slice_policy_data_convertToJSON(OpenAPI_slice_policy_data_t *slice_policy_data);
OpenAPI_slice_policy_data_t *OpenAPI_slice_policy_data_copy(OpenAPI_slice_policy_data_t *dst, OpenAPI_slice_policy_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_slice_policy_data_H_ */

