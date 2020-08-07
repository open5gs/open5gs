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

#include "ogs-core.h"

ogs_tlv_desc_t ogs_tlv_desc_more1 = { 
    OGS_TLV_MORE, "More", 0, 1, 0, 0, { NULL } };
ogs_tlv_desc_t ogs_tlv_desc_more2 = { 
    OGS_TLV_MORE, "More", 0, 2, 0, 0, { NULL } };
ogs_tlv_desc_t ogs_tlv_desc_more3 = { 
    OGS_TLV_MORE, "More", 0, 3, 0, 0, { NULL } };
ogs_tlv_desc_t ogs_tlv_desc_more4 = { 
    OGS_TLV_MORE, "More", 0, 4, 0, 0, { NULL } };
ogs_tlv_desc_t ogs_tlv_desc_more5 = { 
    OGS_TLV_MORE, "More", 0, 5, 0, 0, { NULL } };
ogs_tlv_desc_t ogs_tlv_desc_more6 = { 
    OGS_TLV_MORE, "More", 0, 6, 0, 0, { NULL } };
ogs_tlv_desc_t ogs_tlv_desc_more7 = { 
    OGS_TLV_MORE, "More", 0, 7, 0, 0, { NULL } };
ogs_tlv_desc_t ogs_tlv_desc_more8 = { 
    OGS_TLV_MORE, "More", 0, 8, 0, 0, { NULL } };

static ogs_tlv_t *tlv_add_leaf(
        ogs_tlv_t *parent_tlv, ogs_tlv_t *tlv, ogs_tlv_desc_t *desc, void *msg)
{
    switch (desc->ctype) {
    case OGS_TLV_UINT8:
    case OGS_TLV_INT8:
    {
        ogs_tlv_uint8_t *v = (ogs_tlv_uint8_t *)msg;
        if (parent_tlv)
            tlv = ogs_tlv_embed(parent_tlv, 
                    desc->type, 1, desc->instance, &v->u8);
        else
            tlv = ogs_tlv_add(tlv, desc->type, 1, desc->instance, &v->u8);
        ogs_assert(tlv);
        break;
    }
    case OGS_TLV_UINT16:
    {
        ogs_tlv_uint16_t *v = (ogs_tlv_uint16_t *)msg;

        v->u16 = htobe16(v->u16);

        if (parent_tlv)
            tlv = ogs_tlv_embed(parent_tlv, 
                    desc->type, 2, desc->instance, &v->u16);
        else
            tlv = ogs_tlv_add(tlv, desc->type, 2, desc->instance, &v->u16);
        ogs_assert(tlv);
        break;
    }
    case OGS_TLV_UINT24:
    case OGS_TLV_INT24:
    {
        ogs_tlv_uint24_t *v = (ogs_tlv_uint24_t *)msg;

        v->u24 = v->u24 << 8;
        v->u24 = htobe32(v->u24);

        if (parent_tlv)
            tlv = ogs_tlv_embed(parent_tlv, 
                    desc->type, 3, desc->instance, &v->u24);
        else
            tlv = ogs_tlv_add(tlv, desc->type, 3, desc->instance, &v->u24);
        ogs_assert(tlv);
        break;
    }
    case OGS_TLV_UINT32:
    case OGS_TLV_INT32:
    {
        ogs_tlv_uint32_t *v = (ogs_tlv_uint32_t *)msg;

        v->u32 = htobe32(v->u32);

        if (parent_tlv)
            tlv = ogs_tlv_embed(parent_tlv,
                    desc->type, 4, desc->instance, &v->u32);
        else
            tlv = ogs_tlv_add(tlv,
                    desc->type, 4, desc->instance, &v->u32);
        ogs_assert(tlv);
        break;
    }
    case OGS_TLV_FIXED_STR:
    {
        ogs_tlv_octet_t *v = (ogs_tlv_octet_t *)msg;

        if (parent_tlv)
            tlv = ogs_tlv_embed(parent_tlv, 
                    desc->type, desc->length, desc->instance, v->data);
        else
            tlv = ogs_tlv_add(tlv, 
                    desc->type, desc->length, desc->instance, v->data);
        ogs_assert(tlv);
        break;
    }
    case OGS_TLV_VAR_STR:
    {
        ogs_tlv_octet_t *v = (ogs_tlv_octet_t *)msg;

        if (v->len == 0) {
            ogs_fatal("No TLV length - [%s] T:%d I:%d (vsz=%d)",
                    desc->name, desc->type, desc->instance, desc->vsize);
            ogs_assert_if_reached();
        }

        if (parent_tlv)
            tlv = ogs_tlv_embed(parent_tlv, 
                    desc->type, v->len, desc->instance, v->data);
        else
            tlv = ogs_tlv_add(tlv, 
                    desc->type, v->len, desc->instance, v->data);
        ogs_assert(tlv);
        break;
    }
    case OGS_TLV_NULL:
    {
        if (parent_tlv)
            tlv = ogs_tlv_embed(parent_tlv, 
                    desc->type, 0, desc->instance, NULL);
        else
            tlv = ogs_tlv_add(tlv, 
                    desc->type, 0, desc->instance, NULL);
        ogs_assert(tlv);
        break;
    }
    default:
        ogs_assert_if_reached();
        break;
    }

    return tlv;
}

static uint32_t tlv_add_compound(ogs_tlv_t **root, ogs_tlv_t *parent_tlv,
        ogs_tlv_desc_t *parent_desc, void *msg, int depth)
{
    ogs_tlv_presence_t *presence_p;
    ogs_tlv_desc_t *desc = NULL, *next_desc = NULL;
    ogs_tlv_t *tlv = NULL, *emb_tlv = NULL;
    uint8_t *p = msg;
    uint32_t offset = 0, count = 0;
    int i, j, r;
    char indent[17] = "                "; /* 16 spaces */

    ogs_assert(root);
    ogs_assert(parent_desc);
    ogs_assert(msg);

    ogs_assert(depth <= 8);
    indent[depth*2] = 0;

    *root = NULL;

    for (i = 0, desc = parent_desc->child_descs[i]; desc != NULL;
            i++, desc = parent_desc->child_descs[i]) {
        next_desc = parent_desc->child_descs[i+1];
        if (next_desc != NULL && next_desc->ctype == OGS_TLV_MORE) {
            int offset2 = offset;
            for (j = 0; j < next_desc->length; j++) {
                presence_p = (ogs_tlv_presence_t *)(p + offset2);

                if (*presence_p == 0)
                    break;

                if (desc->ctype == OGS_TLV_COMPOUND) {
                    ogs_trace("BUILD %sC#%d [%s] T:%d I:%d (vsz=%d) off:%p ",
                            indent, i, desc->name, desc->type, desc->instance, 
                            desc->vsize, p + offset2);

                    if (parent_tlv)
                        tlv = ogs_tlv_embed(parent_tlv, 
                                desc->type, 0, desc->instance, NULL);
                    else
                        tlv = ogs_tlv_add(tlv,
                                desc->type, 0, desc->instance, NULL);

                    r = tlv_add_compound(&emb_tlv, tlv, desc,
                            p + offset2 + sizeof(ogs_tlv_presence_t),
                            depth + 1);
                    ogs_assert(r > 0 && emb_tlv);
                    count += 1 + r;
                } else {
                    ogs_trace("BUILD %sL#%d [%s] T:%d L:%d I:%d "
                            "(cls:%d vsz:%d) off:%p ",
                            indent, i, desc->name, desc->type, desc->length, 
                            desc->instance, desc->ctype, desc->vsize, 
                            p + offset2);

                    tlv = tlv_add_leaf(parent_tlv, tlv, desc, p + offset2);
                    ogs_assert(tlv);
                    count++;
                }

                if (*root == NULL)
                    *root = tlv;

                offset2 += desc->vsize;
            }
            offset += desc->vsize * next_desc->length;
            i++;
        } else {
            presence_p = (ogs_tlv_presence_t *)(p + offset);

            if (*presence_p) {
                if (desc->ctype == OGS_TLV_COMPOUND) {
                    ogs_trace("BUILD %sC#%d [%s] T:%d I:%d (vsz=%d) off:%p ",
                            indent, i, desc->name, desc->type, desc->instance, 
                            desc->vsize, p + offset);

                    if (parent_tlv)
                        tlv = ogs_tlv_embed(parent_tlv, 
                                desc->type, 0, desc->instance, NULL);
                    else
                        tlv = ogs_tlv_add(tlv,
                                desc->type, 0, desc->instance, NULL);

                    r = tlv_add_compound(&emb_tlv, tlv, desc,
                            p + offset + sizeof(ogs_tlv_presence_t),
                            depth + 1);
                    ogs_assert(r > 0 && emb_tlv);
                    count += 1 + r;
                } else {
                    ogs_trace("BUILD %sL#%d [%s] T:%d L:%d I:%d "
                            "(cls:%d vsz:%d) off:%p ",
                            indent, i, desc->name, desc->type, desc->length, 
                            desc->instance, desc->ctype, desc->vsize, 
                            p + offset);

                    tlv = tlv_add_leaf(parent_tlv, tlv, desc, p + offset);
                    ogs_assert(tlv);
                    count++;
                }

                if (*root == NULL)
                    *root = tlv;
            }
            offset += desc->vsize;
        }
    }

    return count;
}

ogs_pkbuf_t *ogs_tlv_build_msg(ogs_tlv_desc_t *desc, void *msg, int mode)
{
    ogs_tlv_t *root = NULL;
    uint32_t r, length, rendlen;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(desc);
    ogs_assert(msg);

    ogs_assert(desc->ctype == OGS_TLV_MESSAGE);
    
    if (desc->child_descs[0]) {
        r = tlv_add_compound(&root, NULL, desc, msg, 0);
        ogs_assert(r > 0 && root);

        length = ogs_tlv_calc_length(root, mode);
    } else {
        length = 0;
    }
    pkbuf = ogs_pkbuf_alloc(NULL, OGS_TLV_MAX_HEADROOM+length);
    ogs_assert(pkbuf);
    ogs_pkbuf_reserve(pkbuf, OGS_TLV_MAX_HEADROOM);
    ogs_pkbuf_put(pkbuf, length);

    if (desc->child_descs[0]) {
        rendlen = ogs_tlv_render(root, pkbuf->data, length, mode);
        ogs_assert(rendlen == length);

        ogs_tlv_free_all(root);
    }

    return pkbuf;
}

static ogs_tlv_desc_t* tlv_find_desc(uint8_t *desc_index,
        uint32_t *tlv_offset, ogs_tlv_desc_t *parent_desc, ogs_tlv_t *tlv)
{
    ogs_tlv_desc_t *prev_desc = NULL, *desc = NULL;
    int i, offset = 0;

    ogs_assert(parent_desc);
    ogs_assert(tlv);

    for (i = 0, desc = parent_desc->child_descs[i]; desc != NULL;
            i++, desc = parent_desc->child_descs[i]) {
        if (desc->type == tlv->type && desc->instance == tlv->instance) {
            *desc_index = i;
            *tlv_offset = offset;
            break;
        }

        if (desc->ctype == OGS_TLV_MORE) {
            ogs_assert(prev_desc && prev_desc->ctype != OGS_TLV_MORE);
            offset += prev_desc->vsize * (desc->length - 1);
        } else {
            offset += desc->vsize;
        }

        prev_desc = desc;
    }

    return desc;
}

static int tlv_parse_leaf(void *msg, ogs_tlv_desc_t *desc, ogs_tlv_t *tlv)
{
    ogs_assert(msg);
    ogs_assert(desc);
    ogs_assert(tlv);

    switch (desc->ctype) {
    case OGS_TLV_UINT8:
    case OGS_TLV_INT8:
    {
        ogs_tlv_uint8_t *v = (ogs_tlv_uint8_t *)msg;

        if (tlv->length != 1)
        {
            ogs_error("Invalid TLV length %d. It should be 1", tlv->length);
            return OGS_ERROR;
        }
        v->u8 = *(uint8_t*)(tlv->value);
        break;
    }
    case OGS_TLV_UINT16:
    case OGS_TLV_INT16:
    {
        ogs_tlv_uint16_t *v = (ogs_tlv_uint16_t *)msg;

        if (tlv->length != 2)
        {
            ogs_error("Invalid TLV length %d. It should be 2", tlv->length);
            return OGS_ERROR;
        }
        v->u16 = ((((uint8_t*)tlv->value)[0]<< 8)&0xff00) |
               ((((uint8_t*)tlv->value)[1]    )&0x00ff);
        break;
    }
    case OGS_TLV_UINT24:
    case OGS_TLV_INT24:
    {
        ogs_tlv_uint24_t *v = (ogs_tlv_uint24_t *)msg;

        if (tlv->length != 3)
        {
            ogs_error("Invalid TLV length %d. It should be 3", tlv->length);
            return OGS_ERROR;
        }
        v->u24 = ((((uint8_t*)tlv->value)[0]<<16)&0x00ff0000) |
               ((((uint8_t*)tlv->value)[1]<< 8)&0x0000ff00) |
               ((((uint8_t*)tlv->value)[2]    )&0x000000ff);
        break;
    }
    case OGS_TLV_UINT32:
    case OGS_TLV_INT32:
    {
        ogs_tlv_uint32_t *v = (ogs_tlv_uint32_t *)msg;

        if (tlv->length != 4)
        {
            ogs_error("Invalid TLV length %d. It should be 4", tlv->length);
            return OGS_ERROR;
        }
        v->u32 = ((((uint8_t*)tlv->value)[0]<<24)&0xff000000) |
               ((((uint8_t*)tlv->value)[1]<<16)&0x00ff0000) |
               ((((uint8_t*)tlv->value)[2]<< 8)&0x0000ff00) |
               ((((uint8_t*)tlv->value)[3]    )&0x000000ff);
        break;
    }
    case OGS_TLV_FIXED_STR:
    {
        ogs_tlv_octet_t *v = (ogs_tlv_octet_t *)msg;

        if (tlv->length != desc->length)
        {
            ogs_error("Invalid TLV length %d. It should be %d",
                    tlv->length, desc->length);
            return OGS_ERROR;
        }

        v->data = tlv->value;
        v->len = tlv->length;
        break;
    }
    case OGS_TLV_VAR_STR:
    {
        ogs_tlv_octet_t *v = (ogs_tlv_octet_t *)msg;

        v->data = tlv->value;
        v->len = tlv->length;
        break;
    }
    case OGS_TLV_NULL:
    {
        if (tlv->length != 0) {
            ogs_error("Invalid TLV length %d. It should be 0", tlv->length);
            return OGS_ERROR;
        }
        break;
    }
    default:
        ogs_assert_if_reached();
        break;
    }

    return OGS_OK;
}

static int tlv_parse_compound(void *msg, ogs_tlv_desc_t *parent_desc, 
        ogs_tlv_t *parent_tlv, int depth, int mode)
{
    int rv;
    ogs_tlv_presence_t *presence_p = (ogs_tlv_presence_t *)msg;
    ogs_tlv_desc_t *desc = NULL, *next_desc = NULL;
    ogs_tlv_t *tlv = NULL, *emb_tlv = NULL;
    uint8_t *p = msg;
    uint32_t offset = 0;
    uint8_t index = 0;
    int i = 0, j;
    char indent[17] = "                "; /* 16 spaces */

    ogs_assert(msg);
    ogs_assert(parent_desc);
    ogs_assert(parent_tlv);

    ogs_assert(depth <= 8);
    indent[depth*2] = 0;

    tlv = parent_tlv;
    while (tlv) {
        desc = tlv_find_desc(&index, &offset, parent_desc, tlv);
        if (desc == NULL) {
            ogs_error("Unexpected TLV type:%d", tlv->type);
            return OGS_ERROR;
        }

        presence_p = (ogs_tlv_presence_t *)(p + offset);

        /* Multiple of the same type TLV may be included */
        next_desc = parent_desc->child_descs[index+1];
        if (next_desc != NULL && next_desc->ctype == OGS_TLV_MORE) {
            for (j = 0; j < next_desc->length; j++) {
                presence_p = 
                    (ogs_tlv_presence_t *)(p + offset + desc->vsize * j);
                if (*presence_p == 0) {
                    offset += desc->vsize * j;
                    break;
                }
            }
            if (j == next_desc->length) {
                ogs_fatal("Multiple of the same type TLV need more room");
                ogs_assert_if_reached();
                tlv = tlv->next;
                continue;
            }
        }

        if (desc->ctype == OGS_TLV_COMPOUND) {
            emb_tlv = ogs_tlv_parse_embedded_block(tlv, mode);
            if (emb_tlv == NULL) {
                ogs_error("Error while parse TLV");
                return OGS_ERROR;
            }

            ogs_trace("PARSE %sC#%d [%s] T:%d I:%d (vsz=%d) off:%p ",
                    indent, i++, desc->name, desc->type, desc->instance, 
                    desc->vsize, p + offset);

            offset += sizeof(ogs_tlv_presence_t);

            rv = tlv_parse_compound(
                    p + offset, desc, emb_tlv, depth + 1, mode);
            if (rv != OGS_OK) {
                ogs_error("Can't parse compound TLV");
                return OGS_ERROR;
            }

            *presence_p = 1;
        } else {
            ogs_trace("PARSE %sL#%d [%s] T:%d L:%d I:%d "
                    "(cls:%d vsz:%d) off:%p ",
                    indent, i++, desc->name, desc->type, desc->length, 
                    desc->instance, desc->ctype, desc->vsize, p + offset);

            rv = tlv_parse_leaf(p + offset, desc, tlv);
            if (rv != OGS_OK) {
                ogs_error("Can't parse leaf TLV");
                return OGS_ERROR;
            }

            *presence_p = 1;
        }

        tlv = tlv->next;
    }

    return OGS_OK;
}

int ogs_tlv_parse_msg(void *msg, ogs_tlv_desc_t *desc, ogs_pkbuf_t *pkbuf,
        int mode)
{
    int rv;
    ogs_tlv_t *root;

    ogs_assert(msg);
    ogs_assert(desc);
    ogs_assert(pkbuf);

    ogs_assert(desc->ctype == OGS_TLV_MESSAGE);
    ogs_assert(desc->child_descs[0]);

    root = ogs_tlv_parse_block(pkbuf->len, pkbuf->data, mode);
    if (root == NULL) {
        ogs_error("Can't parse TLV message");
        return OGS_ERROR;
    }

    rv = tlv_parse_compound(msg, desc, root, 0, mode);

    ogs_tlv_free_all(root);

    return rv;
}
