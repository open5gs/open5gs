#include "s1ap_path.h"
#include "nas_path.h"
#include "mme_gtp_path.h"
#include "mme_path.h"

int mme_send_delete_session_or_detach(mme_ue_t *mme_ue)
{
    int rv;

    ogs_assert(mme_ue);

    if (SESSION_CONTEXT_IS_AVAILABLE(mme_ue))
    {
        rv = mme_gtp_send_delete_all_sessions(mme_ue);
        ogs_assert(rv == OGS_OK);
    }
    else
    {
        rv = nas_send_detach_accept(mme_ue);
        ogs_assert(rv == OGS_OK);
    }

    return rv;
}

int mme_send_delete_session_or_ue_context_release(
        mme_ue_t *mme_ue, enb_ue_t *enb_ue)
{
    int rv;

    ogs_assert(enb_ue);

    if (SESSION_CONTEXT_IS_AVAILABLE(mme_ue))
    {
        rv = mme_gtp_send_delete_all_sessions(mme_ue);
        ogs_assert(rv == OGS_OK);
    }
    else
    {
        rv = s1ap_send_ue_context_release_command(enb_ue,
                S1AP_Cause_PR_nas, S1AP_CauseNas_normal_release,
                S1AP_UE_CTX_REL_UE_CONTEXT_REMOVE, 0);
        ogs_assert(rv == OGS_OK);
    }

    return rv;
}

int mme_send_release_access_bearer_or_ue_context_release(
        mme_ue_t *mme_ue, enb_ue_t *enb_ue)
{
    int rv;

    ogs_assert(enb_ue);

    if (BEARER_CONTEXT_IS_ACTIVE(mme_ue))
    {
        rv = mme_gtp_send_release_access_bearers_request(mme_ue);
        ogs_assert(rv == OGS_OK);
    }
    else
    {
        rv = s1ap_send_ue_context_release_command(enb_ue,
                S1AP_Cause_PR_nas, S1AP_CauseNas_normal_release,
                S1AP_UE_CTX_REL_S1_NORMAL_RELEASE, 0);
        ogs_assert(rv == OGS_OK);
    }

    return rv;
}
