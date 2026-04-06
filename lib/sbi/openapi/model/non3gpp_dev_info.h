/*
 * non3gpp_dev_info.h
 *
 * Represents the Non-3GPP Device Identifier Information.
 */

#ifndef _OpenAPI_non3gpp_dev_info_H_
#define _OpenAPI_non3gpp_dev_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_non3gpp_dev_info_s OpenAPI_non3gpp_dev_info_t;
#include "non3gpp_device_information.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_non3gpp_dev_info_s {
    char *supi;
    OpenAPI_list_t *non3gpp_dev_infos;
    char *supp_feat;
};

OpenAPI_non3gpp_dev_info_t *OpenAPI_non3gpp_dev_info_create(
    char *supi,
    OpenAPI_list_t *non3gpp_dev_infos,
    char *supp_feat
);
void OpenAPI_non3gpp_dev_info_free(OpenAPI_non3gpp_dev_info_t *non3gpp_dev_info);
OpenAPI_non3gpp_dev_info_t *OpenAPI_non3gpp_dev_info_parseFromJSON(cJSON *non3gpp_dev_infoJSON);
cJSON *OpenAPI_non3gpp_dev_info_convertToJSON(OpenAPI_non3gpp_dev_info_t *non3gpp_dev_info);
OpenAPI_non3gpp_dev_info_t *OpenAPI_non3gpp_dev_info_copy(OpenAPI_non3gpp_dev_info_t *dst, OpenAPI_non3gpp_dev_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_non3gpp_dev_info_H_ */

