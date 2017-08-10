#define TRACE_MODULE _nas_path

#include "core_debug.h"
#include "s1ap_path.h"
#include "s1ap_build.h"
#include "esm_build.h"
#include "emm_build.h"
#include "nas_path.h"

status_t nas_send_to_enb(enb_ue_t *enb_ue, pkbuf_t *pkbuf)
{
    mme_enb_t *enb = NULL;

    d_assert(enb_ue, return CORE_ERROR, "Null param");
    enb = enb_ue->enb;
    d_assert(enb, return CORE_ERROR, "Null param");

    return s1ap_send_to_enb(enb, pkbuf);
}

status_t nas_send_to_downlink_nas_transport(
    mme_ue_t *mme_ue, pkbuf_t *pkbuf)
{
    status_t rv;
    pkbuf_t *s1apbuf = NULL;
    enb_ue_t *enb_ue = NULL;

    enb_ue = mme_ue->enb_ue;
    d_assert(enb_ue, return CORE_ERROR, "Null param");

    rv = s1ap_build_downlink_nas_transport(&s1apbuf, enb_ue, pkbuf);
    d_assert(rv == CORE_OK && s1apbuf, 
            pkbuf_free(pkbuf); return CORE_ERROR, "s1ap build error");

    return nas_send_to_enb(enb_ue, s1apbuf);
}
