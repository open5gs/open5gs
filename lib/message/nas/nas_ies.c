#define TRACE_MODULE _nasies

#include "core_debug.h"
#include "nas_ies.h"

status_t nas_decode_attach_info(
    nas_attach_info_t *attach_info, pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;

    size = sizeof(nas_attach_info_t);
    d_assert(pkbuf->len >= size, return CORE_ERROR, 
            "pkbuf->len(%d), size(%d)\n", pkbuf->len, size);
    memcpy(attach_info, pkbuf->payload, size);
    pkbuf_header(pkbuf, -size);
    
    return CORE_OK;
}

status_t nas_decode_eps_mobile_identity(
    nas_eps_mobile_identity_t *eps_mobile_identity, pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;
    nas_eps_mobile_identity_t *header = pkbuf->payload;

    size = header->len + sizeof(header->len);
    d_assert(pkbuf->len >= size, return CORE_ERROR, 
            "pkbuf->len(%d), size(%d)\n", pkbuf->len, size);
    memcpy(eps_mobile_identity, pkbuf->payload, size);

    if (header->type_of_identity == NAS_EPS_MOBILE_IDENTITY_GUTI)
    {
        eps_mobile_identity->u.guti.mme_group_id = 
            ntohs(eps_mobile_identity->u.guti.mme_group_id);
        eps_mobile_identity->u.guti.m_tmsi = 
            ntohl(eps_mobile_identity->u.guti.m_tmsi);
    }

    pkbuf_header(pkbuf, -size);
    
    return CORE_OK;
}

status_t nas_decode_ue_network_capability(
    nas_ue_network_capability_t *ue_network_capability, pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;
    nas_ue_network_capability_t *header = pkbuf->payload;

    size = header->len + sizeof(header->len);
    d_assert(pkbuf->len >= size, return CORE_ERROR, 
            "pkbuf->len(%d), size(%d)\n", pkbuf->len, size);
    memcpy(ue_network_capability, pkbuf->payload, size);
    pkbuf_header(pkbuf, -size);

    return CORE_OK;
}
