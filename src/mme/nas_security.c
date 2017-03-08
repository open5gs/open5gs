#define TRACE_MODULE _nas_sec

#include "core_debug.h"

#include "context.h"
#include "nas_message.h"
#include "nas_security.h"

status_t nas_security_encode(pkbuf_t **pkbuf, 
        ue_ctx_t *ue, nas_message_t *message)
{
    d_assert(ue, return CORE_ERROR, "Null param");
    d_assert(message, return CORE_ERROR, "Null param");

    switch(message->h.security_header_type)
    {
        case NAS_SECURITY_HEADER_PLAIN_NAS_MESSAGE:
        {
            return nas_encode_pdu(pkbuf, message);
        }
        case NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_NEW_SECURITY_CONTEXT:
        {
            ue->dl_count = 0;
            ue->ul_count = 0;
        }
        case NAS_SECURITY_HEADER_INTEGRITY_PROTECTED:
        {
            nas_security_header_t h;
            c_uint8_t mac[4];
            pkbuf_t *new = NULL;

            memset(&h, 0, sizeof(nas_security_header_t));
            h.security_header_type = message->h.security_header_type;
            h.protocol_discriminator = message->h.protocol_discriminator;
            h.sequence_number = (ue->dl_count & 0xff);

            d_assert(nas_encode_pdu(&new, message) == CORE_OK, 
                    return CORE_ERROR,
                    "NAS encoding error");

            d_assert(CORE_OK == pkbuf_header(new, sizeof(h.sequence_number)),
                pkbuf_free(new);return CORE_ERROR, 
                "pkbuf_header error");
            *(c_uint8_t *)(new->payload) = h.sequence_number;

            nas_mac_calculate(mme_self()->selected_int_algorithm,
                ue->knas_int, ue->dl_count, NAS_SECURITY_BEARER, 
                NAS_SECURITY_DOWNLINK_DIRECTION, new, mac);

            d_assert(CORE_OK == pkbuf_header(new, 
                    sizeof(nas_security_header_t) - sizeof(h.sequence_number)),
                pkbuf_free(new);return CORE_ERROR,
                "pkbuf_header error");
            memcpy(new->payload, &h, sizeof(nas_security_header_t));
            memcpy(new->payload+1, mac, sizeof(mac));

            ue->dl_count = (ue->dl_count + 1) & 0xffffff; /* Use 24bit */

            *pkbuf = new;
            break;
        }
        default:
        {
            d_warn("Not implemented(securiry header type:0x%x)", 
                    message->h.security_header_type);
            return CORE_ERROR;
        }
    }

    return CORE_OK;
}

void nas_mac_calculate(c_uint8_t algorithm_identity,
        c_uint8_t *knas_int, c_uint32_t count, c_uint8_t bearer, 
        c_uint8_t direction, pkbuf_t *pkbuf, c_uint8_t *mac)
{
    d_assert(knas_int, return, "Null param");
    d_assert(bearer >= 0 && bearer <= 0x1f, return, "Invalid param");
    d_assert(direction == 0 || direction == 1, return, "Invalid param");
    d_assert(pkbuf, return, "Null param");
    d_assert(pkbuf->payload, return, "Null param");
    d_assert(pkbuf->len, return, "Null param");
    d_assert(mac, return, "Null param");

    switch(algorithm_identity)
    {
        case NAS_SECURITY_ALGORITHMS_128_EIA1:
        {
            snow_3g_f9(knas_int, count, (bearer << 27), direction, 
                    pkbuf->payload, (pkbuf->len << 3), mac);
            break;
        }
        case NAS_SECURITY_ALGORITHMS_128_EIA2:
        {
            count = htonl(count);
            c_uint8_t *ivec = NULL;;
            c_uint8_t cmac[16];

            pkbuf_header(pkbuf, 8);

            ivec = pkbuf->payload;
            memset(ivec, 0, 8);
            memcpy(ivec + 0, &count, sizeof(count));
            ivec[4] = (bearer << 3) | (direction << 2);

            aes_cmac_calculate(cmac, knas_int, pkbuf->payload, pkbuf->len);
            memcpy(mac, cmac, 4);

            pkbuf_header(pkbuf, -8);

            break;
        }
        case NAS_SECURITY_ALGORITHMS_128_EIA3:
        {
            c_uint32_t mac32;

            zuc_eia3(knas_int, count, bearer, direction, 
                    (pkbuf->len << 3), pkbuf->payload, &mac32);
            mac32 = ntohl(mac32);
            memcpy(mac, &mac32, sizeof(c_uint32_t));
            break;
        }
        case NAS_SECURITY_ALGORITHMS_EIA0:
        {
            d_error("Invalid identity : NAS_SECURITY_ALGORITHMS_EIA0");
            break;
        }
        default:
        {
            d_assert(0, return, "Unknown algorithm identity(%d)", 
                    algorithm_identity);
            break;
        }
    }
}

void nas_encrypt(c_uint8_t algorithm_identity,
        c_uint8_t *knas_enc, c_uint32_t count, c_uint8_t bearer, 
        c_uint8_t direction, pkbuf_t *pkbuf)
{
    d_assert(knas_enc, return, "Null param");
    d_assert(bearer >= 0 && bearer <= 0x1f, return, "Invalid param");
    d_assert(direction == 0 || direction == 1, return, "Invalid param");
    d_assert(pkbuf, return, "Null param");
    d_assert(pkbuf->payload, return, "Null param");
    d_assert(pkbuf->len, return, "Null param");

    switch(algorithm_identity)
    {
        case NAS_SECURITY_ALGORITHMS_128_EEA1:
        {
            snow_3g_f8(knas_enc, count, bearer, direction, 
                    pkbuf->payload, (pkbuf->len << 3));
            break;
        }
        case NAS_SECURITY_ALGORITHMS_128_EEA2:
        {
            count = htonl(count);
            c_uint8_t ivec[16];

            memset(ivec, 0, 16);
            memcpy(ivec + 0, &count, sizeof(count));
            ivec[4] = (bearer << 3) | (direction << 2);
            aes_ctr128_encrypt(knas_enc, ivec, 
                    pkbuf->payload, pkbuf->len, pkbuf->payload);
            break;
        }
        case NAS_SECURITY_ALGORITHMS_128_EEA3:
        {
            zuc_eea3(knas_enc, count, bearer, direction, 
                    (pkbuf->len << 3), pkbuf->payload, pkbuf->payload);
            break;
        }
        case NAS_SECURITY_ALGORITHMS_EEA0:
        {
            d_error("Invalid identity : NAS_SECURITY_ALGORITHMS_EEA0");
            break;
        }
        default:
        {
            d_assert(0, return, "Unknown algorithm identity(%d)", 
                    algorithm_identity);
            break;
        }
    }
}
