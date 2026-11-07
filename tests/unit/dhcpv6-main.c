/*
 * Copyright (C) 2026 by Nimbus Solutions
 *
 * Standalone runner for the DHCPv6-PD unit tests. The combined tests/unit
 * binary already links AMF, whose metrics and NGAP symbols collide with
 * SMF, so this binary links SMF only.
 */

#include "ogs-core.h"
#include "core/abts.h"

void ogs_sbi_message_init(int num_of_request_pool, int num_of_response_pool);
void ogs_sbi_message_final(void);

extern int __ogs_nas_domain;
extern int __ogs_gtp_domain;
extern int __ogs_sbi_domain;
extern int __ogs_app_domain;
extern int __ogs_diam_domain;
extern int __ogs_pfcp_domain;
extern int __ogs_ngap_domain;
extern int __smf_log_domain;
extern int __gsm_log_domain;

abts_suite *test_dhcpv6(abts_suite *suite);

static void terminate(void)
{
    ogs_sbi_message_final();
    ogs_pkbuf_default_destroy();
    ogs_core_terminate();
}

int main(int argc, const char *const argv[])
{
    int rv, opt;
    ogs_getopt_t options;
    struct {
        char *log_level;
        char *domain_mask;
    } optarg;
    const char *argv_out[argc+3];
    abts_suite *suite = NULL;
    ogs_pkbuf_config_t config;

    rv = abts_main(argc, argv, argv_out);
    if (rv != OGS_OK)
        return rv;

    memset(&optarg, 0, sizeof(optarg));
    ogs_getopt_init(&options, (char **)argv_out);
    while ((opt = ogs_getopt(&options, "e:m:")) != -1) {
        switch (opt) {
        case 'e':
            optarg.log_level = options.optarg;
            break;
        case 'm':
            optarg.domain_mask = options.optarg;
            break;
        default:
            fprintf(stderr, "%s: should not be reached\n", OGS_FUNC);
            return OGS_ERROR;
        }
    }

    ogs_core_initialize();
    ogs_pkbuf_default_init(&config);
    ogs_pkbuf_default_create(&config);
    ogs_sbi_message_init(32, 32);

    ogs_log_install_domain(&__ogs_nas_domain, "nas", OGS_LOG_ERROR);
    ogs_log_install_domain(&__ogs_gtp_domain, "gtp", OGS_LOG_ERROR);
    ogs_log_install_domain(&__ogs_sbi_domain, "sbi", OGS_LOG_ERROR);
    ogs_log_install_domain(&__ogs_app_domain, "app", OGS_LOG_ERROR);
    ogs_log_install_domain(&__ogs_diam_domain, "diam", OGS_LOG_ERROR);
    ogs_log_install_domain(&__ogs_pfcp_domain, "pfcp", OGS_LOG_ERROR);
    ogs_log_install_domain(&__ogs_ngap_domain, "ngap", OGS_LOG_ERROR);
    ogs_log_install_domain(&__smf_log_domain, "smf", OGS_LOG_ERROR);
    ogs_log_install_domain(&__gsm_log_domain, "gsm", OGS_LOG_ERROR);

    atexit(terminate);

    rv = ogs_log_config_domain(optarg.domain_mask, optarg.log_level);
    if (rv != OGS_OK)
        return rv;

    suite = test_dhcpv6(suite);
    return abts_report(suite);
}
