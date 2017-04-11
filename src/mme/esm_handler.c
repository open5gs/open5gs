#define TRACE_MODULE _esm_handler

#include "core_debug.h"

#include "nas_message.h"

#include "mme_context.h"
#include "mme_event.h"
#include "mme_s11_build.h"
#include "mme_s11_path.h"

void esm_handle_pdn_connectivity_request(mme_esm_t *esm, 
        nas_pdn_connectivity_request_t *pdn_connectivity_request)
{
#if 0 /* TODO */
    printf("request type = 0x%x, %d\n", 
            pdn_connectivity_request->request_type.request_type,
            pdn_connectivity_request->request_type.pdn_type);
    printf("precesce = 0x%x\n", pdn_connectivity_request->presencemask);
    printf("flag = 0x%x\n", 
            pdn_connectivity_request->esm_information_transfer_flag.security_protected_required);
#endif
}

void esm_handle_information_response(mme_esm_t *esm, 
        nas_esm_information_response_t *esm_information_response)
{
    pkbuf_t *pkbuf;

    mme_s11_build_create_session_req(&pkbuf, esm);
    mme_s11_send_to_sgw(esm->sgw, GTP_CREATE_SESSION_REQUEST_TYPE, 0, pkbuf);
}
