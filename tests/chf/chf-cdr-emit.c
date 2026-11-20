/*
 * Copyright (C) 2026 by Rami Mohamed <ramrode@gmail.com>
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

/*
 * Write CDRs in both encodings, for test-cdr-decode.sh to compare.
 *
 * The claim this exists to test is that the JSON mirrors the BER: same
 * record, same field names, two syntaxes.  Asserting that inside the C
 * tests would only compare the writer against itself, so the check runs
 * the Python decoder - which is generated from the ASN.1 module, not
 * from any of this code - over the .ber and diffs the result against the
 * .json.
 *
 * Usage:  chf-cdr-emit <output-directory>
 *
 * Writes one CDR per session below <output-directory>/YYYY/MM/DD/ and
 * prints the paths it produced, one "json <path>" or "ber <path>" per
 * line.
 */

#include <stdio.h>
#include <string.h>

#include "ogs-app.h"
#include "ogs-sbi.h"
#include "chf/context.h"
#include "chf/cdr-path.h"

/* The sessions written, chosen to exercise the parts of the record whose
 * two renderings could plausibly disagree: a slice with an SD, a GPSI,
 * volumes past 2^32, and each closing cause. */
static const struct {
    const char *supi;
    const char *ref;
    const char *gpsi;
    const char *dnn;
    uint64_t    ul;
    uint64_t    dl;
    uint32_t    duration;
    uint8_t     sst;
    uint32_t    sd;
    uint32_t    rating_group;
    uint32_t    service_id;
    const char *cause;
} sessions[] = {
    { "imsi-001010000000001", "emit-0001", "msisdn-447700900001",
      "internet", 512000, 2048000, 300, 1, 0x010203, 10, 7,
      "normalRelease" },
    { "imsi-001010000000002", "emit-0002", "extid-user@example.com",
      "ims", 5ULL * 1024 * 1024 * 1024, 9ULL * 1024 * 1024 * 1024,
      86400, 2, OGS_S_NSSAI_NO_SD_VALUE, 20, 0, "volumeLimit" },
    { "imsi-001010000000003", "emit-0003", NULL,
      NULL, 0, 0, 0, 0, OGS_S_NSSAI_NO_SD_VALUE, 0, 0,
      "managementIntervention" },
};

int main(int argc, char *argv[])
{
    ogs_pkbuf_config_t config;
    size_t i;
    int rv = 0;

    if (argc < 2) {
        fprintf(stderr, "usage: %s <output-directory>\n", argv[0]);
        return 2;
    }

    ogs_core_initialize();
    ogs_pkbuf_default_init(&config);
    ogs_pkbuf_default_create(&config);

    ogs_app()->pool.sess = 128;
    ogs_app()->pool.nf = 16;
    ogs_app()->pool.nf_service = 16;
    ogs_app()->pool.event = 64;
    ogs_app()->pool.xact = 64;
    ogs_app()->pool.subscription = 16;
    ogs_app()->pool.message = 128;

    ogs_sbi_context_init(OpenAPI_nf_type_CHF);
    chf_context_init();

    chf_self()->cdr.enable = true;
    chf_self()->cdr.formats = CHF_CDR_FMT_JSON | CHF_CDR_FMT_ASN1;
    if (chf_self()->cdr.path)
        ogs_free(chf_self()->cdr.path);
    chf_self()->cdr.path = ogs_strdup(argv[1]);
    chf_self()->cdr.json.aggregate = false;
    chf_self()->cdr.asn1.aggregate = false;

    chf_cdr_init();

    for (i = 0; i < sizeof(sessions) / sizeof(sessions[0]); i++) {
        chf_sess_t *sess;
        char *path;

        sess = chf_sess_add(sessions[i].supi, sessions[i].ref);
        if (!sess) {
            fprintf(stderr, "chf_sess_add failed for %s\n",
                    sessions[i].supi);
            rv = 1;
            break;
        }

        if (sessions[i].gpsi)
            sess->gpsi = ogs_strdup(sessions[i].gpsi);
        if (sessions[i].dnn)
            sess->dnn = ogs_strdup(sessions[i].dnn);
        sess->smf_id = ogs_strdup("11111111-2222-3333-4444-555555555555");
        sess->psi = (uint8_t)(i + 1);
        sess->ul_octets = sessions[i].ul;
        sess->dl_octets = sessions[i].dl;
        sess->duration = sessions[i].duration;
        sess->rating_group = sessions[i].rating_group;
        sess->service_id = sessions[i].service_id;
        sess->s_nssai.sst = sessions[i].sst;
        sess->s_nssai.sd.v = sessions[i].sd;
        sess->period_start = time(NULL) - (time_t)sessions[i].duration;

        path = chf_cdr_write(sess, sessions[i].cause);
        if (!path) {
            fprintf(stderr, "chf_cdr_write failed for %s\n",
                    sessions[i].supi);
            rv = 1;
            chf_sess_remove(sess);
            break;
        }

        /* chf_cdr_write returns the JSON path; the .ber is beside it. */
        printf("json %s\n", path);
        {
            char ber[1024];
            char *dot;

            ogs_cpystrn(ber, path, sizeof(ber));
            dot = strrchr(ber, '.');
            if (dot) {
                ogs_cpystrn(dot, ".ber", sizeof(ber) - (dot - ber));
                printf("ber %s\n", ber);
            }
        }

        ogs_free(path);
        chf_sess_remove(sess);
    }

    chf_cdr_final();
    chf_context_final();
    ogs_sbi_context_final();
    ogs_pkbuf_default_destroy();
    ogs_core_terminate();

    return rv;
}
