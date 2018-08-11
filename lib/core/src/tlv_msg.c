#define TRACE_MODULE _tlv_msg

#include "core_debug.h"
#include "core_tlv_msg.h"

tlv_desc_t tlv_desc_more1 = { TLV_MORE, "More", 0, 1, 0, 0, { NULL } };
tlv_desc_t tlv_desc_more2 = { TLV_MORE, "More", 0, 2, 0, 0, { NULL } };
tlv_desc_t tlv_desc_more3 = { TLV_MORE, "More", 0, 3, 0, 0, { NULL } };
tlv_desc_t tlv_desc_more4 = { TLV_MORE, "More", 0, 4, 0, 0, { NULL } };
tlv_desc_t tlv_desc_more5 = { TLV_MORE, "More", 0, 5, 0, 0, { NULL } };
tlv_desc_t tlv_desc_more6 = { TLV_MORE, "More", 0, 6, 0, 0, { NULL } };
tlv_desc_t tlv_desc_more7 = { TLV_MORE, "More", 0, 7, 0, 0, { NULL } };
tlv_desc_t tlv_desc_more8 = { TLV_MORE, "More", 0, 8, 0, 0, { NULL } };

static tlv_t* _tlv_add_leaf(
        tlv_t *parent_tlv, tlv_t *tlv, tlv_desc_t *desc, void *msg)
{
    switch (desc->ctype)
    {
        case TLV_UINT8:
        case TLV_INT8:
        {
            tlv_uint8_t *v = (tlv_uint8_t *)msg;

            d_trace2(25, "V_1B:%02x", v->u8);

            if (parent_tlv)
                tlv = tlv_embed(parent_tlv, 
                        desc->type, 1, desc->instance, (c_uint8_t*)&v->u8);
            else
                tlv = tlv_add(tlv, 
                        desc->type, 1, desc->instance, (c_uint8_t*)&v->u8);
            d_assert(tlv, return NULL, "Can't add TLV");
            break;
        }
        case TLV_UINT16:
        {
            tlv_uint16_t *v = (tlv_uint16_t *)msg;

            d_trace2(25, "V_2B:%04x", v->u16);

            v->u16 = htons(v->u16);

            if (parent_tlv)
                tlv = tlv_embed(parent_tlv, 
                        desc->type, 2, desc->instance, (c_uint8_t*)&v->u16);
            else
                tlv = tlv_add(tlv, 
                        desc->type, 2, desc->instance, (c_uint8_t*)&v->u16);
            d_assert(tlv, return NULL, "Can't add TLV");
            break;
        }
        case TLV_UINT24:
        case TLV_INT24:
        {
            tlv_uint24_t *v = (tlv_uint24_t *)msg;

            d_trace2(25, "V_3B:%06x", v->u24);

            v->u24 = v->u24 << 8;
            v->u24 = htonl(v->u24);

            if (parent_tlv)
                tlv = tlv_embed(parent_tlv, 
                        desc->type, 3, desc->instance, (c_uint8_t*)&v->u24);
            else
                tlv = tlv_add(tlv, 
                        desc->type, 3, desc->instance, (c_uint8_t*)&v->u24);
            d_assert(tlv, return NULL, "Can't add TLV");
            break;
        }
        case TLV_UINT32:
        case TLV_INT32:
        {
            tlv_uint32_t *v = (tlv_uint32_t *)msg;

            d_trace2(25, "V_4B:%08x", v->u32);

            v->u32 = htonl(v->u32);

            if (parent_tlv)
                tlv = tlv_embed(parent_tlv,
                        desc->type, 4, desc->instance, (c_uint8_t*)&v->u32);
            else
                tlv = tlv_add(tlv,
                        desc->type, 4, desc->instance, (c_uint8_t*)&v->u32);
            d_assert(tlv, return NULL, "Can't add TLV");
            break;
        }
        case TLV_FIXED_STR:
        {
            tlv_octet_t *v = (tlv_octet_t *)msg;

            d_trace2(25, "V_FSTR: ", v->data);
            d_trace_hex(25, v->data, v->len);

            if (parent_tlv)
                tlv = tlv_embed(parent_tlv, 
                        desc->type, desc->length, desc->instance, v->data);
            else
                tlv = tlv_add(tlv, 
                        desc->type, desc->length, desc->instance, v->data);
            d_assert(tlv, return NULL, "Can't add TLV");
            break;
        }
        case TLV_VAR_STR:
        {
            tlv_octet_t *v = (tlv_octet_t *)msg;

            d_assert(v->len > 0, return NULL, "Length is zero");

            d_trace2(25, "V_VSTR: ", v->data);
            d_trace_hex(25, v->data, v->len);

            if (parent_tlv)
                tlv = tlv_embed(parent_tlv, 
                        desc->type, v->len, desc->instance, v->data);
            else
                tlv = tlv_add(tlv, 
                        desc->type, v->len, desc->instance, v->data);
            d_assert(tlv, return NULL, "Can't add TLV");
            break;
        }
        case TLV_NULL:
        {
            d_trace2(25, "V_NULL" );

            if (parent_tlv)
                tlv = tlv_embed(parent_tlv, 
                        desc->type, 0, desc->instance, NULL);
            else
                tlv = tlv_add(tlv, 
                        desc->type, 0, desc->instance, NULL);
            d_assert(tlv, return NULL, "Can't add TLV");
            break;
        }
        default:
        {
            d_assert(0, return NULL, "Unknown class %d", desc->ctype);
            break;
        }
    }

    return tlv;
}

static c_uint32_t _tlv_add_compound(tlv_t **root, tlv_t *parent_tlv,
        tlv_desc_t *parent_desc, void *msg, int depth)
{
    tlv_presence_t *presence_p;
    tlv_desc_t *desc = NULL, *next_desc = NULL;
    tlv_t *tlv = NULL, *emb_tlv = NULL;
    c_uint8_t *p = msg;
    c_uint32_t offset = 0, count = 0;
    int i, j, r;
    char indent[17] = "                "; /* 16 spaces */

    d_assert(root, return 0, "Null param");
    d_assert(parent_desc, return 0, "Null param");
    d_assert(msg, return 0, "Null param");

    d_assert(depth <= 8, return 0, "Too deep recursion");
    indent[depth*2] = 0;

    *root = NULL;

    for (i = 0, desc = parent_desc->child_descs[i]; desc != NULL;
            i++, desc = parent_desc->child_descs[i])
    {
        next_desc = parent_desc->child_descs[i+1];
        if (next_desc != NULL && next_desc->ctype == TLV_MORE)
        {
            int offset2 = offset;
            for (j = 0; j < next_desc->length; j++)
            {
                presence_p = (tlv_presence_t *)(p + offset2);

                if (*presence_p == 0)
                    break;

                if (desc->ctype == TLV_COMPOUND)
                {
                    d_trace2(25, "\nBUILD %sC#%d [%s] T:%d I:%d (vsz=%d) off:%p ",
                            indent, i, desc->name, desc->type, desc->instance, 
                            desc->vsize, p + offset2);

                    if (parent_tlv)
                        tlv = tlv_embed(parent_tlv, 
                                desc->type, 0, desc->instance, NULL);
                    else
                        tlv = tlv_add(tlv, desc->type, 0, desc->instance, NULL);

                    r = _tlv_add_compound(&emb_tlv, tlv, desc,
                            p + offset2 + sizeof(tlv_presence_t), depth + 1);
                    d_assert(r > 0 && emb_tlv, return 0,
                            "Can't build compound TLV");
                    count += 1 + r;
                }
                else
                {
                    d_trace2(25, "\nBUILD %sL#%d [%s] T:%d L:%d I:%d "
                            "(cls:%d vsz:%d) off:%p ",
                            indent, i, desc->name, desc->type, desc->length, 
                            desc->instance, desc->ctype, desc->vsize, 
                            p + offset2);

                    tlv = _tlv_add_leaf(parent_tlv, tlv, desc, p + offset2);
                    d_assert(tlv, return 0, "Can't build leaf TLV");
                    count++;
                }

                if (*root == NULL)
                    *root = tlv;

                offset2 += desc->vsize;
            }
            offset += desc->vsize * next_desc->length;
            i++;
        }
        else
        {
            presence_p = (tlv_presence_t *)(p + offset);

            if (*presence_p)
            {
                if (desc->ctype == TLV_COMPOUND)
                {
                    d_trace2(25, "\nBUILD %sC#%d [%s] T:%d I:%d (vsz=%d) off:%p ",
                            indent, i, desc->name, desc->type, desc->instance, 
                            desc->vsize, p + offset);

                    if (parent_tlv)
                        tlv = tlv_embed(parent_tlv, 
                                desc->type, 0, desc->instance, NULL);
                    else
                        tlv = tlv_add(tlv, desc->type, 0, desc->instance, NULL);

                    r = _tlv_add_compound(&emb_tlv, tlv, desc,
                            p + offset + sizeof(tlv_presence_t), depth + 1);
                    d_assert(r > 0 && emb_tlv, return 0,
                            "Can't build compound TLV");
                    count += 1 + r;
                }
                else
                {
                    d_trace2(25, "\nBUILD %sL#%d [%s] T:%d L:%d I:%d "
                            "(cls:%d vsz:%d) off:%p ",
                            indent, i, desc->name, desc->type, desc->length, 
                            desc->instance, desc->ctype, desc->vsize, 
                            p + offset);

                    tlv = _tlv_add_leaf(parent_tlv, tlv, desc, p + offset);
                    d_assert(tlv, return 0, "Can't build leaf TLV");
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

status_t tlv_build_msg(pkbuf_t **pkbuf, tlv_desc_t *desc, void *msg, int mode)
{
    tlv_t *root = NULL;
    c_uint32_t r, length, rendlen;

    d_assert(pkbuf, return CORE_ERROR, "Null param");
    d_assert(desc, return CORE_ERROR, "Null param");
    d_assert(msg, return CORE_ERROR, "Null param");

    d_assert(desc->ctype == TLV_MESSAGE, return CORE_ERROR,
            "Not TLV message descriptor");
    d_assert(desc->child_descs[0], return CORE_ERROR,
            "TLV message descriptor has no members");
    
    d_trace2(25, "\n");
    d_trace2(25, "[GTP] Build %s\n", desc->name);

    r = _tlv_add_compound(&root, NULL, desc, msg, 0);
    d_assert(r > 0 && root, tlv_free_all(root); return CORE_ERROR,
            "Can't build TLV message");

    length = tlv_calc_length(root, mode);
    *pkbuf = pkbuf_alloc(TLV_MAX_HEADROOM, length);
    d_assert(*pkbuf, tlv_free_all(root); return CORE_ENOMEM,
            "pkbuf_alloc() failed");
    (*pkbuf)->len = length;

    rendlen = tlv_render(root, (*pkbuf)->payload, length, mode);
    d_assert(rendlen == length, 
            pkbuf_free(*pkbuf); tlv_free_all(root); return CORE_ERROR,
            "Error while render TLV (%d != %d)", length, rendlen);

    tlv_free_all(root);

    return CORE_OK;
}

static tlv_desc_t* _tlv_find_desc(c_uint8_t *desc_index,
        c_uint32_t *tlv_offset, tlv_desc_t *parent_desc, tlv_t *tlv)
{
    tlv_desc_t *prev_desc = NULL, *desc = NULL;
    int i, offset = 0;

    d_assert(parent_desc, return NULL, "Null param");
    d_assert(tlv, return NULL, "Null param");

    d_trace2(25, "_tlv_find_desc:T(%d), I(%d) - ", tlv->type, tlv->instance);

    for (i = 0, desc = parent_desc->child_descs[i]; desc != NULL;
            i++, desc = parent_desc->child_descs[i])
    {
        d_trace2(25, "%d, ", desc->type);

        if (desc->type == tlv->type && desc->instance == tlv->instance)
        {
            *desc_index = i;
            *tlv_offset = offset;
            break;
        }

        if (desc->ctype == TLV_MORE)
        {
            d_assert(prev_desc && prev_desc->ctype != TLV_MORE,
                    return NULL, "Invalid previous tlv_desc_t");
            offset += prev_desc->vsize * (desc->length - 1);
        }
        else
        {
            offset += desc->vsize;
        }

        prev_desc = desc;
    }

    d_trace2(25, "\n");
    
    return desc;
}

static status_t _tlv_parse_leaf(void *msg, tlv_desc_t *desc, tlv_t *tlv)
{
    d_assert(msg, return CORE_ERROR, "Null param");
    d_assert(desc, return CORE_ERROR, "Null param");
    d_assert(tlv, return CORE_ERROR, "Null param");

    switch (desc->ctype)
    {
        case TLV_UINT8:
        case TLV_INT8:
        {
            tlv_uint8_t *v = (tlv_uint8_t *)msg;

            if (tlv->length != 1)
            {
                d_error("Invalid TLV length %d. It should be 1", tlv->length);
                return CORE_ERROR;
            }
            v->u8 = *(c_uint8_t*)(tlv->value);
            d_trace2(25, "V_1B:%02x", v->u8);
            break;
        }
        case TLV_UINT16:
        case TLV_INT16:
        {
            tlv_uint16_t *v = (tlv_uint16_t *)msg;

            if (tlv->length != 2)
            {
                d_error("Invalid TLV length %d. It should be 2", tlv->length);
                return CORE_ERROR;
            }
            v->u16 = ((((c_uint8_t*)tlv->value)[0]<< 8)&0xff00) |
                   ((((c_uint8_t*)tlv->value)[1]    )&0x00ff);
            d_trace2(25, "V_2B:%02x", v->u16);
            break;
        }
        case TLV_UINT24:
        case TLV_INT24:
        {
            tlv_uint24_t *v = (tlv_uint24_t *)msg;

            if (tlv->length != 3)
            {
                d_error("Invalid TLV length %d. It should be 3", tlv->length);
                return CORE_ERROR;
            }
            v->u24 = ((((c_uint8_t*)tlv->value)[0]<<16)&0x00ff0000) |
                   ((((c_uint8_t*)tlv->value)[1]<< 8)&0x0000ff00) |
                   ((((c_uint8_t*)tlv->value)[2]    )&0x000000ff);
            d_trace2(25, "V_3B:%06x", v->u24);
            break;
        }
        case TLV_UINT32:
        case TLV_INT32:
        {
            tlv_uint32_t *v = (tlv_uint32_t *)msg;

            if (tlv->length != 4)
            {
                d_error("Invalid TLV length %d. It should be 4", tlv->length);
                return CORE_ERROR;
            }
            v->u32 = ((((c_uint8_t*)tlv->value)[0]<<24)&0xff000000) |
                   ((((c_uint8_t*)tlv->value)[1]<<16)&0x00ff0000) |
                   ((((c_uint8_t*)tlv->value)[2]<< 8)&0x0000ff00) |
                   ((((c_uint8_t*)tlv->value)[3]    )&0x000000ff);
            d_trace2(25, "V_4B:%08x", v->u32);
            break;
        }
        case TLV_FIXED_STR:
        {
            tlv_octet_t *v = (tlv_octet_t *)msg;

            if (tlv->length != desc->length)
            {
                d_error("Invalid TLV length %d. It should be %d",
                        tlv->length, desc->length);
                return CORE_ERROR;
            }

            v->data = tlv->value;
            v->len = tlv->length;

            d_trace2(25, "V_FSTR: ", v->data);
            d_trace_hex(25, v->data, v->len);
            break;
        }
        case TLV_VAR_STR:
        {
            tlv_octet_t *v = (tlv_octet_t *)msg;

            v->data = tlv->value;
            v->len = tlv->length;

            d_trace2(25, "V_VSTR: ", v->data);
            d_trace_hex(25, v->data, v->len);
            break;
        }
        case TLV_NULL:
        {
            if (tlv->length != 0)
            {
                d_error("Invalid TLV length %d. It should be 0", tlv->length);
                return CORE_ERROR;
            }
            break;
        }
        default:
            d_assert(0, return CORE_ERROR, "Unknown TLV class");
            break;
    }

    return CORE_OK;
}

static status_t _tlv_parse_compound(void *msg, tlv_desc_t *parent_desc, 
        tlv_t *parent_tlv, int depth, int mode)
{
    status_t rv;
    tlv_presence_t *presence_p = (tlv_presence_t *)msg;
    tlv_desc_t *desc = NULL, *next_desc = NULL;
    tlv_t *tlv = NULL, *emb_tlv = NULL;
    c_uint8_t *p = msg;
    c_uint32_t offset = 0;
    c_uint8_t index = 0;
    int i = 0, j;
    char indent[17] = "                "; /* 16 spaces */

    d_assert(msg, return CORE_ERROR, "Null param");
    d_assert(parent_desc, return CORE_ERROR, "Null param");
    d_assert(parent_tlv, return CORE_ERROR, "Null param");

    d_assert(depth <= 8, return 0, "Too deep recursion");
    indent[depth*2] = 0;

    tlv = parent_tlv;
    while(tlv)
    {
        desc = _tlv_find_desc(&index, &offset, parent_desc, tlv);
        if (desc == NULL)
        {
            d_error("Unexpected TLV type:%d", tlv->type);
            return CORE_ERROR;
        }

        presence_p = (tlv_presence_t *)(p + offset);

        /* Multiple of the same type TLV may be included */
        next_desc = parent_desc->child_descs[index+1];
        if (next_desc != NULL && next_desc->ctype == TLV_MORE)
        {
            for (j = 0; j < next_desc->length; j++)
            {
                presence_p = (tlv_presence_t *)(p + offset + desc->vsize * j);
                if (*presence_p == 0)
                {
                    offset += desc->vsize * j;
                    break;
                }
            }
            if (j == next_desc->length)
            {
                d_fatal("Multiple of the same type TLV need more room");
                tlv = tlv->next;
                continue;
            }
        }

        if (desc->ctype == TLV_COMPOUND)
        {
            emb_tlv = tlv_parse_embedded_block(tlv, mode);
            if (emb_tlv == NULL)
            {
                d_error("Error while parse TLV");
                return CORE_ERROR;
            }

            d_trace2(25, "\nPARSE %sC#%d [%s] T:%d I:%d (vsz=%d) off:%p ",
                    indent, i++, desc->name, desc->type, desc->instance, 
                    desc->vsize, p + offset);

            offset += sizeof(tlv_presence_t);

            rv = _tlv_parse_compound(
                    p + offset, desc, emb_tlv, depth + 1, mode);
            if (rv != CORE_OK)
            {
                d_error("Can't parse compound TLV");
                return CORE_ERROR;
            }

            *presence_p = 1;
        }
        else
        {
            d_trace2(25, "\nPARSE %sL#%d [%s] T:%d L:%d I:%d "
                    "(cls:%d vsz:%d) off:%p ",
                    indent, i++, desc->name, desc->type, desc->length, 
                    desc->instance, desc->ctype, desc->vsize, p + offset);

            rv = _tlv_parse_leaf(p + offset, desc, tlv);
            if (rv != CORE_OK)
            {
                d_error("Can't parse leaf TLV");
                return CORE_ERROR;
            }

            *presence_p = 1;
        }

        tlv = tlv->next;
    }
    d_trace2(25, "\n");

    return CORE_OK;
}

status_t tlv_parse_msg(void *msg, tlv_desc_t *desc, pkbuf_t *pkbuf, int mode)
{
    status_t rv;
    tlv_t *root;

    d_assert(msg, return CORE_ERROR, "Null param");
    d_assert(desc, return CORE_ERROR, "Null param");
    d_assert(pkbuf, return CORE_ERROR, "Null param");

    d_assert(desc->ctype == TLV_MESSAGE, return CORE_ERROR,
            "Not TLV message descriptor");
    d_assert(desc->child_descs[0], return CORE_ERROR,
            "TLV message descriptor has no members");

    d_trace2(25, "\n");
    d_trace2(25, "[GTP] Parse %s\n", desc->name);

    root = tlv_parse_block(pkbuf->len, pkbuf->payload, mode);
    if (root == NULL)
    {
        d_error("Can't parse TLV message");
        return CORE_ERROR;
    }

    rv = _tlv_parse_compound(msg, desc, root, 0, mode);

    tlv_free_all(root);

    return rv;
}

