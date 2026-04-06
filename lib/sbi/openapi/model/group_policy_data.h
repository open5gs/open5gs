/*
 * group_policy_data.h
 *
 * Contains the group specific policy control data information.
 */

#ifndef _OpenAPI_group_policy_data_H_
#define _OpenAPI_group_policy_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_group_policy_data_s OpenAPI_group_policy_data_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_group_policy_data_s {
    char *max_group_mbr_ul;
    char *max_group_mbr_dl;
    char *remain_group_mbr_ul;
    char *remain_group_mbr_dl;
    char *supp_feat;
};

OpenAPI_group_policy_data_t *OpenAPI_group_policy_data_create(
    char *max_group_mbr_ul,
    char *max_group_mbr_dl,
    char *remain_group_mbr_ul,
    char *remain_group_mbr_dl,
    char *supp_feat
);
void OpenAPI_group_policy_data_free(OpenAPI_group_policy_data_t *group_policy_data);
OpenAPI_group_policy_data_t *OpenAPI_group_policy_data_parseFromJSON(cJSON *group_policy_dataJSON);
cJSON *OpenAPI_group_policy_data_convertToJSON(OpenAPI_group_policy_data_t *group_policy_data);
OpenAPI_group_policy_data_t *OpenAPI_group_policy_data_copy(OpenAPI_group_policy_data_t *dst, OpenAPI_group_policy_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_group_policy_data_H_ */

