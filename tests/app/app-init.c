/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "test-app.h"

static ogs_thread_t *nrf_thread = NULL;
static ogs_thread_t *pcrf_thread = NULL;
static ogs_thread_t *hss_thread = NULL;
static ogs_thread_t *upf_thread = NULL;
static ogs_thread_t *sgwc_thread = NULL;
static ogs_thread_t *sgwu_thread = NULL;
static ogs_thread_t *smf_thread = NULL;
static ogs_thread_t *mme_thread = NULL;
static ogs_thread_t *amf_thread = NULL;
static ogs_thread_t *ausf_thread = NULL;
static ogs_thread_t *udm_thread = NULL;
static ogs_thread_t *pcf_thread = NULL;
static ogs_thread_t *nssf_thread = NULL;
static ogs_thread_t *bsf_thread = NULL;
static ogs_thread_t *udr_thread = NULL;

int app_initialize(const char *const argv[])
{
    const char *argv_out[OGS_ARG_MAX];
    bool user_config = false;
    int i = 0;

    for (i = 0; argv[i]; i++) {
        if (strcmp("-c", argv[i]) == 0) {
            user_config = true; 
        }
        argv_out[i] = argv[i];
    }
    argv_out[i] = NULL;

    if (!user_config) {
        argv_out[i++] = "-c";
        argv_out[i++] = DEFAULT_CONFIG_FILENAME;
        argv_out[i] = NULL;
    }

    if (ogs_app()->parameter.no_nrf == 0)
        nrf_thread = test_child_create("nrf", argv_out);
    if (ogs_app()->parameter.no_hss == 0)
        hss_thread = test_child_create("hss", argv_out);
    if (ogs_app()->parameter.no_pcrf == 0)
        pcrf_thread = test_child_create("pcrf", argv_out);

    if (ogs_app()->parameter.no_upf == 0)
        upf_thread = test_child_create("upf", argv_out);
    if (ogs_app()->parameter.no_sgwu == 0)
        sgwu_thread = test_child_create("sgwu", argv_out);

    if (ogs_app()->parameter.no_smf == 0)
        smf_thread = test_child_create("smf", argv_out);
    if (ogs_app()->parameter.no_sgwc == 0)
        sgwc_thread = test_child_create("sgwc", argv_out);

    if (ogs_app()->parameter.no_mme == 0)
        mme_thread = test_child_create("mme", argv_out);
    if (ogs_app()->parameter.no_amf == 0)
        amf_thread = test_child_create("amf", argv_out);

    if (ogs_app()->parameter.no_ausf == 0)
        ausf_thread = test_child_create("ausf", argv_out);
    if (ogs_app()->parameter.no_udm == 0)
        udm_thread = test_child_create("udm", argv_out);
    if (ogs_app()->parameter.no_pcf == 0)
        pcf_thread = test_child_create("pcf", argv_out);
    if (ogs_app()->parameter.no_nssf == 0)
        nssf_thread = test_child_create("nssf", argv_out);
    if (ogs_app()->parameter.no_bsf == 0)
        bsf_thread = test_child_create("bsf", argv_out);
    if (ogs_app()->parameter.no_udr == 0)
        udr_thread = test_child_create("udr", argv_out);

    /*
     * Wait for all sockets listening
     *
     * Note that at least 4 seconds are needed if freeDiameter is running.
     */
    ogs_msleep(5000);

    return OGS_OK;;
}

void app_terminate(void)
{
    if (amf_thread) ogs_thread_destroy(amf_thread);
    if (mme_thread) ogs_thread_destroy(mme_thread);

    if (sgwc_thread) ogs_thread_destroy(sgwc_thread);
    if (smf_thread) ogs_thread_destroy(smf_thread);

    if (sgwu_thread) ogs_thread_destroy(sgwu_thread);
    if (upf_thread) ogs_thread_destroy(upf_thread);

    if (udr_thread) ogs_thread_destroy(udr_thread);
    if (bsf_thread) ogs_thread_destroy(bsf_thread);
    if (nssf_thread) ogs_thread_destroy(nssf_thread);
    if (pcf_thread) ogs_thread_destroy(pcf_thread);
    if (udm_thread) ogs_thread_destroy(udm_thread);
    if (ausf_thread) ogs_thread_destroy(ausf_thread);

    if (hss_thread) ogs_thread_destroy(hss_thread);
    if (pcrf_thread) ogs_thread_destroy(pcrf_thread);
    if (nrf_thread) ogs_thread_destroy(nrf_thread);
}

void test_app_init(void)
{
    ogs_log_install_domain(&__ogs_sctp_domain, "sctp", OGS_LOG_ERROR);
    ogs_log_install_domain(&__ogs_s1ap_domain, "s1ap", OGS_LOG_ERROR);
    ogs_log_install_domain(&__ogs_ngap_domain, "ngap", OGS_LOG_ERROR);
    ogs_log_install_domain(&__ogs_diam_domain, "diam", OGS_LOG_ERROR);
    ogs_log_install_domain(&__ogs_dbi_domain, "dbi", OGS_LOG_ERROR);
    ogs_log_install_domain(&__ogs_nas_domain, "nas", OGS_LOG_ERROR);
    ogs_log_install_domain(&__ogs_gtp_domain, "gtp", OGS_LOG_ERROR);

    ogs_sctp_init(ogs_app()->usrsctp.udp_port);
    ogs_assert(ogs_dbi_init(ogs_app()->db_uri) == OGS_OK);
}

void test_app_final(void)
{
    ogs_dbi_final();
    ogs_sctp_final();

    test_context_final();
}
