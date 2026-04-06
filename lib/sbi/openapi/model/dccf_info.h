/*
 * dccf_info.h
 *
 * Information of a DCCF NF Instance
 */

#ifndef _OpenAPI_dccf_info_H_
#define _OpenAPI_dccf_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_dccf_info_s OpenAPI_dccf_info_t;
#include "nf_type.h"
#include "tai.h"
#include "tai_range.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_dccf_info_s {
    OpenAPI_list_t *serving_nf_type_list;
    OpenAPI_list_t *serving_nf_set_id_list;
    OpenAPI_list_t *tai_list;
    OpenAPI_list_t *tai_range_list;
    bool is_data_subs_reloc_ind;
    int data_subs_reloc_ind;
};

OpenAPI_dccf_info_t *OpenAPI_dccf_info_create(
    OpenAPI_list_t *serving_nf_type_list,
    OpenAPI_list_t *serving_nf_set_id_list,
    OpenAPI_list_t *tai_list,
    OpenAPI_list_t *tai_range_list,
    bool is_data_subs_reloc_ind,
    int data_subs_reloc_ind
);
void OpenAPI_dccf_info_free(OpenAPI_dccf_info_t *dccf_info);
OpenAPI_dccf_info_t *OpenAPI_dccf_info_parseFromJSON(cJSON *dccf_infoJSON);
cJSON *OpenAPI_dccf_info_convertToJSON(OpenAPI_dccf_info_t *dccf_info);
OpenAPI_dccf_info_t *OpenAPI_dccf_info_copy(OpenAPI_dccf_info_t *dst, OpenAPI_dccf_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_dccf_info_H_ */

