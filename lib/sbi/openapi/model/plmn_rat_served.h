/*
 * plmn_rat_served.h
 *
 *
 */

#ifndef _OpenAPI_plmn_rat_served_H_
#define _OpenAPI_plmn_rat_served_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "plmn_id.h"
#include "v2x_rat_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_plmn_rat_served_s OpenAPI_plmn_rat_served_t;
typedef struct OpenAPI_plmn_rat_served_s {
    struct OpenAPI_plmn_id_s *plmn;
    OpenAPI_list_t *rats;
} OpenAPI_plmn_rat_served_t;

OpenAPI_plmn_rat_served_t *OpenAPI_plmn_rat_served_create(
    OpenAPI_plmn_id_t *plmn,
    OpenAPI_list_t *rats
    );
void OpenAPI_plmn_rat_served_free(OpenAPI_plmn_rat_served_t *plmn_rat_served);
OpenAPI_plmn_rat_served_t *OpenAPI_plmn_rat_served_parseFromJSON(cJSON *plmn_rat_servedJSON);
cJSON *OpenAPI_plmn_rat_served_convertToJSON(OpenAPI_plmn_rat_served_t *plmn_rat_served);
OpenAPI_plmn_rat_served_t *OpenAPI_plmn_rat_served_copy(OpenAPI_plmn_rat_served_t *dst, OpenAPI_plmn_rat_served_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_plmn_rat_served_H_ */

