#define TRACE_MODULE _mme_path

#include "s1ap_path.h"
#include "nas_path.h"
#include "mme_gtp_path.h"
#include "mme_path.h"

status_t mme_send_detach_accept(mme_ue_t *mme_ue)
{
    status_t rv;

    d_assert(mme_ue, return CORE_ERROR,);

    if (SESSION_CONTEXT_IS_VALID(mme_ue))
    {
        rv = mme_gtp_send_delete_all_sessions(mme_ue);
        d_assert(rv == CORE_OK,,
            "mme_gtp_send_delete_all_sessions failed");
    }
    else
    {
        rv = nas_send_detach_accept(mme_ue);
        d_assert(rv == CORE_OK,,
            "nas_send_detach_accept failed");
    }

    return rv;
}

status_t mme_send_ue_context_release_command(mme_ue_t *mme_ue, enb_ue_t *enb_ue)
{
    status_t rv;

    d_assert(enb_ue, return CORE_ERROR,);

    if (SESSION_CONTEXT_IS_VALID(mme_ue))
    {
        rv = mme_gtp_send_delete_all_sessions(mme_ue);
        d_assert(rv == CORE_OK,,
            "mme_gtp_send_delete_all_sessions failed");
    }
    else
    {
        rv = s1ap_send_ue_context_release_command(enb_ue,
                S1ap_Cause_PR_nas, S1ap_CauseNas_normal_release,
                S1AP_UE_CTX_REL_REMOVE_MME_UE_CONTEXT, 0);
        d_assert(rv == CORE_OK,, "s1ap send error");
    }

    return rv;
}
