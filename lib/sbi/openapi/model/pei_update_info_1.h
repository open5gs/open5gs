/*
 * pei_update_info_1.h
 *
 * This data type contains information about the PEI.
 */

#ifndef _OpenAPI_pei_update_info_1_H_
#define _OpenAPI_pei_update_info_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_pei_update_info_1_s OpenAPI_pei_update_info_1_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_pei_update_info_1_s {
    char *pei;
};

OpenAPI_pei_update_info_1_t *OpenAPI_pei_update_info_1_create(
    char *pei
);
void OpenAPI_pei_update_info_1_free(OpenAPI_pei_update_info_1_t *pei_update_info_1);
OpenAPI_pei_update_info_1_t *OpenAPI_pei_update_info_1_parseFromJSON(cJSON *pei_update_info_1JSON);
cJSON *OpenAPI_pei_update_info_1_convertToJSON(OpenAPI_pei_update_info_1_t *pei_update_info_1);
OpenAPI_pei_update_info_1_t *OpenAPI_pei_update_info_1_copy(OpenAPI_pei_update_info_1_t *dst, OpenAPI_pei_update_info_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pei_update_info_1_H_ */

