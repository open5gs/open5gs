/*
 * Copyright (c) 2003-2017 Lev Walkin <vlm@lionet.info>. All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <constr_CHOICE.h>

asn_TYPE_operation_t asn_OP_CHOICE = {
    CHOICE_free,
#if !defined(ASN_DISABLE_PRINT_SUPPORT)
    CHOICE_print,
#else
    0,
#endif  /* !defined(ASN_DISABLE_PRINT_SUPPORT) */
    CHOICE_compare,
#if !defined(ASN_DISABLE_BER_SUPPORT)
    CHOICE_decode_ber,
    CHOICE_encode_der,
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_BER_SUPPORT) */
#if !defined(ASN_DISABLE_XER_SUPPORT)
    CHOICE_decode_xer,
    CHOICE_encode_xer,
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_XER_SUPPORT) */
#if !defined(ASN_DISABLE_JER_SUPPORT)
    CHOICE_encode_jer,
#else
    0,
#endif  /* !defined(ASN_DISABLE_JER_SUPPORT) */
#if !defined(ASN_DISABLE_OER_SUPPORT)
    CHOICE_decode_oer,
    CHOICE_encode_oer,
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT)
    CHOICE_decode_uper,
    CHOICE_encode_uper,
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) */
#if !defined(ASN_DISABLE_APER_SUPPORT)
    CHOICE_decode_aper,
    CHOICE_encode_aper,
#else
    0,
    0,
#endif  /* !defined(ASN_DISABLE_APER_SUPPORT) */
#if !defined(ASN_DISABLE_RFILL_SUPPORT)
    CHOICE_random_fill,
#else
    0,
#endif  /* !defined(ASN_DISABLE_RFILL_SUPPORT) */
    CHOICE_outmost_tag
};

ber_tlv_tag_t
CHOICE_outmost_tag(const asn_TYPE_descriptor_t *td, const void *ptr, int tag_mode, ber_tlv_tag_t tag) {
    const asn_CHOICE_specifics_t *specs = (const asn_CHOICE_specifics_t *)td->specifics;
    unsigned present;

    assert(tag_mode == 0); (void)tag_mode;
    assert(tag == 0); (void)tag;

    /*
     * Figure out which CHOICE element is encoded.
     */
    present = _fetch_present_idx(ptr, specs->pres_offset, specs->pres_size);

    if(present > 0 && present <= td->elements_count) {
        const asn_TYPE_member_t *elm = &td->elements[present-1];
        const void *memb_ptr;

        if(elm->flags & ATF_POINTER) {
            memb_ptr = *(const void * const *)
                    ((const char *)ptr + elm->memb_offset);
        } else {
            memb_ptr = (const void *)
                    ((const char *)ptr + elm->memb_offset);
        }

        return asn_TYPE_outmost_tag(elm->type, memb_ptr,
                                    elm->tag_mode, elm->tag);
    } else {
        return (ber_tlv_tag_t)-1;
    }
}

/*
 * See the definitions.
 */
static const void *_get_member_ptr(const asn_TYPE_descriptor_t *,
                                   const void *sptr, asn_TYPE_member_t **elm,
                                   unsigned *present);

int
CHOICE_constraint(const asn_TYPE_descriptor_t *td, const void *sptr,
                  asn_app_constraint_failed_f *ctfailcb, void *app_key) {
    const asn_CHOICE_specifics_t *specs =
        (const asn_CHOICE_specifics_t *)td->specifics;
    unsigned present;

	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}

	/*
	 * Figure out which CHOICE element is encoded.
	 */
	present = _fetch_present_idx(sptr, specs->pres_offset,specs->pres_size);
	if(present > 0 && present <= td->elements_count) {
		asn_TYPE_member_t *elm = &td->elements[present-1];
		const void *memb_ptr;

		if(elm->flags & ATF_POINTER) {
			memb_ptr = *(const void * const *)((const char *)sptr + elm->memb_offset);
			if(!memb_ptr) {
				if(elm->optional)
					return 0;
				ASN__CTFAIL(app_key, td, sptr,
					"%s: mandatory CHOICE element %s absent (%s:%d)",
					td->name, elm->name, __FILE__, __LINE__);
				return -1;
			}
		} else {
			memb_ptr = (const void *)((const char *)sptr + elm->memb_offset);
		}

		if(elm->encoding_constraints.general_constraints) {
			return elm->encoding_constraints.general_constraints(elm->type, memb_ptr,
				ctfailcb, app_key);
		} else {
			return elm->type->encoding_constraints.general_constraints(elm->type,
					memb_ptr, ctfailcb, app_key);
		}
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: no CHOICE element given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

void
CHOICE_free(const asn_TYPE_descriptor_t *td, void *ptr,
            enum asn_struct_free_method method) {
    const asn_CHOICE_specifics_t *specs =
        (const asn_CHOICE_specifics_t *)td->specifics;
    unsigned present;

	if(!td || !ptr)
		return;

	ASN_DEBUG("Freeing %s as CHOICE", td->name);

	/*
	 * Figure out which CHOICE element is encoded.
	 */
	present = _fetch_present_idx(ptr, specs->pres_offset, specs->pres_size);

	/*
	 * Free that element.
	 */
	if(present > 0 && present <= td->elements_count) {
		asn_TYPE_member_t *elm = &td->elements[present-1];
		void *memb_ptr;

		if(elm->flags & ATF_POINTER) {
			memb_ptr = *(void **)((char *)ptr + elm->memb_offset);
			if(memb_ptr)
				ASN_STRUCT_FREE(*elm->type, memb_ptr);
		} else {
			memb_ptr = (void *)((char *)ptr + elm->memb_offset);
			ASN_STRUCT_FREE_CONTENTS_ONLY(*elm->type, memb_ptr);
		}
	}

    switch(method) {
    case ASFM_FREE_EVERYTHING:
        FREEMEM(ptr);
        break;
    case ASFM_FREE_UNDERLYING:
        break;
    case ASFM_FREE_UNDERLYING_AND_RESET:
        memset(ptr, 0, specs->struct_size);
        break;
    }
}


/*
 * The following functions functions offer protection against -fshort-enums,
 * compatible with little- and big-endian machines.
 * If assertion is triggered, either disable -fshort-enums, or add an entry
 * here with the ->pres_size of your target stracture.
 * Unless the target structure is packed, the ".present" member
 * is guaranteed to be aligned properly. ASN.1 compiler itself does not
 * produce packed code.
 */
unsigned
_fetch_present_idx(const void *struct_ptr, unsigned pres_offset,
                   unsigned pres_size) {
    const void *present_ptr;
	unsigned present;

	present_ptr = ((const char *)struct_ptr) + pres_offset;

	switch(pres_size) {
	case sizeof(int):	present = *(const unsigned int *)present_ptr; break;
	case sizeof(short):	present = *(const unsigned short *)present_ptr; break;
	case sizeof(char):	present = *(const unsigned char *)present_ptr; break;
	default:
		/* ANSI C mandates enum to be equivalent to integer */
		assert(pres_size != sizeof(int));
		return 0;	/* If not aborted, pass back safe value */
	}

	return present;
}

void
_set_present_idx(void *struct_ptr, unsigned pres_offset, unsigned pres_size,
                 unsigned present) {
    void *present_ptr;
	present_ptr = ((char *)struct_ptr) + pres_offset;

	switch(pres_size) {
	case sizeof(int):	*(unsigned int *)present_ptr   = present; break;
	case sizeof(short):	*(unsigned short *)present_ptr = present; break;
	case sizeof(char):	*(unsigned char *)present_ptr  = present; break;
	default:
		/* ANSI C mandates enum to be equivalent to integer */
		assert(pres_size != sizeof(int));
	}
}

static const void *
_get_member_ptr(const asn_TYPE_descriptor_t *td, const void *sptr,
                asn_TYPE_member_t **elm_ptr, unsigned *present_out) {
    const asn_CHOICE_specifics_t *specs =
        (const asn_CHOICE_specifics_t *)td->specifics;
    unsigned present;

    if(!sptr) {
        *elm_ptr = NULL;
        *present_out = 0;
        return NULL;
    }

    /*
	 * Figure out which CHOICE element is encoded.
	 */
	present = _fetch_present_idx(sptr, specs->pres_offset, specs->pres_size);
    *present_out = present;

    /*
     * The presence index is intentionally 1-based to avoid
     * treating zeroed structure as a valid one.
     */
	if(present > 0 && present <= td->elements_count) {
        asn_TYPE_member_t *const elm = &td->elements[present - 1];
        const void *memb_ptr;

		if(elm->flags & ATF_POINTER) {
            memb_ptr =
                *(const void *const *)((const char *)sptr + elm->memb_offset);
        } else {
            memb_ptr = (const void *)((const char *)sptr + elm->memb_offset);
        }
        *elm_ptr = elm;
        return memb_ptr;
    } else {
        *elm_ptr = NULL;
        return NULL;
    }

}

int
CHOICE_compare(const asn_TYPE_descriptor_t *td, const void *aptr, const void *bptr) {
    asn_TYPE_member_t *aelm;
    asn_TYPE_member_t *belm;
    unsigned apresent = 0;
    unsigned bpresent = 0;
    const void *amember = _get_member_ptr(td, aptr, &aelm, &apresent);
    const void *bmember = _get_member_ptr(td, bptr, &belm, &bpresent);

    if(amember && bmember) {
        if(apresent == bpresent) {
            assert(aelm == belm);
            return aelm->type->op->compare_struct(aelm->type, amember, bmember);
        } else if(apresent < bpresent) {
            return -1;
        } else {
            return 1;
        }
    } else if(!amember) {
        return -1;
    } else {
        return 1;
    }
}

/*
 * Return the 1-based choice variant presence index.
 * Returns 0 in case of error.
 */
unsigned
CHOICE_variant_get_presence(const asn_TYPE_descriptor_t *td, const void *sptr) {
    const asn_CHOICE_specifics_t *specs =
        (const asn_CHOICE_specifics_t *)td->specifics;
    return _fetch_present_idx(sptr, specs->pres_offset, specs->pres_size);
}

/*
 * Sets or resets the 1-based choice variant presence index.
 * In case a previous index is not zero, the currently selected structure
 * member is freed and zeroed-out first.
 * Returns 0 on success and -1 on error.
 */
int
CHOICE_variant_set_presence(const asn_TYPE_descriptor_t *td, void *sptr,
                            unsigned present) {
    const asn_CHOICE_specifics_t *specs =
        (const asn_CHOICE_specifics_t *)td->specifics;
    unsigned old_present;

    if(!sptr) {
        return -1;
    }

    if(present > td->elements_count)
        return -1;

    old_present =
        _fetch_present_idx(sptr, specs->pres_offset, specs->pres_size);
    if(present == old_present)
        return 0;

    if(old_present != 0) {
        assert(old_present <= td->elements_count);
        ASN_STRUCT_RESET(*td, sptr);
    }

    _set_present_idx(sptr, specs->pres_offset, specs->pres_size, present);

    return 0;
}
