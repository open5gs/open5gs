/*
 * ncgi.h
 *
 *
 */

#ifndef _OpenAPI_ncgi_H_
#define _OpenAPI_ncgi_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "plmn_id.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ncgi_s OpenAPI_ncgi_t;
typedef struct OpenAPI_ncgi_s {
    struct OpenAPI_plmn_id_s *plmn_id;
    char *nr_cell_id;
    char *nid;
} OpenAPI_ncgi_t;

OpenAPI_ncgi_t *OpenAPI_ncgi_create(
    OpenAPI_plmn_id_t *plmn_id,
    char *nr_cell_id,
    char *nid
    );
void OpenAPI_ncgi_free(OpenAPI_ncgi_t *ncgi);
OpenAPI_ncgi_t *OpenAPI_ncgi_parseFromJSON(cJSON *ncgiJSON);
cJSON *OpenAPI_ncgi_convertToJSON(OpenAPI_ncgi_t *ncgi);
OpenAPI_ncgi_t *OpenAPI_ncgi_copy(OpenAPI_ncgi_t *dst, OpenAPI_ncgi_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ncgi_H_ */

