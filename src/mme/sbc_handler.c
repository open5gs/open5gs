#define TRACE_MODULE _sbc_handler

#include "core_debug.h"

#include "mme_context.h"
#include "s1ap_path.h"
#include "s1ap_build.h"
#include "sbc_handler.h"

void sbc_handle_write_replace_warning_request(sbc_pws_data_t *sbc_pws)
{
    pkbuf_t *s1apbuf = NULL;
    hash_index_t *hi = NULL;
    mme_enb_t *enb = NULL;
    int i, j, flag;
    status_t rv;

    /* Find enB with matched TAI */
    for (hi = mme_enb_first(); hi; hi = mme_enb_next(hi))
    {
        flag = 0;
        enb = mme_enb_this(hi);
        if (sbc_pws->no_of_tai > 0)
        {
            for (i = 0, flag = 0; i < enb->num_of_supported_ta_list; i++)
            {
                for (j = 0; j < sbc_pws->no_of_tai; j++)
                {
                    if (!memcmp(&enb->supported_ta_list[i],
                                &sbc_pws->tai[j], sizeof(tai_t)))
                        flag = 1;

                    if (flag) break;
                }
                if (flag) break;
            }
        }
        else
            flag = 1;

        if (flag)
        {
            s1apbuf = NULL;

            /* Buidl S1AP Write Replace Warning Request message */
            rv = s1ap_build_write_replace_warning_request(&s1apbuf, sbc_pws);
            d_assert(rv == CORE_OK && s1apbuf, return, 
                    "s1ap build error");

            /* Send to enb */
            d_assert(s1ap_send_to_enb(
                    enb, s1apbuf, S1AP_NON_UE_SIGNALLING) == CORE_OK,
                    return, "s1ap send error");
        }
    }
}

void sbc_handle_stop_warning_request(sbc_pws_data_t *sbc_pws)
{
    pkbuf_t *s1apbuf = NULL;
    hash_index_t *hi = NULL;
    mme_enb_t *enb = NULL;
    int i, j, flag;
    status_t rv;

    /* Find enB with matched TAI */
    for (hi = mme_enb_first(); hi; hi = mme_enb_next(hi))
    {
        flag = 0;
        enb = mme_enb_this(hi);
        if (sbc_pws->no_of_tai > 0)
        {
            for (i = 0, flag = 0; i < enb->num_of_supported_ta_list; i++)
            {
                for (j = 0; j < sbc_pws->no_of_tai; j++)
                {
                    if (!memcmp(&enb->supported_ta_list[i],
                                &sbc_pws->tai[j], sizeof(tai_t)))
                        flag = 1;

                    if (flag) break;
                }
                if (flag) break;
            }
        }
        else
            flag = 1;

        if (flag)
        {
            s1apbuf = NULL;

            /* Buidl S1AP Kill request message */
            rv = s1ap_build_kill_request(&s1apbuf, sbc_pws);
            d_assert(rv == CORE_OK && s1apbuf, return, 
                    "s1ap build error");

            /* Send to enb */
            d_assert(s1ap_send_to_enb(
                    enb, s1apbuf, S1AP_NON_UE_SIGNALLING) == CORE_OK,
                    return, "s1ap send error");
        }
    }
}
