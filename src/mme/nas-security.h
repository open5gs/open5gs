#ifndef __NAS_SECURITY_H__
#define __NAS_SECURITY_H__

#include "ogs-crypt.h"

#include "mme-context.h"
#include "snow-3g.h"
#include "zuc.h"

#define NAS_SECURITY_BEARER 0
#define NAS_SECURITY_DOWNLINK_DIRECTION 1
#define NAS_SECURITY_UPLINK_DIRECTION 0

#define NAS_SECURITY_MAC_SIZE 4

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct _nas_security_header_type_t {
    union {
        struct {
        ED5(uint8_t integrity_protected:1;,
            uint8_t ciphered:1;,
            uint8_t new_security_context:1;,
            uint8_t service_request:1;,
            uint8_t reserved:4;)
        };
        uint8_t type;
    };
} __attribute__ ((packed)) nas_security_header_type_t;

int nas_security_encode(
        ogs_pkbuf_t **pkbuf, mme_ue_t *mme_ue, nas_message_t *message);
int nas_security_decode(mme_ue_t *mme_ue, 
        nas_security_header_type_t security_header_type, ogs_pkbuf_t *pkbuf);

void nas_mac_calculate(uint8_t algorithm_identity,
        uint8_t *knas_int, uint32_t count, uint8_t bearer, 
        uint8_t direction, ogs_pkbuf_t *pkbuf, uint8_t *mac);

void nas_encrypt(uint8_t algorithm_identity,
        uint8_t *knas_enc, uint32_t count, uint8_t bearer, 
        uint8_t direction, ogs_pkbuf_t *pkbuf);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __NAS_SECURITY_H__ */

