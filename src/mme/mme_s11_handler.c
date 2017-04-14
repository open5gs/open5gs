#define TRACE_MODULE _mme_s11_handler

#include "core_debug.h"

#include "gtp_types.h"

#include "mme_event.h"
#include "mme_context.h"
#include "mme_s11_handler.h"

void mme_s11_handle_create_session_response(
        mme_ue_t *ue, gtp_create_session_response_t *rsp)
{
    event_t e;

    gtp_f_teid_t *sgw_s11_teid = NULL;
    gtp_f_teid_t *sgw_s1u_teid = NULL;

    mme_bearer_t *bearer = NULL;
    pdn_t *pdn = NULL;
    
    d_assert(ue, return, "Null param");
    d_assert(rsp, return, "Null param");

    if (rsp->sender_f_teid_for_control_plane.presence == 0)
    {
        d_error("No GTP TEID");
        return;
    }
    if (rsp->pdn_address_allocation.presence == 0)
    {
        d_error("No PDN Address Allocation");
        return;
    }
    if (rsp->bearer_contexts_created.presence == 0)
    {
        d_error("No Bearer");
        return;
    }
    if (rsp->bearer_contexts_created.eps_bearer_id.presence == 0)
    {
        d_error("No EPS Bearer ID");
        return;
    }
    if (rsp->bearer_contexts_created.s1_u_enodeb_f_teid.presence == 0)
    {
        d_error("No GTP TEID");
        return;
    }

    bearer = mme_bearer_find_by_ebi(
            ue, rsp->bearer_contexts_created.eps_bearer_id.u8);
    d_assert(bearer, return, "No ESM Context");
    pdn = bearer->pdn;
    d_assert(pdn, return, "No PDN Context");

    /* Receive Control Plane(UL) : SGW-S11 */
    sgw_s11_teid = rsp->sender_f_teid_for_control_plane.data;
    ue->sgw_s11_teid = ntohl(sgw_s11_teid->teid);
    ue->sgw_s11_addr = sgw_s11_teid->ipv4_addr;

    memcpy(&pdn->paa, rsp->pdn_address_allocation.data,
            rsp->pdn_address_allocation.len);

    if (rsp->protocol_configuration_options.presence)
    {
        bearer->pgw_pco_len = rsp->protocol_configuration_options.len;
        memcpy(bearer->pgw_pco, rsp->protocol_configuration_options.data,
                bearer->pgw_pco_len);
    }

    /* Receive Data Plane(UL) : SGW-S1U */
    sgw_s1u_teid = rsp->bearer_contexts_created.s1_u_enodeb_f_teid.data;
    bearer->sgw_s1u_teid = ntohl(sgw_s1u_teid->teid);
    bearer->sgw_s1u_addr = sgw_s1u_teid->ipv4_addr;

    d_info("[GTP] Create Session Response : "
            "MME[%d] <-- SGW[%d]", ue->mme_s11_teid, ue->sgw_s11_teid);

    event_set(&e, MME_EVT_EMM_BEARER_FROM_S11);
    event_set_param1(&e, (c_uintptr_t)bearer->index);
    event_set_param2(&e, (c_uintptr_t)GTP_CREATE_SESSION_RESPONSE_TYPE);
    mme_event_send(&e);
}

void mme_s11_handle_modify_bearer_response(
        mme_ue_t *ue, gtp_modify_bearer_response_t *rsp)
{
#if 0
    event_t e;

    event_set(&e, MME_EVT_EMM_BEARER_LO_MODIFY_BEARER);
    event_set_param1(&e, (c_uintptr_t)bearer->index);
    mme_event_send(&e);
#endif
}
