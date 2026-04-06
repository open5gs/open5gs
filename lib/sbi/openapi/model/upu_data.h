/*
 * upu_data.h
 *
 * Contains UE parameters update data set (e.g., the updated Routing ID Data or the Default configured NSSAI or the disaster roaming parameters). 
 */

#ifndef _OpenAPI_upu_data_H_
#define _OpenAPI_upu_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_upu_data_s OpenAPI_upu_data_t;
#include "plmn_id.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_upu_data_s {
    char *sec_packet;
    OpenAPI_list_t *default_conf_nssai;
    char *routing_id;
    bool is_drei;
    int drei;
    bool is_drei_eps;
    int drei_eps;
    bool is_aol;
    int aol;
    OpenAPI_list_t *disaster_cond_plmn_ids;
};

OpenAPI_upu_data_t *OpenAPI_upu_data_create(
    char *sec_packet,
    OpenAPI_list_t *default_conf_nssai,
    char *routing_id,
    bool is_drei,
    int drei,
    bool is_drei_eps,
    int drei_eps,
    bool is_aol,
    int aol,
    OpenAPI_list_t *disaster_cond_plmn_ids
);
void OpenAPI_upu_data_free(OpenAPI_upu_data_t *upu_data);
OpenAPI_upu_data_t *OpenAPI_upu_data_parseFromJSON(cJSON *upu_dataJSON);
cJSON *OpenAPI_upu_data_convertToJSON(OpenAPI_upu_data_t *upu_data);
OpenAPI_upu_data_t *OpenAPI_upu_data_copy(OpenAPI_upu_data_t *dst, OpenAPI_upu_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_upu_data_H_ */

