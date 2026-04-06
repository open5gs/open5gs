/*
 * multicast_mbs_group_memb.h
 *
 * Contains Multicast MBS group membership management
 */

#ifndef _OpenAPI_multicast_mbs_group_memb_H_
#define _OpenAPI_multicast_mbs_group_memb_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_multicast_mbs_group_memb_s OpenAPI_multicast_mbs_group_memb_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_multicast_mbs_group_memb_s {
    OpenAPI_list_t *multicast_group_memb;
    char *af_instance_id;
    char *internal_group_identifier;
};

OpenAPI_multicast_mbs_group_memb_t *OpenAPI_multicast_mbs_group_memb_create(
    OpenAPI_list_t *multicast_group_memb,
    char *af_instance_id,
    char *internal_group_identifier
);
void OpenAPI_multicast_mbs_group_memb_free(OpenAPI_multicast_mbs_group_memb_t *multicast_mbs_group_memb);
OpenAPI_multicast_mbs_group_memb_t *OpenAPI_multicast_mbs_group_memb_parseFromJSON(cJSON *multicast_mbs_group_membJSON);
cJSON *OpenAPI_multicast_mbs_group_memb_convertToJSON(OpenAPI_multicast_mbs_group_memb_t *multicast_mbs_group_memb);
OpenAPI_multicast_mbs_group_memb_t *OpenAPI_multicast_mbs_group_memb_copy(OpenAPI_multicast_mbs_group_memb_t *dst, OpenAPI_multicast_mbs_group_memb_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_multicast_mbs_group_memb_H_ */

