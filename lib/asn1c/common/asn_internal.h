/*
 * Copyright (c) 2003-2017 Lev Walkin <vlm@lionet.info>. All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
/*
 * Declarations internally useful for the ASN.1 support code.
 */
#ifndef	ASN_INTERNAL_H
#define	ASN_INTERNAL_H
#ifndef __EXTENSIONS__
#define __EXTENSIONS__          /* for Sun */
#endif

typedef enum asn_type_kind {
    ASN_KIND_PRIMITIVE,
    ASN_KIND_SEQUENCE,
    ASN_KIND_SET,
    ASN_KIND_CHOICE,
    ASN_KIND_SEQUENCE_OF,
    ASN_KIND_SET_OF,
} asn_type_kind_t;

#include "asn_application.h"	/* Application-visible API */

#ifndef	__NO_ASSERT_H__		/* Include assert.h only for internal use. */
#include <assert.h>		/* for assert() macro */
#endif

#ifdef	__cplusplus
extern "C" {
#endif

#if !defined(ASN_DISABLE_UPER_SUPPORT)
#include <uper_decoder.h>
#include <uper_encoder.h>
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) */
#if !defined(ASN_DISABLE_APER_SUPPORT)
#include <aper_decoder.h>
#include <aper_encoder.h>
#endif  /* !defined(ASN_DISABLE_APER_SUPPORT) */

/* Environment version might be used to avoid running with the old library */
#define	ASN1C_ENVIRONMENT_VERSION	923	/* Compile-time version */
int get_asn1c_environment_version(void);	/* Run-time version */

#if 0 /* modified by acetcom */
#define	CALLOC(nmemb, size)	calloc(nmemb, size)
#define	MALLOC(size)		malloc(size)
#define	REALLOC(oldptr, size)	realloc(oldptr, size)
#define	FREEMEM(ptr)		free(ptr)
#else
#include "proto/ogs-proto.h"

static ogs_inline void *ogs_asn_malloc(size_t size, const char *file_line)
{
    void *ptr = ogs_malloc(size);
    if (!ptr) {
        ogs_fatal("asn_malloc() failed in `%s`", file_line);
        ogs_assert_if_reached();
    }

    return ptr;
}
static ogs_inline void *ogs_asn_calloc(
        size_t nmemb, size_t size, const char *file_line)
{
    void *ptr = ogs_calloc(nmemb, size);
    if (!ptr) {
        ogs_fatal("asn_calloc() failed in `%s`", file_line);
        ogs_assert_if_reached();
    }

    return ptr;
}
static ogs_inline void *ogs_asn_realloc(
        void *oldptr, size_t size, const char *file_line)
{ 
    void *ptr = ogs_realloc(oldptr, size);
    if (!ptr) {
        ogs_fatal("asn_realloc() failed in `%s`", file_line);
        ogs_assert_if_reached();
    }

    return ptr;
}

#define CALLOC(nmemb, size) ogs_asn_calloc(nmemb, size, OGS_FILE_LINE)
#define MALLOC(size) ogs_asn_malloc(size, OGS_FILE_LINE)
#define REALLOC(oldptr, size) ogs_asn_realloc(oldptr, size, OGS_FILE_LINE)
#define FREEMEM(ptr) ogs_free(ptr)

#endif

#define	asn_debug_indent	0
#define ASN_DEBUG_INDENT_ADD(i) do{}while(0)

#ifdef  EMIT_ASN_DEBUG
#warning "Use ASN_EMIT_DEBUG instead of EMIT_ASN_DEBUG"
#define ASN_EMIT_DEBUG  EMIT_ASN_DEBUG
#endif

/*
 * A macro for debugging the ASN.1 internals.
 * You may enable or override it.
 */
#ifndef	ASN_DEBUG	/* If debugging code is not defined elsewhere... */
#if	ASN_EMIT_DEBUG == 1	/* And it was asked to emit this code... */
#if __STDC_VERSION__ >= 199901L
#ifdef	ASN_THREAD_SAFE
/* Thread safety requires sacrifice in output indentation:
 * Retain empty definition of ASN_DEBUG_INDENT_ADD. */
#else	/* !ASN_THREAD_SAFE */
#undef  ASN_DEBUG_INDENT_ADD
#undef  asn_debug_indent
extern int asn_debug_indent;
#define ASN_DEBUG_INDENT_ADD(i) do { asn_debug_indent += i; } while(0)
#endif	/* ASN_THREAD_SAFE */
#define	ASN_DEBUG(fmt, args...)	do {			\
		int adi = asn_debug_indent;		\
		while(adi--) fprintf(stderr, " ");	\
		fprintf(stderr, fmt, ##args);		\
		fprintf(stderr, " (%s:%d)\n",		\
			__FILE__, __LINE__);		\
	} while(0)
#else	/* !C99 */
void CC_PRINTFLIKE(1, 2) ASN_DEBUG_f(const char *fmt, ...);
#define	ASN_DEBUG	ASN_DEBUG_f
#endif	/* C99 */
#else	/* ASN_EMIT_DEBUG != 1 */
#if __STDC_VERSION__ >= 199901L
#define ASN_DEBUG(...) do{}while(0)
#else   /* not C99 */
static void CC_PRINTFLIKE(1, 2) ASN_DEBUG(const char *fmt, ...) { (void)fmt; }
#endif  /* C99 or better */
#endif	/* ASN_EMIT_DEBUG */
#endif	/* ASN_DEBUG */

/*
 * Print to a callback.
 * The callback is expected to return negative values on error.
 * 0 and positive values are treated as success.
 * RETURN VALUES:
 *  -1: Failed to format or invoke the callback.
 *  >0: Size of the data that got delivered to the callback.
 */
ssize_t CC_PRINTFLIKE(3, 4)
asn__format_to_callback(
    int (*callback)(const void *, size_t, void *key), void *key,
    const char *fmt, ...);

/*
 * Invoke the application-supplied callback and fail, if something is wrong.
 */
#define ASN__E_cbc(buf, size) (cb((buf), (size), app_key) < 0)
#define ASN__E_CALLBACK(size, foo) \
    do {                           \
        if(foo) goto cb_failed;    \
        er.encoded += (size);      \
    } while(0)
#define ASN__CALLBACK(buf, size) ASN__E_CALLBACK(size, ASN__E_cbc(buf, size))
#define ASN__CALLBACK2(buf1, size1, buf2, size2) \
    ASN__E_CALLBACK((size1) + (size2),           \
                    ASN__E_cbc(buf1, size1) || ASN__E_cbc(buf2, size2))
#define ASN__CALLBACK3(buf1, size1, buf2, size2, buf3, size3)          \
    ASN__E_CALLBACK((size1) + (size2) + (size3),                       \
                    ASN__E_cbc(buf1, size1) || ASN__E_cbc(buf2, size2) \
                        || ASN__E_cbc(buf3, size3))

#define ASN__TEXT_INDENT(nl, level)                                          \
    do {                                                                     \
        int tmp_level = (level);                                             \
        int tmp_nl = ((nl) != 0);                                            \
        int tmp_i;                                                           \
        if(tmp_nl) ASN__CALLBACK("\n", 1);                                   \
        if(tmp_level < 0) tmp_level = 0;                                     \
        for(tmp_i = 0; tmp_i < tmp_level; tmp_i++) ASN__CALLBACK("    ", 4); \
    } while(0)

/*
 * Check if an ASN.1 type is a structured type that outputs newlines in XER.
 * Structured types (SEQUENCE, SET, CHOICE, SEQUENCE_OF, SET_OF) output 
 * multi-line XER content, so their closing tags need indentation.
 * Primitive types output inline content, so no indentation is needed.
 */
#define ASN__IS_STRUCTURED_TYPE(elm) \
    ((elm)->type->op->kind == ASN_KIND_SEQUENCE ||    \
     (elm)->type->op->kind == ASN_KIND_SET ||         \
     (elm)->type->op->kind == ASN_KIND_CHOICE ||      \
     (elm)->type->op->kind == ASN_KIND_SEQUENCE_OF || \
     (elm)->type->op->kind == ASN_KIND_SET_OF)

#define	_i_INDENT(nl)	do {                        \
        int tmp_i;                                  \
        if((nl) && cb("\n", 1, app_key) < 0)        \
            return -1;                              \
        for(tmp_i = 0; tmp_i < ilevel; tmp_i++)     \
            if(cb("    ", 4, app_key) < 0)          \
                return -1;                          \
    } while(0)

/*
 * Check stack against overflow, if limit is set.
 */

/* Since GCC 13, AddressSanitizer started defaulting to
* ASAN_OPTIONS="detect_stack_use_after_return=1", which makes this check
* fail due to apparently jumping stack pointers.
* Hence, disable this check if building with ASan, as documented in:
* GCC: https://gcc.gnu.org/onlinedocs/cpp/Common-Predefined-Macros.html
* Clang: https://clang.llvm.org/docs/AddressSanitizer.html#conditional-compilation-with-has-feature-address-sanitizer
*/
#if defined(__SANITIZE_ADDRESS__)
	#define ASN__SANITIZE_ENABLED 1
#elif defined(__has_feature)
#if __has_feature(address_sanitizer)
	#define ASN__SANITIZE_ENABLED 1
#endif
#endif

#define	ASN__DEFAULT_STACK_MAX	(30000)

/*
 * Recursion depth limit for encoding/decoding to prevent stack overflow
 * from circular references in ASN.1 structures. Can be overridden at
 * compile time with -DASN_STACK_OVERFLOW_LIMIT=<value>
 */
#ifndef ASN_STACK_OVERFLOW_LIMIT
#define ASN_STACK_OVERFLOW_LIMIT 30
#endif

#if defined(ASN__SANITIZE_ENABLED) || defined(ASN_DISABLE_STACK_OVERFLOW_CHECK)
static int CC_NOTUSED
ASN__STACK_OVERFLOW_CHECK(const asn_codec_ctx_t *ctx) {
   (void)ctx;
   return 0;
}
#else
static int CC_NOTUSED
ASN__STACK_OVERFLOW_CHECK(const asn_codec_ctx_t *ctx) {
	if(ctx && ctx->max_stack_size) {

		/* ctx MUST be allocated on the stack */
		ptrdiff_t usedstack = ((const char *)ctx - (const char *)&ctx);
		if(usedstack > 0) usedstack = -usedstack; /* grows up! */

		/* double negative required to avoid int wrap-around */
		if(usedstack < -(ptrdiff_t)ctx->max_stack_size) {
			ASN_DEBUG("Stack limit %ld reached",
				(long)ctx->max_stack_size);
			return -1;
		}
	}
	return 0;
}
#endif

/*
* Decoder recursion/stack depth tracking.
 * Use ASN__STACK_OVERFLOW_CHECK() to detect when the decoding context
 * has exhausted the allowed stack space and abort decoding to prevent
 * stack overflow from deep or circular references.
 */
#define ASN__DECODER_RECURSION_DEPTH_CHECK(ctx) \
    do { \
        if(ASN__STACK_OVERFLOW_CHECK((ctx)) != 0) { \
            ASN_DEBUG("Decoding stack limit reached"); \
        } \
    } while(0)

/*
 * Encoder recursion depth tracking using thread-local storage.
 * For encoders, we use a thread-local counter since they don't have ctx.
 * Separate counters per format allow independent depth tracking and
 * better debugging (know which format hit the limit).
 */
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L && !defined(__STDC_NO_THREADS__)
/* C11 thread support */
#include <threads.h>
extern thread_local int asn1_encoding_depth;  /* BER/DER */
extern thread_local int uper_encoding_depth;  /* UPER */
extern thread_local int aper_encoding_depth;  /* APER */
extern thread_local int oer_encoding_depth;   /* OER */
extern thread_local int xer_encoding_depth;   /* XER */
extern thread_local int jer_encoding_depth;   /* JER */
#elif defined(__GNUC__) || defined(__clang__)
/* GCC/Clang thread-local extension */
extern __thread int asn1_encoding_depth;  /* BER/DER */
extern __thread int uper_encoding_depth;  /* UPER */
extern __thread int aper_encoding_depth;  /* APER */
extern __thread int oer_encoding_depth;   /* OER */
extern __thread int xer_encoding_depth;   /* XER */
extern __thread int jer_encoding_depth;   /* JER */
#elif defined(_MSC_VER)
/* MSVC thread-local */
extern __declspec(thread) int asn1_encoding_depth;  /* BER/DER */
extern __declspec(thread) int uper_encoding_depth;  /* UPER */
extern __declspec(thread) int aper_encoding_depth;  /* APER */
extern __declspec(thread) int oer_encoding_depth;   /* OER */
extern __declspec(thread) int xer_encoding_depth;   /* XER */
extern __declspec(thread) int jer_encoding_depth;   /* JER */
#else
/* No thread-local support, use regular variable (not thread-safe) */
extern int asn1_encoding_depth;  /* BER/DER */
extern int uper_encoding_depth;  /* UPER */
extern int aper_encoding_depth;  /* APER */
extern int oer_encoding_depth;   /* OER */
extern int xer_encoding_depth;   /* XER */
extern int jer_encoding_depth;   /* JER */
#endif

/* BER/DER encoder depth tracking (already implemented) */
#define ASN__ENCODER_RECURSION_DEPTH_INC() \
    do { \
        if(asn1_encoding_depth >= ASN_STACK_OVERFLOW_LIMIT) { \
            ASN_DEBUG("Encoding recursion depth limit exceeded"); \
            ASN__ENCODE_FAILED; \
        } \
        asn1_encoding_depth++; \
    } while(0)

#define ASN__ENCODER_RECURSION_DEPTH_DEC() \
    do { \
        if(asn1_encoding_depth > 0) { \
            asn1_encoding_depth--; \
        } else { \
            ASN_DEBUG("BER/DER encoding depth underflow detected"); \
        } \
    } while(0)

/* UPER encoder depth tracking */
#define UPER_ENCODER_RECURSION_DEPTH_INC() \
    do { \
        if(uper_encoding_depth >= ASN_STACK_OVERFLOW_LIMIT) { \
            ASN_DEBUG("UPER encoding recursion depth limit exceeded"); \
            ASN__ENCODE_FAILED; \
        } \
        uper_encoding_depth++; \
    } while(0)

#define UPER_ENCODER_RECURSION_DEPTH_DEC() \
    do { \
        if(uper_encoding_depth > 0) { \
            uper_encoding_depth--; \
        } else { \
            ASN_DEBUG("UPER encoding depth underflow detected"); \
        } \
    } while(0)

/* APER encoder depth tracking */
#define APER_ENCODER_RECURSION_DEPTH_INC() \
    do { \
        if(aper_encoding_depth >= ASN_STACK_OVERFLOW_LIMIT) { \
            ASN_DEBUG("APER encoding recursion depth limit exceeded"); \
            ASN__ENCODE_FAILED; \
        } \
        aper_encoding_depth++; \
    } while(0)

#define APER_ENCODER_RECURSION_DEPTH_DEC() \
    do { \
        if(aper_encoding_depth > 0) { \
            aper_encoding_depth--; \
        } else { \
            ASN_DEBUG("APER encoding depth underflow detected"); \
        } \
    } while(0)

/* OER encoder depth tracking */
#define OER_ENCODER_RECURSION_DEPTH_INC() \
    do { \
        if(oer_encoding_depth >= ASN_STACK_OVERFLOW_LIMIT) { \
            ASN_DEBUG("OER encoding recursion depth limit exceeded"); \
            ASN__ENCODE_FAILED; \
        } \
        oer_encoding_depth++; \
    } while(0)

#define OER_ENCODER_RECURSION_DEPTH_DEC() \
    do { \
        if(oer_encoding_depth > 0) { \
            oer_encoding_depth--; \
        } else { \
            ASN_DEBUG("OER encoding depth underflow detected"); \
        } \
    } while(0)

/* XER encoder depth tracking */
#define XER_ENCODER_RECURSION_DEPTH_INC() \
    do { \
        if(xer_encoding_depth >= ASN_STACK_OVERFLOW_LIMIT) { \
            ASN_DEBUG("XER encoding recursion depth limit exceeded"); \
            ASN__ENCODE_FAILED; \
        } \
        xer_encoding_depth++; \
    } while(0)

#define XER_ENCODER_RECURSION_DEPTH_DEC() \
    do { \
        if(xer_encoding_depth > 0) { \
            xer_encoding_depth--; \
        } else { \
            ASN_DEBUG("XER encoding depth underflow detected"); \
        } \
    } while(0)

/* JER encoder depth tracking */
#define JER_ENCODER_RECURSION_DEPTH_INC() \
    do { \
        if(jer_encoding_depth >= ASN_STACK_OVERFLOW_LIMIT) { \
            ASN_DEBUG("JER encoding recursion depth limit exceeded"); \
            ASN__ENCODE_FAILED; \
        } \
        jer_encoding_depth++; \
    } while(0)

#define JER_ENCODER_RECURSION_DEPTH_DEC() \
    do { \
        if(jer_encoding_depth > 0) { \
            jer_encoding_depth--; \
        } else { \
            ASN_DEBUG("JER encoding depth underflow detected"); \
        } \
    } while(0)

/*
 * XER decoder support for empty OPTIONAL fields.
 * When enabled, the XER decoder treats empty tags (e.g., <field></field> or <field/>)
 * in OPTIONAL fields as absent rather than invalid.
 * Define XER_ALLOW_EMPTY_OPTIONALS at compile time to enable this feature:
 *   -DXER_ALLOW_EMPTY_OPTIONALS
 */
#ifdef XER_ALLOW_EMPTY_OPTIONALS
#define XER_EMPTY_OPTIONALS_ENABLED 1
#else
#define XER_EMPTY_OPTIONALS_ENABLED 0
#endif

/**
 * Check if the given name is an ASN.1 meta-syntax keyword that should
 * not be output as an XML wrapper tag (e.g., "SEQUENCE OF", "SET OF").
 * 
 * Returns 1 if the name is a meta-syntax keyword, 0 otherwise.
 */
static inline int
asn_is_meta_syntax_keyword(const char *name) {
    if(!name) return 0;
    
    size_t len = strlen(name);
    
    /* Check for "SEQUENCE OF", "SEQUENCE-OF", "SET OF", "SET-OF" */
    /* Match if keyword is at start, followed by anything except lowercase letter */
    if((len >= 11 && strncmp(name, "SEQUENCE OF", 11) == 0 &&
        (len == 11 || !(name[11] >= 'a' && name[11] <= 'z'))) ||
       (len >= 11 && strncmp(name, "SEQUENCE-OF", 11) == 0 &&
        (len == 11 || !(name[11] >= 'a' && name[11] <= 'z'))) ||
       (len >= 6 && strncmp(name, "SET OF", 6) == 0 &&
        (len == 6 || !(name[6] >= 'a' && name[6] <= 'z'))) ||
       (len >= 6 && strncmp(name, "SET-OF", 6) == 0 &&
        (len == 6 || !(name[6] >= 'a' && name[6] <= 'z')))) {
        return 1;
    }
    
    return 0;
}

#ifdef	__cplusplus
}
#endif

#endif	/* ASN_INTERNAL_H */
