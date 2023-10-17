/*
 * Copyright (c) 2017 Lev Walkin <vlm@lionet.info>.
 * All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <constr_SEQUENCE.h>
#include <OPEN_TYPE.h>
#include <aper_opentype.h>

/*
 * Check whether we are inside the extensions group.
 */
#define IN_EXTENSION_GROUP(specs, memb_idx)                \
    ((specs)->first_extension >= 0                         \
     && (unsigned)(specs)->first_extension <= (memb_idx))

asn_dec_rval_t
SEQUENCE_decode_aper(const asn_codec_ctx_t *opt_codec_ctx,
                     const asn_TYPE_descriptor_t *td,
                     const asn_per_constraints_t *constraints, void **sptr, asn_per_data_t *pd) {
    const asn_SEQUENCE_specifics_t *specs = (const asn_SEQUENCE_specifics_t *)td->specifics;
    void *st = *sptr;  /* Target structure. */
    int extpresent;    /* Extension additions are present */
    uint8_t *opres;    /* Presence of optional root members */
    asn_per_data_t opmd;
    asn_dec_rval_t rv;
    size_t edx;

    (void)constraints;

    if(ASN__STACK_OVERFLOW_CHECK(opt_codec_ctx))
        ASN__DECODE_FAILED;

    if(!st) {
        st = *sptr = CALLOC(1, specs->struct_size);
        if(!st) ASN__DECODE_FAILED;
    }

    ASN_DEBUG("Decoding %s as SEQUENCE (APER)", td->name);

    /* Handle extensions */
    if(specs->first_extension < 0) {
        extpresent = 0;
    } else {
        extpresent = per_get_few_bits(pd, 1);
        if(extpresent < 0) ASN__DECODE_STARVED;
    }

    /* Prepare a place and read-in the presence bitmap */
    memset(&opmd, 0, sizeof(opmd));
    if(specs->roms_count) {
        opres = (uint8_t *)MALLOC(((specs->roms_count + 7) >> 3) + 1);
        if(!opres) ASN__DECODE_FAILED;
        /* Get the presence map */
        if(per_get_many_bits(pd, opres, 0, specs->roms_count)) {
            FREEMEM(opres);
            ASN__DECODE_STARVED;
        }
        opmd.buffer = opres;
        opmd.nbits = specs->roms_count;
        ASN_DEBUG("Read in presence bitmap for %s of %d bits (%x..)",
                  td->name, specs->roms_count, *opres);
    } else {
        opres = 0;
    }

    /*
     * Get the sequence ROOT elements.
     */
    for(edx = 0; edx < td->elements_count; edx++) {
        asn_TYPE_member_t *elm = &td->elements[edx];
        void *memb_ptr;    /* Pointer to the member */
        void **memb_ptr2;  /* Pointer to that pointer */
#if 0
        int padding;
#endif

        if(IN_EXTENSION_GROUP(specs, edx))
            continue;

        /* Fetch the pointer to this member */
        if(elm->flags & ATF_POINTER) {
            memb_ptr2 = (void **)((char *)st + elm->memb_offset);
        } else {
            memb_ptr = (char *)st + elm->memb_offset;
            memb_ptr2 = &memb_ptr;
        }
#if 0
        /* Get Padding */
        padding = (8 - (pd->moved % 8)) % 8;
        if(padding > 0)
            ASN_DEBUG("For element %s,offset= %ld Padding bits = %d", td->name, pd->moved, padding);
#if 0 /* old way of removing padding */
        per_get_few_bits(pd, padding);
#else /* Experimental fix proposed by @mhanna123 */
        if(edx != (td->elements_count-1))
            per_get_few_bits(pd, padding);
        else {
            if(specs->roms_count && (padding > 0))
                ASN_DEBUG(">>>>> not skipping padding of %d bits for element:%ld out of %d", padding, edx, td->elements_count);
            else
                per_get_few_bits(pd, padding);
        }
#endif /* dealing with padding */
#endif
        /* Deal with optionality */
        if(elm->optional) {
            int present = per_get_few_bits(&opmd, 1);
            ASN_DEBUG("Member %s->%s is optional, p=%d (%d->%d)",
                      td->name, elm->name, present,
                      (int)opmd.nboff, (int)opmd.nbits);
            if(present == 0) {
                /* This element is not present */
                if(elm->default_value_set) {
                    /* Fill-in DEFAULT */
                    if(elm->default_value_set(memb_ptr2)) {
                        FREEMEM(opres);
                        ASN__DECODE_FAILED;
                    }
                    ASN_DEBUG("Filled-in default");
                }
                /* The member is just not present */
                continue;
            }
            /* Fall through */
        }

        /* Fetch the member from the stream */
        ASN_DEBUG("Decoding member \"%s\" in %s", elm->name, td->name);

        if(elm->flags & ATF_OPEN_TYPE) {
            if (OPEN_TYPE_aper_is_unknown_type(td, st, elm)) {
                rv = OPEN_TYPE_aper_unknown_type_discard_bytes(pd);
                FREEMEM(opres);
                return rv;
            }
            rv = OPEN_TYPE_aper_get(opt_codec_ctx, td, st, elm, pd);
        } else {
            rv = elm->type->op->aper_decoder(opt_codec_ctx, elm->type,
                                             elm->encoding_constraints.per_constraints,
                                             memb_ptr2, pd);
        }
        if(rv.code != RC_OK) {
            ASN_DEBUG("Failed decode %s in %s",
                      elm->name, td->name);
            FREEMEM(opres);
            return rv;
        }
    }

    /* Optionality map is not needed anymore */
    FREEMEM(opres);

    /*
     * Deal with extensions.
     */
    if(extpresent) {
        ssize_t bmlength;
        uint8_t *epres;  /* Presence of extension members */
        asn_per_data_t epmd;

        bmlength = aper_get_nslength(pd);
        if(bmlength < 0) ASN__DECODE_STARVED;

        ASN_DEBUG("Extensions %" ASN_PRI_SSIZE " present in %s", bmlength, td->name);

        epres = (uint8_t *)MALLOC((bmlength + 15) >> 3);
        if(!epres) ASN__DECODE_STARVED;

        /* Get the extensions map */
        if(per_get_many_bits(pd, epres, 0, bmlength))
            ASN__DECODE_STARVED;

        memset(&epmd, 0, sizeof(epmd));
        epmd.buffer = epres;
        epmd.nbits = bmlength;
        ASN_DEBUG("Read in extensions bitmap for %s of %zd bits (%x..)",
                  td->name, bmlength, *epres);

        /* Deal with padding */
        if (aper_get_align(pd) < 0)
            ASN__DECODE_STARVED;

        /* Go over extensions and read them in */
        for(edx = specs->first_extension; edx < td->elements_count; edx++) {
            asn_TYPE_member_t *elm = &td->elements[edx];
            void *memb_ptr;    /* Pointer to the member */
            void **memb_ptr2;  /* Pointer to that pointer */
            int present;

            if(!IN_EXTENSION_GROUP(specs, edx)) {
                ASN_DEBUG("%zu is not extension", edx);
                continue;
            }

            /* Fetch the pointer to this member */
            if(elm->flags & ATF_POINTER) {
                memb_ptr2 = (void **)((char *)st + elm->memb_offset);
            } else {
                memb_ptr = (void *)((char *)st + elm->memb_offset);
                memb_ptr2 = &memb_ptr;
            }

            present = per_get_few_bits(&epmd, 1);
            if(present <= 0) {
                if(present < 0) break;  /* No more extensions */
                continue;
            }

            ASN_DEBUG("Decoding member %s in %s %p", elm->name, td->name, *memb_ptr2);
            rv = aper_open_type_get(opt_codec_ctx, elm->type,
                                    elm->encoding_constraints.per_constraints,
                                    memb_ptr2, pd);
            if(rv.code != RC_OK) {
                FREEMEM(epres);
                return rv;
            }
        }

        /* Skip over overflow extensions which aren't present
         * in this system's version of the protocol */
        for(;;) {
            ASN_DEBUG("Getting overflow extensions");
            switch(per_get_few_bits(&epmd, 1)) {
            case -1:
                break;
            case 0:
                continue;
            default:
                if(aper_open_type_skip(opt_codec_ctx, pd)) {
                    FREEMEM(epres);
                    ASN__DECODE_STARVED;
                }
            }
            break;
        }

        FREEMEM(epres);
    }

    /* Fill DEFAULT members in extensions */
    for(edx = specs->roms_count; edx < specs->roms_count
            + specs->aoms_count; edx++) {
        asn_TYPE_member_t *elm = &td->elements[edx];
        void **memb_ptr2;  /* Pointer to member pointer */

        if(!elm->default_value_set) continue;

        /* Fetch the pointer to this member */
        if(elm->flags & ATF_POINTER) {
            memb_ptr2 = (void **)((char *)st + elm->memb_offset);
            if(*memb_ptr2) continue;
        } else {
            continue;  /* Extensions are all optionals */
        }

        /* Set default value */
        if(elm->default_value_set(memb_ptr2)) {
            ASN__DECODE_FAILED;
        }
    }

    rv.consumed = 0;
    rv.code = RC_OK;
    return rv;
}

static int
SEQUENCE_handle_extensions_aper(const asn_TYPE_descriptor_t *td,
                                const void *sptr,
                                asn_per_outp_t *po1, asn_per_outp_t *po2) {
    const asn_SEQUENCE_specifics_t *specs
        = (const asn_SEQUENCE_specifics_t *)td->specifics;
    int exts_present = 0;
    int exts_count = 0;
    size_t edx;

    if(specs->first_extension < 0) {
        return 0;
    }

    /* Find out which extensions are present */
    for(edx = specs->first_extension; edx < td->elements_count; edx++) {
        asn_TYPE_member_t *elm = &td->elements[edx];
        const void *memb_ptr;           /* Pointer to the member */
        const void * const *memb_ptr2;  /* Pointer to that pointer */
        int present;

        if(!IN_EXTENSION_GROUP(specs, edx)) {
            ASN_DEBUG("%s (@%zu) is not extension", elm->type->name, edx);
            continue;
        }

        /* Fetch the pointer to this member */
        if(elm->flags & ATF_POINTER) {
            memb_ptr2 = (const void * const *)((const char *)sptr + elm->memb_offset);
            present = (*memb_ptr2 != 0);
        } else {
            memb_ptr = (const void *)((const char *)sptr + elm->memb_offset);
            memb_ptr2 = &memb_ptr;
            present = 1;
        }

        ASN_DEBUG("checking %s (@%zu) present => %d",
                  elm->type->name, edx, present);
        exts_count++;
        exts_present += present;

        /* Encode as presence marker */
        if(po1 && per_put_few_bits(po1, present, 1))
            return -1;
        /* Encode as open type field */
        if(po2 && present && aper_open_type_put(elm->type,
                                                elm->encoding_constraints.per_constraints,
                                                *memb_ptr2, po2))
            return -1;

    }

    return exts_present ? exts_count : 0;
}

asn_enc_rval_t
SEQUENCE_encode_aper(const asn_TYPE_descriptor_t *td,
                     const asn_per_constraints_t *constraints,
                     const void *sptr, asn_per_outp_t *po) {
    const asn_SEQUENCE_specifics_t *specs
        = (const asn_SEQUENCE_specifics_t *)td->specifics;
    asn_enc_rval_t er = {0,0,0};
    int n_extensions;
    size_t edx;
    size_t i;

    (void)constraints;

    if(!sptr)
        ASN__ENCODE_FAILED;

    er.encoded = 0;

    ASN_DEBUG("Encoding %s as SEQUENCE (APER)", td->name);

    /*
     * X.691#18.1 Whether structure is extensible
     * and whether to encode extensions
     */
    if(specs->first_extension < 0) {
        n_extensions = 0; /* There are no extensions to encode */
    } else {
        n_extensions = SEQUENCE_handle_extensions_aper(td, sptr, 0, 0);
        if(n_extensions < 0) ASN__ENCODE_FAILED;
        if(per_put_few_bits(po, n_extensions ? 1 : 0, 1)) {
            ASN__ENCODE_FAILED;
        }
    }

    /* Encode a presence bitmap */
    for(i = 0; i < specs->roms_count; i++) {
        asn_TYPE_member_t *elm;
        const void *memb_ptr;           /* Pointer to the member */
        const void * const *memb_ptr2;  /* Pointer to that pointer */
        int present;

        edx = specs->oms[i];
        elm = &td->elements[edx];

        /* Fetch the pointer to this member */
        if(elm->flags & ATF_POINTER) {
            memb_ptr2 = (const void * const *)((const char *)sptr + elm->memb_offset);
            present = (*memb_ptr2 != 0);
        } else {
            memb_ptr = (const void *)((const char *)sptr + elm->memb_offset);
            memb_ptr2 = &memb_ptr;
            present = 1;
        }

        /* Eliminate default values */
        if(present && elm->default_value_cmp
                && elm->default_value_cmp(*memb_ptr2) == 0)
            present = 0;

        ASN_DEBUG("Element %s %s %s->%s is %s",
                  elm->flags & ATF_POINTER ? "ptr" : "inline",
                  elm->default_value_cmp ? "def" : "wtv",
                  td->name, elm->name, present ? "present" : "absent");
        if(per_put_few_bits(po, present, 1))
            ASN__ENCODE_FAILED;
    }

    /*
     * Encode the sequence ROOT elements.
     */
    ASN_DEBUG("first_extension = %d, elements = %d", specs->first_extension,
              td->elements_count);
    for(edx = 0;
        edx < ((specs->first_extension < 0) ? td->elements_count
                                            : (size_t)specs->first_extension);
        edx++) {
        asn_TYPE_member_t *elm = &td->elements[edx];
        const void *memb_ptr;           /* Pointer to the member */
        const void * const *memb_ptr2;  /* Pointer to that pointer */

        if(IN_EXTENSION_GROUP(specs, edx))
            continue;

        ASN_DEBUG("About to encode %s", elm->type->name);

        /* Fetch the pointer to this member */
        if(elm->flags & ATF_POINTER) {
            memb_ptr2 = (const void * const *)((const char *)sptr + elm->memb_offset);
            if(!*memb_ptr2) {
                ASN_DEBUG("Element %s %zu not present",
                          elm->name, edx);
                if(elm->optional)
                    continue;
                /* Mandatory element is missing */
                ASN__ENCODE_FAILED;
            }
        } else {
            memb_ptr = (const void *)((const char *)sptr + elm->memb_offset);
            memb_ptr2 = &memb_ptr;
        }

        /* Eliminate default values */
        if(elm->default_value_cmp && elm->default_value_cmp(*memb_ptr2) == 0)
            continue;

        ASN_DEBUG("Encoding %s->%s", td->name, elm->name);
        er = elm->type->op->aper_encoder(elm->type,
                                         elm->encoding_constraints.per_constraints,
                                         *memb_ptr2, po);
        if(er.encoded == -1)
            return er;
    }

    /* No extensions to encode */
    if(!n_extensions) ASN__ENCODED_OK(er);

    ASN_DEBUG("Length of %d bit-map", n_extensions);
    /* #18.8. Write down the presence bit-map length. */
    if(aper_put_nslength(po, n_extensions))
        ASN__ENCODE_FAILED;

    ASN_DEBUG("Bit-map of %d elements", n_extensions);
    /* #18.7. Encoding the extensions presence bit-map. */
    /* TODO: act upon NOTE in #18.7 for canonical PER */
    if(SEQUENCE_handle_extensions_aper(td, sptr, po, 0) != n_extensions)
        ASN__ENCODE_FAILED;

    ASN_DEBUG("Writing %d extensions", n_extensions);
    /* #18.9. Encode extensions as open type fields. */
    if(SEQUENCE_handle_extensions_aper(td, sptr, 0, po) != n_extensions)
        ASN__ENCODE_FAILED;

    ASN__ENCODED_OK(er);
}
