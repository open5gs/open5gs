/*
 * suggested_packet_num_dl_1.h
 *
 * 
 */

#ifndef _OpenAPI_suggested_packet_num_dl_1_H_
#define _OpenAPI_suggested_packet_num_dl_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_suggested_packet_num_dl_1_s OpenAPI_suggested_packet_num_dl_1_t;
typedef struct OpenAPI_suggested_packet_num_dl_1_s {
    int suggested_packet_num_dl;
    char *validity_time;
} OpenAPI_suggested_packet_num_dl_1_t;

OpenAPI_suggested_packet_num_dl_1_t *OpenAPI_suggested_packet_num_dl_1_create(
    int suggested_packet_num_dl,
    char *validity_time
);
void OpenAPI_suggested_packet_num_dl_1_free(OpenAPI_suggested_packet_num_dl_1_t *suggested_packet_num_dl_1);
OpenAPI_suggested_packet_num_dl_1_t *OpenAPI_suggested_packet_num_dl_1_parseFromJSON(cJSON *suggested_packet_num_dl_1JSON);
cJSON *OpenAPI_suggested_packet_num_dl_1_convertToJSON(OpenAPI_suggested_packet_num_dl_1_t *suggested_packet_num_dl_1);
OpenAPI_suggested_packet_num_dl_1_t *OpenAPI_suggested_packet_num_dl_1_copy(OpenAPI_suggested_packet_num_dl_1_t *dst, OpenAPI_suggested_packet_num_dl_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_suggested_packet_num_dl_1_H_ */

