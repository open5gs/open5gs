/*
 * gnb_id.h
 *
 * Provides the G-NB identifier.
 */

#ifndef _OpenAPI_gnb_id_H_
#define _OpenAPI_gnb_id_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_gnb_id_s OpenAPI_gnb_id_t;
typedef struct OpenAPI_gnb_id_s {
    int bit_length;
    char *g_nb_value;
} OpenAPI_gnb_id_t;

OpenAPI_gnb_id_t *OpenAPI_gnb_id_create(
    int bit_length,
    char *g_nb_value
);
void OpenAPI_gnb_id_free(OpenAPI_gnb_id_t *gnb_id);
OpenAPI_gnb_id_t *OpenAPI_gnb_id_parseFromJSON(cJSON *gnb_idJSON);
cJSON *OpenAPI_gnb_id_convertToJSON(OpenAPI_gnb_id_t *gnb_id);
OpenAPI_gnb_id_t *OpenAPI_gnb_id_copy(OpenAPI_gnb_id_t *dst, OpenAPI_gnb_id_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_gnb_id_H_ */

