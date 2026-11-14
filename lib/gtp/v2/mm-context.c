/*
 * Copyright (C) 2026 by Open5GS contributors
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "ogs-gtp.h"

typedef struct cursor_s {
    const uint8_t *p;
    size_t left;
} cursor_t;

static bool take(cursor_t *c, void *out, size_t n)
{
    if (n > c->left) return false;
    if (out && n) memcpy(out, c->p, n);
    c->p += n;
    c->left -= n;
    return true;
}

static bool number(cursor_t *c, uint32_t *out, size_t n)
{
    size_t i;
    uint32_t v = 0;
    if (n > c->left) return false;
    for (i = 0; i < n; i++) v = (v << 8) | c->p[i];
    *out = v;
    return take(c, NULL, n);
}

static bool blob(cursor_t *c, ogs_tlv_octet_t *out, size_t width)
{
    uint32_t n;
    if (!number(c, &n, width) || n > c->left) return false;
    if (out) {
        out->presence = 1;
        out->data = (void *)c->p;
        out->len = n;
    }
    return take(c, NULL, n);
}

/* TS 29.274 Figure 8.38-5 append-only extension sequence. An IE may end
 * before any later-release field, but not halfway through one. */
static bool extensions_valid(const uint8_t *p, size_t n)
{
    static const uint8_t widths[] = {1, 2, 1, 1, 1, 2, 1, 1};
    cursor_t c = {p, n};
    size_t i;
    for (i = 0; i < OGS_ARRAY_SIZE(widths) && c.left; i++)
        if (!blob(&c, NULL, widths[i])) return false;
    if (c.left && !take(&c, NULL, 1)) return false; /* ENSCT/TRIDI */
    /* Future extension octets are ignored as specified in 8.38. */
    return true;
}

int ogs_gtp2_parse_eps_mm_context(ogs_gtp2_eps_mm_context_t *context,
        const ogs_tlv_octet_t *octet)
{
    ogs_gtp2_eps_mm_context_t v = {0};
    cursor_t c;
    uint8_t flags[3], old;
    unsigned int i;

    if (!context || !octet || !octet->data || octet->len < 45 ||
        octet->len > UINT16_MAX)
        return OGS_ERROR;
    c.p = octet->data;
    c.left = octet->len;
    if (!take(&c, flags, sizeof(flags)) || (flags[0] >> 5) != 4)
        return OGS_ERROR;
    /* Quintuplets must not be sent to another MME (TS 33.401 6.1.6). */
    if (flags[1] >> 5) return OGS_ERROR;
    v.num_vectors = (flags[1] >> 2) & 7;
    if (v.num_vectors > OGS_ARRAY_SIZE(v.vector)) return OGS_ERROR;
    v.ksi = flags[0] & 7;
    v.nh_present = (flags[0] & 0x10) != 0;
    v.drx_present = (flags[0] & 8) != 0;
    v.old_context_present = (flags[1] & 1) != 0;
    v.used_ambr_present = (flags[1] & 2) != 0;
    v.subscribed_ambr_present = (flags[2] & 0x80) != 0;
    v.integrity_algorithm = (flags[2] >> 4) & 7;
    v.cipher_algorithm = flags[2] & 15;
    if (!number(&c, &v.dl_count, 3) || !number(&c, &v.ul_count, 3) ||
        !take(&c, v.kasme, sizeof(v.kasme))) return OGS_ERROR;
    for (i = 0; i < v.num_vectors; i++) {
        ogs_gtp2_eps_auth_vector_t *a = &v.vector[i];
        if (!take(&c, a->rand, sizeof(a->rand)) ||
            !take(&c, &a->xres_len, 1) ||
            a->xres_len < 4 || a->xres_len > sizeof(a->xres) ||
            !take(&c, a->xres, a->xres_len) ||
            !take(&c, &a->autn_len, 1) ||
            a->autn_len != sizeof(a->autn) ||
            !take(&c, a->autn, a->autn_len) ||
            !take(&c, a->kasme, sizeof(a->kasme))) return OGS_ERROR;
    }
    if (v.drx_present && !take(&c, v.drx, sizeof(v.drx))) return OGS_ERROR;
    if (v.nh_present && (!take(&c, v.nh, sizeof(v.nh)) ||
                        !take(&c, &v.ncc, 1))) return OGS_ERROR;
    v.ncc &= 7;
    if (v.subscribed_ambr_present &&
        (!number(&c, &v.subscribed_ambr_ul, 4) ||
         !number(&c, &v.subscribed_ambr_dl, 4))) return OGS_ERROR;
    if (v.used_ambr_present &&
        (!number(&c, &v.used_ambr_ul, 4) ||
         !number(&c, &v.used_ambr_dl, 4))) return OGS_ERROR;
    if (!blob(&c, &v.ue_network_capability, 1) ||
        !blob(&c, &v.ms_network_capability, 1) ||
        !blob(&c, &v.mei, 1) || !take(&c, &v.access_restriction, 1))
        return OGS_ERROR;
    if (v.old_context_present) {
        if (!take(&c, &old, 1)) return OGS_ERROR;
        v.old_ksi = (old >> 3) & 7;
        v.old_ncc = old & 7;
        v.old_nh_present = (old & 0x80) != 0;
        v.rlos = (old & 0x40) != 0;
        if (!take(&c, v.old_kasme, sizeof(v.old_kasme)) ||
            (v.old_nh_present && !take(&c, v.old_nh, sizeof(v.old_nh))))
            return OGS_ERROR;
    }
    if (!extensions_valid(c.p, c.left)) return OGS_ERROR;
    v.extensions.presence = c.left != 0;
    v.extensions.data = (void *)c.p;
    v.extensions.len = c.left;
    *context = v;
    return OGS_OK;
}

static bool valid_blob(const ogs_tlv_octet_t *v)
{
    return v->len <= 255 && (!v->len || v->data);
}

static void put(uint8_t **p, const void *src, size_t n)
{
    if (n) memcpy(*p, src, n);
    *p += n;
}

static void put_number(uint8_t **p, uint32_t v, size_t n)
{
    size_t i;
    for (i = n; i > 0; i--) *(*p)++ = (v >> ((i - 1) * 8)) & 0xff;
}

static void put_blob(uint8_t **p, const ogs_tlv_octet_t *v)
{
    *(*p)++ = v->len;
    put(p, v->data, v->len);
}

int ogs_gtp2_build_eps_mm_context(ogs_tlv_octet_t *octet,
        const ogs_gtp2_eps_mm_context_t *v, void *data, size_t capacity)
{
    size_t n = 45; /* 3 flags + 6 COUNT + 32 KASME + 3 lengths + access */
    unsigned int i;
    uint8_t *p = data;

    if (!octet || !v || !data || v->ksi > 7 ||
        v->integrity_algorithm > 7 || v->cipher_algorithm > 15 ||
        v->dl_count > 0xffffff || v->ul_count > 0xffffff || v->ncc > 7 ||
        v->old_ksi > 7 || v->old_ncc > 7 ||
        v->num_vectors > OGS_ARRAY_SIZE(v->vector) ||
        !valid_blob(&v->ue_network_capability) ||
        !valid_blob(&v->ms_network_capability) || !valid_blob(&v->mei) ||
        v->extensions.len > UINT16_MAX ||
        (v->extensions.len && !v->extensions.data) ||
        !extensions_valid(v->extensions.data, v->extensions.len))
        return OGS_ERROR;
    for (i = 0; i < v->num_vectors; i++) {
        const ogs_gtp2_eps_auth_vector_t *a = &v->vector[i];
        if (a->xres_len < 4 || a->xres_len > sizeof(a->xres) ||
            a->autn_len != sizeof(a->autn)) return OGS_ERROR;
        n += 16 + 1 + a->xres_len + 1 + a->autn_len + 32;
    }
    n += v->drx_present ? 2 : 0;
    n += v->nh_present ? 33 : 0;
    n += v->subscribed_ambr_present ? 8 : 0;
    n += v->used_ambr_present ? 8 : 0;
    n += v->ue_network_capability.len + v->ms_network_capability.len + v->mei.len;
    n += v->old_context_present ? (33 + (v->old_nh_present ? 32 : 0)) : 0;
    n += v->extensions.len;
    if (n > capacity || n > UINT16_MAX) return OGS_ERROR;

    *p++ = 0x80 | (v->nh_present ? 0x10 : 0) |
        (v->drx_present ? 8 : 0) | v->ksi;
    *p++ = (v->num_vectors << 2) | (v->used_ambr_present ? 2 : 0) |
        (v->old_context_present ? 1 : 0);
    *p++ = (v->subscribed_ambr_present ? 0x80 : 0) |
        (v->integrity_algorithm << 4) | v->cipher_algorithm;
    put_number(&p, v->dl_count, 3);
    put_number(&p, v->ul_count, 3);
    put(&p, v->kasme, sizeof(v->kasme));
    for (i = 0; i < v->num_vectors; i++) {
        const ogs_gtp2_eps_auth_vector_t *a = &v->vector[i];
        put(&p, a->rand, sizeof(a->rand));
        *p++ = a->xres_len;
        put(&p, a->xres, a->xres_len);
        *p++ = a->autn_len;
        put(&p, a->autn, a->autn_len);
        put(&p, a->kasme, sizeof(a->kasme));
    }
    if (v->drx_present) put(&p, v->drx, sizeof(v->drx));
    if (v->nh_present) {
        put(&p, v->nh, sizeof(v->nh));
        *p++ = v->ncc;
    }
    if (v->subscribed_ambr_present) {
        put_number(&p, v->subscribed_ambr_ul, 4);
        put_number(&p, v->subscribed_ambr_dl, 4);
    }
    if (v->used_ambr_present) {
        put_number(&p, v->used_ambr_ul, 4);
        put_number(&p, v->used_ambr_dl, 4);
    }
    put_blob(&p, &v->ue_network_capability);
    put_blob(&p, &v->ms_network_capability);
    put_blob(&p, &v->mei);
    *p++ = v->access_restriction;
    if (v->old_context_present) {
        *p++ = (v->old_nh_present ? 0x80 : 0) | (v->rlos ? 0x40 : 0) |
            (v->old_ksi << 3) | v->old_ncc;
        put(&p, v->old_kasme, sizeof(v->old_kasme));
        if (v->old_nh_present) put(&p, v->old_nh, sizeof(v->old_nh));
    }
    put(&p, v->extensions.data, v->extensions.len);
    ogs_assert((size_t)(p - (uint8_t *)data) == n);
    octet->presence = 1;
    octet->data = data;
    octet->len = n;
    return OGS_OK;
}
