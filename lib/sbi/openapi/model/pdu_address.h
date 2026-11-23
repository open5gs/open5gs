/*
 * pdu_address.h
 *
 * 
 */

#ifndef _OpenAPI_pdu_address_H_
#define _OpenAPI_pdu_address_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_pdu_address_s OpenAPI_pdu_address_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_pdu_address_s {
    char *pdu_ipv4_address;
    char *pdu_ipv6_addresswith_prefix;
    bool is_pdu_addressprefixlength;
    int pdu_addressprefixlength;
    bool is_i_pv4dynamic_address_flag;
    int i_pv4dynamic_address_flag;
    bool is_i_pv6dynamic_prefix_flag;
    int i_pv6dynamic_prefix_flag;
    char *add_ipv6_addr_prefixes;
    OpenAPI_list_t *add_ipv6_addr_prefix_list;
};

OpenAPI_pdu_address_t *OpenAPI_pdu_address_create(
    char *pdu_ipv4_address,
    char *pdu_ipv6_addresswith_prefix,
    bool is_pdu_addressprefixlength,
    int pdu_addressprefixlength,
    bool is_i_pv4dynamic_address_flag,
    int i_pv4dynamic_address_flag,
    bool is_i_pv6dynamic_prefix_flag,
    int i_pv6dynamic_prefix_flag,
    char *add_ipv6_addr_prefixes,
    OpenAPI_list_t *add_ipv6_addr_prefix_list
);
void OpenAPI_pdu_address_free(OpenAPI_pdu_address_t *pdu_address);
OpenAPI_pdu_address_t *OpenAPI_pdu_address_parseFromJSON(cJSON *pdu_addressJSON);
cJSON *OpenAPI_pdu_address_convertToJSON(OpenAPI_pdu_address_t *pdu_address);
OpenAPI_pdu_address_t *OpenAPI_pdu_address_copy(OpenAPI_pdu_address_t *dst, OpenAPI_pdu_address_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pdu_address_H_ */

