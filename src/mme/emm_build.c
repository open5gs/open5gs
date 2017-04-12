#define TRACE_MODULE _emm_build

#include "core_debug.h"

#include "nas_message.h"

#include "nas_security.h"
#include "emm_build.h"

status_t emm_build_attach_accept(pkbuf_t **pkbuf, mme_ue_t *ue)
{
    nas_message_t message;

    d_assert(ue, return CORE_ERROR, "Null param");

    memset(&message, 0, sizeof(message));
    message.h.security_header_type = 
       NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED;
    message.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;

    message.emm.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = NAS_ATTACH_ACCEPT;

    d_assert(nas_security_encode(pkbuf, ue, &message) == CORE_OK && *pkbuf,,);

    return CORE_OK;
}

