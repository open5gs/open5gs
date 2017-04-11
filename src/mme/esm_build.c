#define TRACE_MODULE _esm_build

#include "core_debug.h"

#include "nas_message.h"

#include "nas_security.h"
#include "esm_build.h"

status_t esm_build_information_request(pkbuf_t **pkbuf, mme_esm_t *esm)
{
    nas_message_t message;
    mme_ue_t *ue = NULL;

    d_assert(esm, return CORE_ERROR, "Null param");
    ue = esm->ue;
    d_assert(ue, return CORE_ERROR, "Null param");

    memset(&message, 0, sizeof(message));
    message.h.security_header_type = 
       NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED;
    message.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;

    message.esm.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_ESM;
    message.esm.h.procedure_transaction_identity = esm->pti;
    message.esm.h.message_type = NAS_ESM_INFORMATION_REQUEST;

    d_assert(nas_security_encode(pkbuf, ue, &message) == CORE_OK && *pkbuf,,);

    return CORE_OK;
}

