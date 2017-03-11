#define TRACE_MODULE _asn_msg
#include "core_debug.h"
#include "core_tlv.h"

#include "asn_hdr.h"
#include "asn_msg.h"

#include "netinet/in.h"

#include "asn_tlv_desc.c"
#include "asn_msg_desc.c"

asnt_desc_t asnt_more = 
{
    ASNC_MORE, 0, ASNL_MORE, 0, { NULL }
};

static tlv_t* _asn_add_leaf(tlv_t *parent_tlv, tlv_t *tlv, asnt_desc_t *desc,
        void *asnv)
{
    switch (desc->ctype)
    {
        case ASNC_UINT8:
        case ASNC_INT8:
        {
            asnv_uint8_t *v = (asnv_uint8_t *)asnv;

            d_trace(1, "V_1B:%02x", v->v);

            if (parent_tlv)
                tlv = tlv_embed(parent_tlv, desc->type, 1, 0, (c_uint8_t*)&v->v);
            else
                tlv = tlv_add(tlv, desc->type, 1, 0, (c_uint8_t*)&v->v);
            d_assert(tlv, return NULL, "Can't add TLV");
            break;
        }
        case ASNC_UINT16:
        {
            asnv_uint16_t *v = (asnv_uint16_t *)asnv;

            d_trace(1, "V_2B:%04x", v->v);

            v->v = htons(v->v);

            if (parent_tlv)
                tlv = tlv_embed(parent_tlv, desc->type, 2, 0, (c_uint8_t*)&v->v);
            else
                tlv = tlv_add(tlv, desc->type, 2, 0, (c_uint8_t*)&v->v);
            d_assert(tlv, return NULL, "Can't add TLV");
            break;
        }
        case ASNC_UINT24:
        case ASNC_INT24:
        {
            asnv_uint24_t *v = (asnv_uint24_t *)asnv;

            d_trace(1, "V_3B:%06x", v->v);

            v->v = v->v << 8;
            v->v = htonl(v->v);

            if (parent_tlv)
                tlv = tlv_embed(parent_tlv, desc->type, 3, 0, (c_uint8_t*)&v->v);
            else
                tlv = tlv_add(tlv, desc->type, 3, 0, (c_uint8_t*)&v->v);
            d_assert(tlv, return NULL, "Can't add TLV");
            break;
        }
        case ASNC_UINT32:
        case ASNC_INT32:
        {
            asnv_uint32_t *v = (asnv_uint32_t *)asnv;

            d_trace(1, "V_4B:%08x", v->v);

            v->v = htonl(v->v);

            if (parent_tlv)
                tlv = tlv_embed(parent_tlv, desc->type, 4, 0, (c_uint8_t*)&v->v);
            else
                tlv = tlv_add(tlv, desc->type, 4, 0, (c_uint8_t*)&v->v);
            d_assert(tlv, return NULL, "Can't add TLV");
            break;
        }
        case ASNC_FIXED_STR:
        {
            asnv_octets_t *v = (asnv_octets_t *)asnv;

            d_trace(1, "V_FSTR: ", v->v);
            d_trace_hex(1, v->v, v->l);

            if (parent_tlv)
                tlv = tlv_embed(parent_tlv, desc->type, desc->length, 0, v->v);
            else
                tlv = tlv_add(tlv, desc->type, desc->length, 0, v->v);
            d_assert(tlv, return NULL, "Can't add TLV");
            break;
        }
        case ASNC_VAR_STR:
        {
            asnv_octets_t *v = (asnv_octets_t *)asnv;

            d_assert(v->l > 0, return NULL, "Length is zero");

            d_trace(1, "V_VSTR: ", v->v);
            d_trace_hex(1, v->v, v->l);

            if (parent_tlv)
                tlv = tlv_embed(parent_tlv, desc->type, v->l, 0, v->v);
            else
                tlv = tlv_add(tlv, desc->type, v->l, 0, v->v);
            d_assert(tlv, return NULL, "Can't add TLV");
            break;
        }
        case ASNC_NULL:
        {
            d_trace(1, "V_NULL" );

            if (parent_tlv)
                tlv = tlv_embed(parent_tlv, desc->type, 0, 0, NULL);
            else
                tlv = tlv_add(tlv, desc->type, 0, 0, NULL);
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

static c_uint32_t _asn_add_compound(tlv_t **root, tlv_t *parent_tlv,
        asnt_desc_t *parent_desc, void *asnv, int depth)
{
    asnv_header_t *h;
    asnt_desc_t *desc = NULL, *next_desc = NULL;
    tlv_t *tlv = NULL, *emb_tlv = NULL;
    c_uint8_t *p = asnv;
    c_uint32_t offset = 0, count = 0;
    int i, j, r;
    char indent[17] = "                "; /* 16 spaces */

    d_assert(root, return 0, "Null param");
    d_assert(parent_desc, return 0, "Null param");
    d_assert(asnv, return 0, "Null param");

    d_assert(depth <= 8, return 0, "Too deep recursion");
    indent[depth*2] = 0;

    *root = NULL;

    for (i = 0, desc = parent_desc->child_descs[i]; desc != NULL;
            i++, desc = parent_desc->child_descs[i])
    {
        next_desc = parent_desc->child_descs[i+1];
        if (next_desc != NULL && next_desc->ctype == ASNC_MORE)
        {
            int offset2 = offset;
            for (j = 0; j < next_desc->length; j++)
            {
                h = (asnv_header_t *)(p + offset2);

                if (h->set_ind == 0)
                    break;

                if (desc->ctype == ASNC_COMPOUND)
                {
                    d_trace(1, "\nBUILD %sC#%d T:%d (vsz=%d) off:%p ",
                            indent, i, desc->type, desc->vsize, p + offset);

                    if (parent_tlv)
                        tlv = tlv_embed(parent_tlv, desc->type, 0, 0, NULL);
                    else
                        tlv = tlv_add(tlv, desc->type, 0, 0, NULL);

                    r = _asn_add_compound(&emb_tlv, tlv, desc,
                            p + offset + sizeof(asnv_header_t), depth + 1);
                    d_assert(r > 0 && emb_tlv, return 0,
                            "Can't build compound TLV");
                    count += 1 + r;
                }
                else
                {
                    d_trace(1, "\nBUILD %sL#%d T:%d L:%d (cls:%d vsz:%d) off:%p ",
                            indent, i, desc->type, desc->length,
                            desc->ctype, desc->vsize, p + offset);

                    tlv = _asn_add_leaf(parent_tlv, tlv, desc, p + offset);
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
            h = (asnv_header_t *)(p + offset);

            if (h->set_ind)
            {
                if (desc->ctype == ASNC_COMPOUND)
                {
                    d_trace(1, "\nBUILD %sC#%d T:%d (vsz=%d) off:%p ",
                            indent, i, desc->type, desc->vsize, p + offset);

                    if (parent_tlv)
                        tlv = tlv_embed(parent_tlv, desc->type, 0, 0, NULL);
                    else
                        tlv = tlv_add(tlv, desc->type, 0, 0, NULL);

                    r = _asn_add_compound(&emb_tlv, tlv, desc,
                            p + offset + sizeof(asnv_header_t), depth + 1);
                    d_assert(r > 0 && emb_tlv, return 0,
                            "Can't build compound TLV");
                    count += 1 + r;
                }
                else
                {
                    d_trace(1, "\nBUILD %sL#%d T:%d L:%d (cls:%d vsz:%d) off:%p ",
                            indent, i, desc->type, desc->length,
                            desc->ctype, desc->vsize, p + offset);

                    tlv = _asn_add_leaf(parent_tlv, tlv, desc, p + offset);
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

status_t asn_build_msg(pkbuf_t **msg, asn_msg_desc_t *msg_desc, void *asnv)
{
    tlv_t *root = NULL;
    c_uint32_t r, length, rendlen;

    d_assert(msg, return CORE_ERROR, "Null param");
    d_assert(msg_desc, return CORE_ERROR, "Null param");
    d_assert(asnv, return CORE_ERROR, "Null param");

    d_assert(msg_desc->ctype == ASNC_MESSAGE, return CORE_ERROR,
            "Not ASN message descriptor");
    d_assert(msg_desc->child_descs[0], return CORE_ERROR,
            "ASN message descriptor has no members");

    r = _asn_add_compound(&root, NULL, msg_desc, asnv, 0);
    d_assert(r > 0 && root, tlv_free_all(root); return CORE_ERROR,
            "Can't build ASN message");

    length = tlv_calc_length(root, TLV_MODE_T1_L2_I1);
    *msg = pkbuf_alloc(ASN_HEADER_LEN, length);
    d_assert(*msg, tlv_free_all(root); return CORE_ENOMEM,
            "pkbuf_alloc() failed");
    (*msg)->len = length;

    rendlen = tlv_render(root, (*msg)->payload, length, TLV_MODE_T1_L2_I1);
    d_assert(rendlen == length, tlv_free_all(root); return CORE_ERROR,
            "Error while render TLV (%d != %d)", length, rendlen);

    tlv_free_all(root);

    return CORE_OK;
}

static asnt_desc_t* _asn_find_desc(c_uint8_t *asnt_index,
        c_uint32_t *asnv_offset, asnt_desc_t *parent_desc, tlv_t *tlv)
{
    asnt_desc_t *prev_desc = NULL, *desc = NULL;
    int i, offset = 0;

    d_assert(parent_desc, return NULL, "Null param");
    d_assert(tlv, return NULL, "Null param");

    d_trace(5, "_asn_find_desc:%d - ", tlv->type);

    for (i = 0, desc = parent_desc->child_descs[i]; desc != NULL;
            i++, desc = parent_desc->child_descs[i])
    {
        d_trace(5, "%d, ", desc->type);

        if (desc->type == tlv->type)
        {
            *asnt_index = i;
            *asnv_offset = offset;
            break;
        }

        if (desc->ctype == ASNC_MORE)
        {
            d_assert(prev_desc && prev_desc->ctype != ASNC_MORE,
                    return NULL, "Invalid previous asnt_desc_t");
            offset += prev_desc->vsize * (desc->length - 1);
        }
        else
        {
            offset += desc->vsize;
        }

        prev_desc = desc;
    }

    d_trace(5, "\n");
    
    return desc;
}

static status_t _asn_parse_leaf(
        void *asnv, asnt_desc_t *desc, tlv_t *tlv)
{
    d_assert(asnv, return CORE_ERROR, "Null param");
    d_assert(desc, return CORE_ERROR, "Null param");
    d_assert(tlv, return CORE_ERROR, "Null param");

    switch (desc->ctype)
    {
        case ASNC_UINT8:
        case ASNC_INT8:
        {
            asnv_uint8_t *v = (asnv_uint8_t *)asnv;

            if (tlv->length != 1)
            {
                d_error("Invalid TLV length %d. It should be 1", tlv->length);
                return CORE_ERROR;
            }
            v->v = *(c_uint8_t*)(tlv->value);
            d_trace(1, "V_1B:%02x", v->v);
            break;
        }
        case ASNC_UINT16:
        case ASNC_INT16:
        {
            asnv_uint16_t *v = (asnv_uint16_t *)asnv;

            if (tlv->length != 2)
            {
                d_error("Invalid TLV length %d. It should be 2", tlv->length);
                return CORE_ERROR;
            }
            v->v = ((((c_uint8_t*)tlv->value)[0]<< 8)&0xff00) |
                   ((((c_uint8_t*)tlv->value)[1]    )&0x00ff);
            d_trace(1, "V_2B:%02x", v->v);
            break;
        }
        case ASNC_UINT24:
        case ASNC_INT24:
        {
            asnv_uint24_t *v = (asnv_uint24_t *)asnv;

            if (tlv->length != 3)
            {
                d_error("Invalid TLV length %d. It should be 3", tlv->length);
                return CORE_ERROR;
            }
            v->v = ((((c_uint8_t*)tlv->value)[0]<<16)&0x00ff0000) |
                   ((((c_uint8_t*)tlv->value)[1]<< 8)&0x0000ff00) |
                   ((((c_uint8_t*)tlv->value)[2]    )&0x000000ff);
            d_trace(1, "V_3B:%06x", v->v);
            break;
        }
        case ASNC_UINT32:
        case ASNC_INT32:
        {
            asnv_uint32_t *v = (asnv_uint32_t *)asnv;

            if (tlv->length != 4)
            {
                d_error("Invalid TLV length %d. It should be 4", tlv->length);
                return CORE_ERROR;
            }
            v->v = ((((c_uint8_t*)tlv->value)[0]<<24)&0xff000000) |
                   ((((c_uint8_t*)tlv->value)[1]<<16)&0x00ff0000) |
                   ((((c_uint8_t*)tlv->value)[2]<< 8)&0x0000ff00) |
                   ((((c_uint8_t*)tlv->value)[3]    )&0x000000ff);
            d_trace(1, "V_4B:%08x", v->v);
            break;
        }
        case ASNC_FIXED_STR:
        {
            asnv_octets_t *v = (asnv_octets_t *)asnv;

            if (tlv->length != desc->length)
            {
                d_error("Invalid TLV length %d. It should be %d",
                        tlv->length, desc->length);
                return CORE_ERROR;
            }

            v->v = tlv->value;
            v->l = tlv->length;

            d_trace(1, "V_FSTR: ", v->v);
            d_trace_hex(1, v->v, v->l);
            break;
        }
        case ASNC_VAR_STR:
        {
            asnv_octets_t *v = (asnv_octets_t *)asnv;

            v->v = tlv->value;
            v->l = tlv->length;

            d_trace(1, "V_VSTR: ", v->v);
            d_trace_hex(1, v->v, v->l);
            break;
        }
        case ASNC_NULL:
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

static status_t _asn_parse_compound(
        void *asnv, asnt_desc_t *parent_desc, tlv_t *parent_tlv, int depth)
{
    status_t rv;
    asnv_header_t *h = (asnv_header_t *)asnv;
    asnt_desc_t *desc = NULL, *next_desc = NULL;
    tlv_t *tlv = NULL, *emb_tlv = NULL;
    c_uint8_t *p = asnv;
    c_uint32_t offset = 0;
    c_uint8_t index = 0;
    int i = 0, j;
    char indent[17] = "                "; /* 16 spaces */

    d_assert(asnv, return CORE_ERROR, "Null param");
    d_assert(parent_desc, return CORE_ERROR, "Null param");
    d_assert(parent_tlv, return CORE_ERROR, "Null param");

    d_assert(depth <= 8, return 0, "Too deep recursion");
    indent[depth*2] = 0;

    tlv = parent_tlv;
    while(tlv)
    {
        desc = _asn_find_desc(&index, &offset, parent_desc, tlv);
        if (desc == NULL)
        {
            d_error("Unexpected TLV type:%d", tlv->type);
            return CORE_ERROR;
        }

        h = (asnv_header_t *)(p + offset);

        /* Multiple of the same type TLV may be included */
        next_desc = parent_desc->child_descs[index+1];
        if (next_desc != NULL && next_desc->ctype == ASNC_MORE)
        {
            for (j = 0; j < next_desc->length; j++)
            {
                h = (asnv_header_t *)(p + offset + desc->vsize * j);
                if (h->set_ind == 0)
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

        if (desc->ctype == ASNC_COMPOUND)
        {
            emb_tlv = tlv_parse_embedded_block(tlv, TLV_MODE_T1_L2_I1);
            if (emb_tlv == NULL)
            {
                d_error("Error while parse TLV");
                return CORE_ERROR;
            }

            d_trace(1, "\nPARSE %sC#%d T:%d (vsz=%d) off:%p ",
                    indent, i++, desc->type, desc->vsize, p + offset);

            offset += sizeof(asnv_header_t);

            rv = _asn_parse_compound(p + offset, desc, emb_tlv, depth + 1);
            if (rv != CORE_OK)
            {
                d_error("Can't parse compound TLV");
                return CORE_ERROR;
            }

            h->set_ind = 1;
        }
        else
        {
            d_trace(1, "\nPARSE %sL#%d T:%d L:%d (cls:%d vsz:%d) off:%p ",
                    indent, i++, desc->type, desc->length,
                    desc->ctype, desc->vsize, p + offset);

            rv = _asn_parse_leaf(p + offset, desc, tlv);
            if (rv != CORE_OK)
            {
                d_error("Can't parse leaf TLV");
                return CORE_ERROR;
            }

            h->set_ind = 1;
        }

        tlv = tlv->next;
    }
    d_trace(1, "\n");

    return CORE_OK;
}

status_t asn_parse_msg(void *asnv, asn_msg_desc_t *msg_desc, pkbuf_t *msg)
{
    status_t rv;
    tlv_t *root;

    d_assert(asnv, return CORE_ERROR, "Null param");
    d_assert(msg_desc, return CORE_ERROR, "Null param");
    d_assert(msg, return CORE_ERROR, "Null param");

    d_assert(msg_desc->ctype == ASNC_MESSAGE, return CORE_ERROR,
            "Not ASN message descriptor");
    d_assert(msg_desc->child_descs[0], return CORE_ERROR,
            "ASN message descriptor has no members");

    root = tlv_parse_block(msg->len, msg->payload, TLV_MODE_T1_L2_I1);
    if (root == NULL)
    {
        d_error("Can't parse ASN message");
        return CORE_ERROR;
    }

    rv = _asn_parse_compound(asnv, msg_desc, root, 0);

    tlv_free_all(root);

    return rv;
}
