/*
 * l4s_support_info.h
 *
 * Contains the ECN marking for L4S support in 5GS information.
 */

#ifndef _OpenAPI_l4s_support_info_H_
#define _OpenAPI_l4s_support_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_l4s_support_info_s OpenAPI_l4s_support_info_t;
#include "l4s_notif_type.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_l4s_support_info_s {
    OpenAPI_list_t *ref_pcc_rule_ids;
    OpenAPI_l4s_notif_type_e notif_type;
};

OpenAPI_l4s_support_info_t *OpenAPI_l4s_support_info_create(
    OpenAPI_list_t *ref_pcc_rule_ids,
    OpenAPI_l4s_notif_type_e notif_type
);
void OpenAPI_l4s_support_info_free(OpenAPI_l4s_support_info_t *l4s_support_info);
OpenAPI_l4s_support_info_t *OpenAPI_l4s_support_info_parseFromJSON(cJSON *l4s_support_infoJSON);
cJSON *OpenAPI_l4s_support_info_convertToJSON(OpenAPI_l4s_support_info_t *l4s_support_info);
OpenAPI_l4s_support_info_t *OpenAPI_l4s_support_info_copy(OpenAPI_l4s_support_info_t *dst, OpenAPI_l4s_support_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_l4s_support_info_H_ */

