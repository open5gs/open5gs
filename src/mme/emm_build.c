#define TRACE_MODULE _emm_build

#include "core_debug.h"

#include "nas_message.h"

#include "nas_security.h"
#include "emm_build.h"

status_t emm_build_attach_accept(
        pkbuf_t **emmbuf, mme_ue_t *ue, pkbuf_t *esmbuf)
{
    nas_message_t message;
    nas_attach_accept_t *attach_accept = &message.emm.attach_accept;

    d_assert(ue, return CORE_ERROR, "Null param");

    memset(&message, 0, sizeof(message));
    message.h.security_header_type = 
       NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED;
    message.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;

    message.emm.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = NAS_ATTACH_ACCEPT;

    attach_accept->esm_message_container.data = esmbuf->payload;
    attach_accept->esm_message_container.len = esmbuf->len;

    d_assert(nas_security_encode(emmbuf, ue, &message) == CORE_OK && *emmbuf,,);

    return CORE_OK;
}

