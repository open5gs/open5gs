/*
 * Copyright (C) 2026 by Open5GS contributors
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#if !defined(OGS_GTP_INSIDE) && !defined(OGS_GTP_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_GTP2_MM_CONTEXT_H
#define OGS_GTP2_MM_CONTEXT_H

#ifdef __cplusplus
extern "C" {
#endif

/* TS 29.274 8.38, Figure 8.38-5. Counts are the next downlink COUNT and
 * the last successfully verified uplink COUNT, respectively. Bitrates in
 * this wire representation are in kbit/s. Octet strings returned by parse
 * borrow the input buffer; the caller must keep it alive. Never log keys. */
typedef struct ogs_gtp2_eps_auth_vector_s {
    uint8_t rand[16];
    uint8_t xres_len;
    uint8_t xres[16];
    uint8_t autn_len;
    uint8_t autn[16];
    uint8_t kasme[32];
} ogs_gtp2_eps_auth_vector_t;

typedef struct ogs_gtp2_eps_mm_context_s {
    uint8_t ksi;
    uint8_t integrity_algorithm;
    uint8_t cipher_algorithm;
    uint32_t dl_count;
    uint32_t ul_count;
    uint8_t kasme[32];

    uint8_t num_vectors;
    ogs_gtp2_eps_auth_vector_t vector[5];
    bool drx_present;
    uint8_t drx[2];
    bool nh_present;
    uint8_t nh[32];
    uint8_t ncc;
    bool subscribed_ambr_present;
    uint32_t subscribed_ambr_ul;
    uint32_t subscribed_ambr_dl;
    bool used_ambr_present;
    uint32_t used_ambr_ul;
    uint32_t used_ambr_dl;
    ogs_tlv_octet_t ue_network_capability;
    ogs_tlv_octet_t ms_network_capability;
    ogs_tlv_octet_t mei;
    uint8_t access_restriction;

    /* Only permitted in Forward Relocation Request, TS 33.401 7.2.10. */
    bool old_context_present;
    uint8_t old_ksi;
    bool rlos;
    uint8_t old_kasme[32];
    bool old_nh_present;
    uint8_t old_nh[32];
    uint8_t old_ncc;

    /* Release-dependent fields following the security context, beginning
     * with Voice Domain Preference. Kept as a bounded, validated wire view
     * so optional fields are not lost when forwarding a context. */
    ogs_tlv_octet_t extensions;
} ogs_gtp2_eps_mm_context_t;

int ogs_gtp2_parse_eps_mm_context(ogs_gtp2_eps_mm_context_t *context,
        const ogs_tlv_octet_t *octet);
int ogs_gtp2_build_eps_mm_context(ogs_tlv_octet_t *octet,
        const ogs_gtp2_eps_mm_context_t *context, void *data, size_t capacity);

#ifdef __cplusplus
}
#endif
#endif
