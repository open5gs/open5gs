/*
 * acc_net_charging_address.h
 *
 * Describes the network entity within the access network performing charging
 */

#ifndef _OpenAPI_acc_net_charging_address_H_
#define _OpenAPI_acc_net_charging_address_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_acc_net_charging_address_s OpenAPI_acc_net_charging_address_t;
typedef struct OpenAPI_acc_net_charging_address_s {
    char *an_charg_ipv4_addr;
    char *an_charg_ipv6_addr;
} OpenAPI_acc_net_charging_address_t;

OpenAPI_acc_net_charging_address_t *OpenAPI_acc_net_charging_address_create(
    char *an_charg_ipv4_addr,
    char *an_charg_ipv6_addr
);
void OpenAPI_acc_net_charging_address_free(OpenAPI_acc_net_charging_address_t *acc_net_charging_address);
OpenAPI_acc_net_charging_address_t *OpenAPI_acc_net_charging_address_parseFromJSON(cJSON *acc_net_charging_addressJSON);
cJSON *OpenAPI_acc_net_charging_address_convertToJSON(OpenAPI_acc_net_charging_address_t *acc_net_charging_address);
OpenAPI_acc_net_charging_address_t *OpenAPI_acc_net_charging_address_copy(OpenAPI_acc_net_charging_address_t *dst, OpenAPI_acc_net_charging_address_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_acc_net_charging_address_H_ */

