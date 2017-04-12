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

    mme_esm_t *esm = NULL;
    
    d_assert(ue, return, "Null param");
    d_assert(rsp, return, "Null param");

    if (rsp->sender_f_teid_for_control_plane.presence == 0)
    {
        d_error("No GTP TEID");
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

    esm = mme_esm_find_by_ebi(
            ue, rsp->bearer_contexts_created.eps_bearer_id.u8);
    d_assert(esm, return, "No ESM Context");

    /* Receive Control Plane(UL) : SGW-S11 */
    sgw_s11_teid = rsp->sender_f_teid_for_control_plane.data;
    ue->sgw_s11_teid = ntohl(sgw_s11_teid->teid);
    ue->sgw_s11_addr = sgw_s11_teid->ipv4_addr;

    /* Receive Data Plane(UL) : SGW-S1U */
    sgw_s1u_teid = rsp->bearer_contexts_created.s1_u_enodeb_f_teid.data;
    esm->sgw_s1u_teid = ntohl(sgw_s1u_teid->teid);
    esm->sgw_s1u_addr = sgw_s1u_teid->ipv4_addr;

    event_set(&e, EVT_LO_MME_ESM_CREATE_SESSION);
    event_set_param1(&e, (c_uintptr_t)esm->index);
    mme_event_send(&e);
}
