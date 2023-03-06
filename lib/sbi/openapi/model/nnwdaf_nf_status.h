/*
 * nnwdaf_nf_status.h
 *
 * Contains the percentage of time spent on various NF states.
 */

#ifndef _OpenAPI_nnwdaf_nf_status_H_
#define _OpenAPI_nnwdaf_nf_status_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nnwdaf_nf_status_s OpenAPI_nnwdaf_nf_status_t;
typedef struct OpenAPI_nnwdaf_nf_status_s {
    bool is_status_registered;
    int status_registered;
    bool is_status_unregistered;
    int status_unregistered;
    bool is_status_undiscoverable;
    int status_undiscoverable;
} OpenAPI_nnwdaf_nf_status_t;

OpenAPI_nnwdaf_nf_status_t *OpenAPI_nnwdaf_nf_status_create(
    bool is_status_registered,
    int status_registered,
    bool is_status_unregistered,
    int status_unregistered,
    bool is_status_undiscoverable,
    int status_undiscoverable
);
void OpenAPI_nnwdaf_nf_status_free(OpenAPI_nnwdaf_nf_status_t *nnwdaf_nf_status);
OpenAPI_nnwdaf_nf_status_t *OpenAPI_nnwdaf_nf_status_parseFromJSON(cJSON *nnwdaf_nf_statusJSON);
cJSON *OpenAPI_nnwdaf_nf_status_convertToJSON(OpenAPI_nnwdaf_nf_status_t *nnwdaf_nf_status);
OpenAPI_nnwdaf_nf_status_t *OpenAPI_nnwdaf_nf_status_copy(OpenAPI_nnwdaf_nf_status_t *dst, OpenAPI_nnwdaf_nf_status_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nnwdaf_nf_status_H_ */

