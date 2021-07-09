/*
 * tac_info.h
 *
 * 
 */

#ifndef _OpenAPI_tac_info_H_
#define _OpenAPI_tac_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_tac_info_s OpenAPI_tac_info_t;
typedef struct OpenAPI_tac_info_s {
    OpenAPI_list_t *tac_list;
} OpenAPI_tac_info_t;

OpenAPI_tac_info_t *OpenAPI_tac_info_create(
    OpenAPI_list_t *tac_list
);
void OpenAPI_tac_info_free(OpenAPI_tac_info_t *tac_info);
OpenAPI_tac_info_t *OpenAPI_tac_info_parseFromJSON(cJSON *tac_infoJSON);
cJSON *OpenAPI_tac_info_convertToJSON(OpenAPI_tac_info_t *tac_info);
OpenAPI_tac_info_t *OpenAPI_tac_info_copy(OpenAPI_tac_info_t *dst, OpenAPI_tac_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_tac_info_H_ */

