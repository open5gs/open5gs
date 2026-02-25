/*
 * Simple NRPPa encoding harness for memory allocation experiments.
 *
 * This utility calls the production LMF ECID builder in isolation so that
 * memory issues (talloc contexts, pkbuf ownership, ASN.1 freeing order, etc.)
 * can be investigated without running the full daemon.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <getopt.h>
#include <talloc.h>

#include "ogs-core.h"
#include "ogs-sbi.h"
#include "lmf/nrppa-build.h"
#include "lmf/context.h"

static void usage(const char *prog)
{
    fprintf(stderr,
        "Usage: %s [options]\n"
        "  -m, --measurement-id <id>   Measurement ID (default: 1)\n"
        "  -u, --ue-ngap-id <id>       UE NGAP ID (default: 0)\n"
        "  -r, --requested <mask>      Requested measurements bitmap (default: 0x07)\n"
        "  -n, --iterations <count>    Number of encode/free cycles (default: 1)\n"
        "      --dump-talloc           Print talloc report after each cycle\n"
        "  -h, --help                  Show this help message\n",
        prog);
}

static void cleanup(void)
{
    ogs_sbi_message_final();
    ogs_pkbuf_default_destroy();
    ogs_core_terminate();
}

int main(int argc, char *argv[])
{
    uint32_t measurement_id = 1;
    uint64_t ue_ngap_id = 0;
    uint8_t requested = 0x07;
    int iterations = 1;
    bool dump_talloc = false;
    int opt;

    static const struct option long_opts[] = {
        {"measurement-id", required_argument, NULL, 'm'},
        {"ue-ngap-id", required_argument, NULL, 'u'},
        {"requested", required_argument, NULL, 'r'},
        {"iterations", required_argument, NULL, 'n'},
        {"dump-talloc", no_argument, NULL, 1},
        {"help", no_argument, NULL, 'h'},
        {0, 0, 0, 0}
    };

    while ((opt = getopt_long(argc, argv, "m:u:r:n:h", long_opts, NULL)) != -1) {
        switch (opt) {
        case 'm':
            measurement_id = (uint32_t)strtoul(optarg, NULL, 0);
            break;
        case 'u':
            ue_ngap_id = strtoull(optarg, NULL, 0);
            break;
        case 'r':
            requested = (uint8_t)strtoul(optarg, NULL, 0);
            break;
        case 'n':
            iterations = atoi(optarg);
            if (iterations <= 0)
                iterations = 1;
            break;
        case 1:
            dump_talloc = true;
            break;
        case 'h':
        default:
            usage(argv[0]);
            return opt == 'h' ? 0 : EXIT_FAILURE;
        }
    }

    ogs_core_initialize();
    ogs_log_install_domain(&__lmf_log_domain, "lmf", ogs_core()->log.level);

    ogs_pkbuf_config_t pkbuf_conf;
    ogs_pkbuf_default_init(&pkbuf_conf);
    ogs_pkbuf_default_create(&pkbuf_conf);

    ogs_sbi_message_init(32, 32);
    atexit(cleanup);

    int i;

    for (i = 0; i < iterations; ++i) {
        ogs_pkbuf_t *pkbuf =
            lmf_nrppa_build_ecid_measurement_request(
                measurement_id, ue_ngap_id, requested);
        if (!pkbuf) {
            fprintf(stderr, "Encoding failed in iteration %d\n", i);
            return EXIT_FAILURE;
        }

        printf("[iteration %d] encoded %zu bytes\n", i,
               pkbuf->len ? (size_t)pkbuf->len : 0);

        ogs_pkbuf_free(pkbuf);

        if (dump_talloc) {
            printf("[iteration %d] talloc report:\n", i);
            talloc_report_full(NULL, stdout);
        }
    }

    if (dump_talloc) {
        printf("[final] talloc report after cleanup:\n");
        talloc_report_full(NULL, stdout);
    }

    return EXIT_SUCCESS;
}

