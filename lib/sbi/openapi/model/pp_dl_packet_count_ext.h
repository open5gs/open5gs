/*
 * pp_dl_packet_count_ext.h
 *
 * 
 */

#ifndef _OpenAPI_pp_dl_packet_count_ext_H_
#define _OpenAPI_pp_dl_packet_count_ext_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pp_dl_packet_count_ext_s OpenAPI_pp_dl_packet_count_ext_t;
typedef struct OpenAPI_pp_dl_packet_count_ext_s {
    char *af_instance_id;
    int reference_id;
    char *validity_time;
    char *mtc_provider_information;
} OpenAPI_pp_dl_packet_count_ext_t;

OpenAPI_pp_dl_packet_count_ext_t *OpenAPI_pp_dl_packet_count_ext_create(
    char *af_instance_id,
    int reference_id,
    char *validity_time,
    char *mtc_provider_information
);
void OpenAPI_pp_dl_packet_count_ext_free(OpenAPI_pp_dl_packet_count_ext_t *pp_dl_packet_count_ext);
OpenAPI_pp_dl_packet_count_ext_t *OpenAPI_pp_dl_packet_count_ext_parseFromJSON(cJSON *pp_dl_packet_count_extJSON);
cJSON *OpenAPI_pp_dl_packet_count_ext_convertToJSON(OpenAPI_pp_dl_packet_count_ext_t *pp_dl_packet_count_ext);
OpenAPI_pp_dl_packet_count_ext_t *OpenAPI_pp_dl_packet_count_ext_copy(OpenAPI_pp_dl_packet_count_ext_t *dst, OpenAPI_pp_dl_packet_count_ext_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pp_dl_packet_count_ext_H_ */

