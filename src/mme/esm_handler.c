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
    mme_ue_t *ue = NULL;
    pkbuf_t *pkbuf = NULL;
    status_t rv;

    d_assert(esm, return, "Null param");
    ue = esm->ue;
    d_assert(ue, return, "Null param");

    if (esm_information_response->presencemask &
            NAS_ESM_INFORMATION_RESPONSE_ACCESS_POINT_NAME_PRESENT)
    {
        esm->pdn = mme_pdn_find_by_apn(ue, 
                esm_information_response->access_point_name.apn);
        d_assert(esm->pdn, return, "No PDN Context[APN:%s])", 
            esm_information_response->access_point_name.apn);
    }

    if (esm_information_response->presencemask &
            NAS_ESM_INFORMATION_RESPONSE_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT)
    {
        nas_protocol_configuration_options_t *protocol_configuration_options = 
            &esm_information_response->protocol_configuration_options;
        esm->ue_pco_len = protocol_configuration_options->length;
        d_assert(esm->ue_pco_len <= MAX_PCO_LEN, return, 
                "length(%d) exceeds MAX:%d", esm->ue_pco_len, MAX_PCO_LEN);
        memcpy(esm->ue_pco, protocol_configuration_options->buffer, 
                esm->ue_pco_len);
    }

    rv = mme_s11_build_create_session_request(&pkbuf, esm);
    d_assert(rv == CORE_OK, return, "S11 build error");

    rv = mme_s11_send_to_sgw(esm->sgw, 
            GTP_CREATE_SESSION_REQUEST_TYPE, 0, pkbuf);
    d_assert(rv == CORE_OK, return, "S11 send error");
}
