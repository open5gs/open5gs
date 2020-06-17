/*
 * av_ims_gba_eap_aka.h
 *
 *
 */

#ifndef _OpenAPI_av_ims_gba_eap_aka_H_
#define _OpenAPI_av_ims_gba_eap_aka_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "hss_av_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_av_ims_gba_eap_aka_s OpenAPI_av_ims_gba_eap_aka_t;
typedef struct OpenAPI_av_ims_gba_eap_aka_s {
    struct OpenAPI_hss_av_type_s *av_type;
    char *rand;
    char *xres;
    char *autn;
    char *ck;
    char *ik;
} OpenAPI_av_ims_gba_eap_aka_t;

OpenAPI_av_ims_gba_eap_aka_t *OpenAPI_av_ims_gba_eap_aka_create(
    OpenAPI_hss_av_type_t *av_type,
    char *rand,
    char *xres,
    char *autn,
    char *ck,
    char *ik
    );
void OpenAPI_av_ims_gba_eap_aka_free(OpenAPI_av_ims_gba_eap_aka_t *av_ims_gba_eap_aka);
OpenAPI_av_ims_gba_eap_aka_t *OpenAPI_av_ims_gba_eap_aka_parseFromJSON(cJSON *av_ims_gba_eap_akaJSON);
cJSON *OpenAPI_av_ims_gba_eap_aka_convertToJSON(OpenAPI_av_ims_gba_eap_aka_t *av_ims_gba_eap_aka);
OpenAPI_av_ims_gba_eap_aka_t *OpenAPI_av_ims_gba_eap_aka_copy(OpenAPI_av_ims_gba_eap_aka_t *dst, OpenAPI_av_ims_gba_eap_aka_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_av_ims_gba_eap_aka_H_ */

