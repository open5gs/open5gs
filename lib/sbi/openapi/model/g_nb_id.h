/*
 * g_nb_id.h
 *
 *
 */

#ifndef _OpenAPI_g_nb_id_H_
#define _OpenAPI_g_nb_id_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_g_nb_id_s OpenAPI_g_nb_id_t;
typedef struct OpenAPI_g_nb_id_s {
    int bit_length;
    char *g_nb_value;
} OpenAPI_g_nb_id_t;

OpenAPI_g_nb_id_t *OpenAPI_g_nb_id_create(
    int bit_length,
    char *g_nb_value
    );
void OpenAPI_g_nb_id_free(OpenAPI_g_nb_id_t *g_nb_id);
OpenAPI_g_nb_id_t *OpenAPI_g_nb_id_parseFromJSON(cJSON *g_nb_idJSON);
cJSON *OpenAPI_g_nb_id_convertToJSON(OpenAPI_g_nb_id_t *g_nb_id);
OpenAPI_g_nb_id_t *OpenAPI_g_nb_id_copy(OpenAPI_g_nb_id_t *dst, OpenAPI_g_nb_id_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_g_nb_id_H_ */

