#define TRACE_MODULE _nas_sec

#include "core_debug.h"

#include "nas/nas_message.h"
#include "nas_security.h"

status_t nas_security_encode(
        pkbuf_t **pkbuf, mme_ue_t *mme_ue, nas_message_t *message)
{
    int integrity_protected = 0;
    int new_security_context = 0;
    int ciphered = 0;

    d_assert(mme_ue, return CORE_ERROR, "Null param");
    d_assert(message, return CORE_ERROR, "Null param");

    switch(message->h.security_header_type)
    {
        case NAS_SECURITY_HEADER_PLAIN_NAS_MESSAGE:
            return nas_plain_encode(pkbuf, message);
        case NAS_SECURITY_HEADER_INTEGRITY_PROTECTED:
            integrity_protected = 1;
            break;
        case NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED:
            integrity_protected = 1;
            ciphered = 1;
            break;
        case NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_NEW_SECURITY_CONTEXT:
            integrity_protected = 1;
            new_security_context = 1;
            break;
        case NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHTERD_WITH_NEW_INTEGRITY_CONTEXT:
            integrity_protected = 1;
            new_security_context = 1;
            ciphered = 1;
            break;
        default:
            d_warn("Not implemented(securiry header type:0x%x)", 
                    message->h.security_header_type);
            return CORE_ERROR;
    }

    if (new_security_context)
    {
        mme_ue->dl_count = 0;
        mme_ue->ul_count.i32 = 0;
    }

    if (mme_ue->selected_enc_algorithm == 0)
        ciphered = 0;
    if (mme_ue->selected_int_algorithm == 0)
        integrity_protected = 0;

    if (ciphered || integrity_protected)
    {
        nas_security_header_t h;
        pkbuf_t *new = NULL;

        memset(&h, 0, sizeof(h));
        h.security_header_type = message->h.security_header_type;
        h.protocol_discriminator = message->h.protocol_discriminator;
        h.sequence_number = (mme_ue->dl_count & 0xff);

        d_assert(nas_plain_encode(&new, message) == CORE_OK, 
                return CORE_ERROR, "NAS encoding error");

        if (ciphered)
        {
            /* encrypt NAS message */
            nas_encrypt(mme_ue->selected_enc_algorithm,
                mme_ue->knas_enc, mme_ue->dl_count, NAS_SECURITY_BEARER,
                NAS_SECURITY_DOWNLINK_DIRECTION, new);
        }

        /* encode sequence number */
        d_assert(CORE_OK == pkbuf_header(new, 1),
            pkbuf_free(new);return CORE_ERROR, "pkbuf_header error");
        *(c_uint8_t *)(new->payload) = h.sequence_number;

        if (integrity_protected)
        {
            c_uint8_t mac[NAS_SECURITY_MAC_SIZE];

            /* calculate NAS MAC(message authentication code) */
            nas_mac_calculate(mme_ue->selected_int_algorithm,
                mme_ue->knas_int, mme_ue->dl_count, NAS_SECURITY_BEARER, 
                NAS_SECURITY_DOWNLINK_DIRECTION, new, mac);
            memcpy(&h.message_authentication_code, mac, sizeof(mac));
        }

        /* increase dl_count */
        mme_ue->dl_count = (mme_ue->dl_count + 1) & 0xffffff; /* Use 24bit */

        /* encode all security header */
        d_assert(CORE_OK == pkbuf_header(new, 5),
            pkbuf_free(new);return CORE_ERROR, "pkbuf_header error");
        memcpy(new->payload, &h, sizeof(nas_security_header_t));

        *pkbuf = new;

        mme_ue->security_context_available = 1;
    }

    return CORE_OK;
}

status_t nas_security_decode(mme_ue_t *mme_ue, 
    nas_security_header_type_t security_header_type, pkbuf_t *pkbuf)
{
    d_assert(mme_ue, return CORE_ERROR, "Null param");
    d_assert(pkbuf, return CORE_ERROR, "Null param");
    d_assert(pkbuf->payload, return CORE_ERROR, "Null param");

    if (security_header_type.service_request)
    {
#define SHORT_MAC_SIZE 2
        nas_ksi_and_sequence_number_t *ksi_and_sequence_number =
            pkbuf->payload + 1;
        c_uint16_t original_pkbuf_len = pkbuf->len;
        c_uint8_t original_mac[SHORT_MAC_SIZE];
        c_uint8_t estimated_sequence_number;
        c_uint8_t sequence_number_high_3bit;
        c_uint8_t mac[NAS_SECURITY_MAC_SIZE];

        if (mme_ue->selected_int_algorithm == 0)
        {
            d_warn("integrity algorithm is not defined");
            return CORE_ERROR;
        }

        d_assert(ksi_and_sequence_number, return CORE_ERROR, "Null param");
        estimated_sequence_number = 
            ksi_and_sequence_number->sequence_number;

        sequence_number_high_3bit = mme_ue->ul_count.sqn & 0xe0;
        if ((mme_ue->ul_count.sqn & 0x1f) > estimated_sequence_number)
        {
            sequence_number_high_3bit += 0x20;
        }
        estimated_sequence_number += sequence_number_high_3bit;

        if (mme_ue->ul_count.sqn > estimated_sequence_number)
            mme_ue->ul_count.overflow++;
        mme_ue->ul_count.sqn = estimated_sequence_number;

        pkbuf->len = 2;
        memcpy(original_mac, pkbuf->payload + 2, SHORT_MAC_SIZE);

        nas_mac_calculate(mme_ue->selected_int_algorithm,
            mme_ue->knas_int, mme_ue->ul_count.i32, NAS_SECURITY_BEARER,
            NAS_SECURITY_UPLINK_DIRECTION, pkbuf, mac);

        pkbuf->len = original_pkbuf_len;
        memcpy(pkbuf->payload + 2, original_mac, SHORT_MAC_SIZE);

        if (memcmp(mac + 2, pkbuf->payload + 2, 2) != 0)
        {
            d_warn("NAS MAC verification failed");
            mme_ue->mac_failed = 1;
        }

        return CORE_OK;
    }

    if (!mme_ue->security_context_available)
    {
        security_header_type.integrity_protected = 0;
        security_header_type.new_security_context = 0;
        security_header_type.ciphered = 0;
    }

    if (security_header_type.new_security_context)
    {
        mme_ue->ul_count.i32 = 0;
    }

    if (mme_ue->selected_enc_algorithm == 0)
        security_header_type.ciphered = 0;
    if (mme_ue->selected_int_algorithm == 0)
        security_header_type.integrity_protected = 0;

    if (security_header_type.ciphered || 
        security_header_type.integrity_protected)
    {
        nas_security_header_t *h = NULL;

        /* NAS Security Header */
        d_assert(CORE_OK == pkbuf_header(pkbuf, 6),
            return CORE_ERROR, "pkbuf_header error");
        h = pkbuf->payload;

        /* NAS Security Header.Sequence_Number */
        d_assert(CORE_OK == pkbuf_header(pkbuf, -5),
            return CORE_ERROR, "pkbuf_header error");

        /* calculate ul_count */
        if (mme_ue->ul_count.sqn > h->sequence_number)
            mme_ue->ul_count.overflow++;
        mme_ue->ul_count.sqn = h->sequence_number;

        if (security_header_type.integrity_protected)
        {
            c_uint8_t mac[NAS_SECURITY_MAC_SIZE];
            c_uint32_t mac32;
            c_uint32_t original_mac = h->message_authentication_code;

            /* calculate NAS MAC(message authentication code) */
            nas_mac_calculate(mme_ue->selected_int_algorithm,
                mme_ue->knas_int, mme_ue->ul_count.i32, NAS_SECURITY_BEARER, 
                NAS_SECURITY_UPLINK_DIRECTION, pkbuf, mac);
            h->message_authentication_code = original_mac;

            memcpy(&mac32, mac, NAS_SECURITY_MAC_SIZE);
            if (h->message_authentication_code != mac32)
            {
                d_warn("NAS MAC verification failed(0x%x != 0x%x)",
                        ntohl(h->message_authentication_code), ntohl(mac32));
                mme_ue->mac_failed = 1;
            }
        }

        /* NAS EMM Header or ESM Header */
        d_assert(CORE_OK == pkbuf_header(pkbuf, -1),
            return CORE_ERROR, "pkbuf_header error");

        if (security_header_type.ciphered)
        {
            /* decrypt NAS message */
            nas_encrypt(mme_ue->selected_enc_algorithm,
                mme_ue->knas_enc, mme_ue->ul_count.i32, NAS_SECURITY_BEARER,
                NAS_SECURITY_UPLINK_DIRECTION, pkbuf);
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
