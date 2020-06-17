/*
 * ims_vo_ps.h
 *
 *
 */

#ifndef _OpenAPI_ims_vo_ps_H_
#define _OpenAPI_ims_vo_ps_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ims_vo_ps_s OpenAPI_ims_vo_ps_t;
typedef struct OpenAPI_ims_vo_ps_s {
} OpenAPI_ims_vo_ps_t;

OpenAPI_ims_vo_ps_t *OpenAPI_ims_vo_ps_create(
    );
void OpenAPI_ims_vo_ps_free(OpenAPI_ims_vo_ps_t *ims_vo_ps);
OpenAPI_ims_vo_ps_t *OpenAPI_ims_vo_ps_parseFromJSON(cJSON *ims_vo_psJSON);
cJSON *OpenAPI_ims_vo_ps_convertToJSON(OpenAPI_ims_vo_ps_t *ims_vo_ps);
OpenAPI_ims_vo_ps_t *OpenAPI_ims_vo_ps_copy(OpenAPI_ims_vo_ps_t *dst, OpenAPI_ims_vo_ps_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ims_vo_ps_H_ */

