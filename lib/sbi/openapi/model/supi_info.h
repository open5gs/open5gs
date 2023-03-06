/*
 * supi_info.h
 *
 * List of Supis.
 */

#ifndef _OpenAPI_supi_info_H_
#define _OpenAPI_supi_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_supi_info_s OpenAPI_supi_info_t;
typedef struct OpenAPI_supi_info_s {
    OpenAPI_list_t *supi_list;
} OpenAPI_supi_info_t;

OpenAPI_supi_info_t *OpenAPI_supi_info_create(
    OpenAPI_list_t *supi_list
);
void OpenAPI_supi_info_free(OpenAPI_supi_info_t *supi_info);
OpenAPI_supi_info_t *OpenAPI_supi_info_parseFromJSON(cJSON *supi_infoJSON);
cJSON *OpenAPI_supi_info_convertToJSON(OpenAPI_supi_info_t *supi_info);
OpenAPI_supi_info_t *OpenAPI_supi_info_copy(OpenAPI_supi_info_t *dst, OpenAPI_supi_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_supi_info_H_ */

