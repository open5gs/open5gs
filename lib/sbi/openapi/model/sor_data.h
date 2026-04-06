/*
 * sor_data.h
 *
 * Used to store the status of the latest SOR data update.
 */

#ifndef _OpenAPI_sor_data_H_
#define _OpenAPI_sor_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_sor_data_s OpenAPI_sor_data_t;
#include "ue_update_status.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_sor_data_s {
    char *provisioning_time;
    OpenAPI_ue_update_status_e ue_update_status;
    char *sor_xmac_iue;
    char *sor_mac_iue;
    bool is_me_support_of_sor_cmci;
    int me_support_of_sor_cmci;
    bool is_me_support_of_sor_snpn_si;
    int me_support_of_sor_snpn_si;
    bool is_me_support_of_sor_snpn_si_ls;
    int me_support_of_sor_snpn_si_ls;
};

OpenAPI_sor_data_t *OpenAPI_sor_data_create(
    char *provisioning_time,
    OpenAPI_ue_update_status_e ue_update_status,
    char *sor_xmac_iue,
    char *sor_mac_iue,
    bool is_me_support_of_sor_cmci,
    int me_support_of_sor_cmci,
    bool is_me_support_of_sor_snpn_si,
    int me_support_of_sor_snpn_si,
    bool is_me_support_of_sor_snpn_si_ls,
    int me_support_of_sor_snpn_si_ls
);
void OpenAPI_sor_data_free(OpenAPI_sor_data_t *sor_data);
OpenAPI_sor_data_t *OpenAPI_sor_data_parseFromJSON(cJSON *sor_dataJSON);
cJSON *OpenAPI_sor_data_convertToJSON(OpenAPI_sor_data_t *sor_data);
OpenAPI_sor_data_t *OpenAPI_sor_data_copy(OpenAPI_sor_data_t *dst, OpenAPI_sor_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sor_data_H_ */

