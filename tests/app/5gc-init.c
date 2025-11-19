/*
 * Copyright (C) 2019,2020 by Sukchan Lee <acetcom@gmail.com>
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

/* If want to increase this number, check and modify run_threads() function
 * for static integer to string conversion */
#define OGS_MAX_NF_INSTANCES        4

static ogs_thread_t *nrf_thread = NULL;
static ogs_thread_t *scp_thread = NULL;
static ogs_thread_t *sepp_thread = NULL;
static ogs_thread_t *upf_threads[OGS_MAX_NF_INSTANCES] = { NULL };
static ogs_thread_t *smf_threads[OGS_MAX_NF_INSTANCES] = { NULL };
static ogs_thread_t *amf_threads[OGS_MAX_NF_INSTANCES] = { NULL };
static ogs_thread_t *ausf_threads[OGS_MAX_NF_INSTANCES] = { NULL };
static ogs_thread_t *udm_threads[OGS_MAX_NF_INSTANCES] = { NULL };
static ogs_thread_t *pcf_threads[OGS_MAX_NF_INSTANCES] = { NULL };
static ogs_thread_t *nssf_threads[OGS_MAX_NF_INSTANCES] = { NULL };
static ogs_thread_t *bsf_threads[OGS_MAX_NF_INSTANCES] = { NULL };
static ogs_thread_t *udr_threads[OGS_MAX_NF_INSTANCES] = { NULL };


static void run_threads(const char *nf_name, int count,
        const char *argv_out[], int argv_out_idx, ogs_thread_t *threads[])
{
    int i;

    threads[0] = test_child_create(nf_name, argv_out);

    for (i = 1; i < count; i++) {
        const char *idx_string = NULL;;

        switch (i) {
            case 1: idx_string = "1"; break;
            case 2: idx_string = "2"; break;
            case 3: idx_string = "3"; break;
            default:
                idx_string = ogs_msprintf("%d", i);
                ogs_warn("Missing static conversion of integer to string");
                break;
        }
        ogs_assert(idx_string);

        argv_out[argv_out_idx + 0] = "-k";
        argv_out[argv_out_idx + 1] = idx_string;
        argv_out[argv_out_idx + 2] = NULL;

        threads[i] = test_child_create(nf_name, argv_out);
    }

    // reset argv_out and remove the added "-k" parameter
    argv_out[argv_out_idx] = NULL;
}

int app_initialize(const char *const argv[])
{
    const char *argv_out[OGS_ARG_MAX];
    bool user_config = false;
    int i = 0;

    for (i = 0; argv[i] && i < OGS_ARG_MAX-3; i++) {
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

    if (ogs_global_conf()->parameter.no_nrf == 0)
        nrf_thread = test_child_create("nrf", argv_out);
    if (ogs_global_conf()->parameter.no_scp == 0)
        scp_thread = test_child_create("scp", argv_out);
    if (ogs_global_conf()->parameter.no_sepp == 0)
        sepp_thread = test_child_create("sepp", argv_out);

    if (ogs_global_conf()->parameter.no_upf == 0)
        run_threads("upf", ogs_global_conf()->parameter.upf_count,
                argv_out, i, upf_threads);
    if (ogs_global_conf()->parameter.no_smf == 0)
        run_threads("smf", ogs_global_conf()->parameter.smf_count,
                argv_out, i, smf_threads);
    if (ogs_global_conf()->parameter.no_amf == 0)
        run_threads("amf", ogs_global_conf()->parameter.amf_count,
                argv_out, i, amf_threads);
    if (ogs_global_conf()->parameter.no_ausf == 0)
        run_threads("ausf", ogs_global_conf()->parameter.ausf_count,
                argv_out, i, ausf_threads);
    if (ogs_global_conf()->parameter.no_udm == 0)
        run_threads("udm", ogs_global_conf()->parameter.udm_count,
                argv_out, i, udm_threads);
    if (ogs_global_conf()->parameter.no_pcf == 0)
        run_threads("pcf", ogs_global_conf()->parameter.pcf_count,
                argv_out, i, pcf_threads);
    if (ogs_global_conf()->parameter.no_nssf == 0)
        run_threads("nssf", ogs_global_conf()->parameter.nssf_count,
                argv_out, i, nssf_threads);
    if (ogs_global_conf()->parameter.no_bsf == 0)
        run_threads("bsf", ogs_global_conf()->parameter.bsf_count,
                argv_out, i, bsf_threads);
    if (ogs_global_conf()->parameter.no_udr == 0)
        run_threads("udr", ogs_global_conf()->parameter.udr_count,
                argv_out, i, udr_threads);

    /*
     * Wait for all sockets listening
     * 
     * If freeDiameter is not used, it uses a delay of less than 4 seconds.
     */
    ogs_msleep(3000);

    return OGS_OK;
}

void app_terminate(void)
{
    int i;

    for (i = 0; i < OGS_MAX_NF_INSTANCES; i++) {
        if (amf_threads[i])
            ogs_thread_destroy(amf_threads[i]);
        if (smf_threads[i])
            ogs_thread_destroy(smf_threads[i]);
        if (upf_threads[i])
            ogs_thread_destroy(upf_threads[i]);
        if (udr_threads[i])
            ogs_thread_destroy(udr_threads[i]);
        if (nssf_threads[i])
            ogs_thread_destroy(nssf_threads[i]);
        if (bsf_threads[i])
            ogs_thread_destroy(bsf_threads[i]);
        if (pcf_threads[i])
            ogs_thread_destroy(pcf_threads[i]);
        if (udm_threads[i])
            ogs_thread_destroy(udm_threads[i]);
        if (ausf_threads[i])
            ogs_thread_destroy(ausf_threads[i]);
    }

    if (sepp_thread) ogs_thread_destroy(sepp_thread);
    if (scp_thread) ogs_thread_destroy(scp_thread);
    if (nrf_thread) ogs_thread_destroy(nrf_thread);
}

void test_5gc_init(void)
{
    ogs_log_install_domain(&__ogs_sctp_domain, "sctp", OGS_LOG_ERROR);
    ogs_log_install_domain(&__ogs_ngap_domain, "ngap", OGS_LOG_ERROR);
    ogs_log_install_domain(&__ogs_dbi_domain, "dbi", OGS_LOG_ERROR);
    ogs_log_install_domain(&__ogs_nas_domain, "nas", OGS_LOG_ERROR);
    ogs_log_install_domain(&__ogs_gtp_domain, "gtp", OGS_LOG_ERROR);
    ogs_log_install_domain(&__ogs_sbi_domain, "sbi", OGS_LOG_ERROR);

    ogs_sctp_init(ogs_app()->usrsctp.udp_port);
    ogs_assert(ogs_dbi_init(ogs_app()->db_uri) == OGS_OK);
}

void test_5gc_final(void)
{
    ogs_dbi_final();
    ogs_sctp_final();

    test_context_final();
}
