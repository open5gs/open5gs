/*
 * tmgi_range.h
 *
 * Range of TMGIs
 */

#ifndef _OpenAPI_tmgi_range_H_
#define _OpenAPI_tmgi_range_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "plmn_id.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_tmgi_range_s OpenAPI_tmgi_range_t;
typedef struct OpenAPI_tmgi_range_s {
    char *mbs_service_id_start;
    char *mbs_service_id_end;
    struct OpenAPI_plmn_id_s *plmn_id;
    char *nid;
} OpenAPI_tmgi_range_t;

OpenAPI_tmgi_range_t *OpenAPI_tmgi_range_create(
    char *mbs_service_id_start,
    char *mbs_service_id_end,
    OpenAPI_plmn_id_t *plmn_id,
    char *nid
);
void OpenAPI_tmgi_range_free(OpenAPI_tmgi_range_t *tmgi_range);
OpenAPI_tmgi_range_t *OpenAPI_tmgi_range_parseFromJSON(cJSON *tmgi_rangeJSON);
cJSON *OpenAPI_tmgi_range_convertToJSON(OpenAPI_tmgi_range_t *tmgi_range);
OpenAPI_tmgi_range_t *OpenAPI_tmgi_range_copy(OpenAPI_tmgi_range_t *dst, OpenAPI_tmgi_range_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_tmgi_range_H_ */

