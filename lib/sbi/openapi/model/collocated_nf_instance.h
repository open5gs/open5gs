/*
 * collocated_nf_instance.h
 *
 * Information of an collocated NF Instance registered in the NRF
 */

#ifndef _OpenAPI_collocated_nf_instance_H_
#define _OpenAPI_collocated_nf_instance_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "collocated_nf_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_collocated_nf_instance_s OpenAPI_collocated_nf_instance_t;
typedef struct OpenAPI_collocated_nf_instance_s {
    char *nf_instance_id;
    OpenAPI_collocated_nf_type_e nf_type;
} OpenAPI_collocated_nf_instance_t;

OpenAPI_collocated_nf_instance_t *OpenAPI_collocated_nf_instance_create(
    char *nf_instance_id,
    OpenAPI_collocated_nf_type_e nf_type
);
void OpenAPI_collocated_nf_instance_free(OpenAPI_collocated_nf_instance_t *collocated_nf_instance);
OpenAPI_collocated_nf_instance_t *OpenAPI_collocated_nf_instance_parseFromJSON(cJSON *collocated_nf_instanceJSON);
cJSON *OpenAPI_collocated_nf_instance_convertToJSON(OpenAPI_collocated_nf_instance_t *collocated_nf_instance);
OpenAPI_collocated_nf_instance_t *OpenAPI_collocated_nf_instance_copy(OpenAPI_collocated_nf_instance_t *dst, OpenAPI_collocated_nf_instance_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_collocated_nf_instance_H_ */

