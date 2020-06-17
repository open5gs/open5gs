/*
 * suggested_packet_num_dl.h
 *
 *
 */

#ifndef _OpenAPI_suggested_packet_num_dl_H_
#define _OpenAPI_suggested_packet_num_dl_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_suggested_packet_num_dl_s OpenAPI_suggested_packet_num_dl_t;
typedef struct OpenAPI_suggested_packet_num_dl_s {
    int suggested_packet_num_dl;
    char *validity_time;
} OpenAPI_suggested_packet_num_dl_t;

OpenAPI_suggested_packet_num_dl_t *OpenAPI_suggested_packet_num_dl_create(
    int suggested_packet_num_dl,
    char *validity_time
    );
void OpenAPI_suggested_packet_num_dl_free(OpenAPI_suggested_packet_num_dl_t *suggested_packet_num_dl);
OpenAPI_suggested_packet_num_dl_t *OpenAPI_suggested_packet_num_dl_parseFromJSON(cJSON *suggested_packet_num_dlJSON);
cJSON *OpenAPI_suggested_packet_num_dl_convertToJSON(OpenAPI_suggested_packet_num_dl_t *suggested_packet_num_dl);
OpenAPI_suggested_packet_num_dl_t *OpenAPI_suggested_packet_num_dl_copy(OpenAPI_suggested_packet_num_dl_t *dst, OpenAPI_suggested_packet_num_dl_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_suggested_packet_num_dl_H_ */

