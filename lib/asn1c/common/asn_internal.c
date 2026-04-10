#include <asn_internal.h>

#if ASN_EMIT_DEBUG == 1 && __STDC_VERSION__ >= 199901L && !defined(ASN_THREAD_SAFE)
int asn_debug_indent = 0;
#endif

/*
 * Thread-local encoding recursion depth counters for preventing stack overflow.
 * Separate counters per format allow independent depth tracking.
 */
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L && !defined(__STDC_NO_THREADS__)
/* C11 thread support */
thread_local int asn1_encoding_depth = 0;  /* BER/DER */
thread_local int uper_encoding_depth = 0;  /* UPER */
thread_local int aper_encoding_depth = 0;  /* APER */
thread_local int oer_encoding_depth = 0;   /* OER */
thread_local int xer_encoding_depth = 0;   /* XER */
thread_local int jer_encoding_depth = 0;   /* JER */
#elif defined(__GNUC__) || defined(__clang__)
/* GCC/Clang thread-local extension */
__thread int asn1_encoding_depth = 0;  /* BER/DER */
__thread int uper_encoding_depth = 0;  /* UPER */
__thread int aper_encoding_depth = 0;  /* APER */
__thread int oer_encoding_depth = 0;   /* OER */
__thread int xer_encoding_depth = 0;   /* XER */
__thread int jer_encoding_depth = 0;   /* JER */
#elif defined(_MSC_VER)
/* MSVC thread-local */
__declspec(thread) int asn1_encoding_depth = 0;  /* BER/DER */
__declspec(thread) int uper_encoding_depth = 0;  /* UPER */
__declspec(thread) int aper_encoding_depth = 0;  /* APER */
__declspec(thread) int oer_encoding_depth = 0;   /* OER */
__declspec(thread) int xer_encoding_depth = 0;   /* XER */
__declspec(thread) int jer_encoding_depth = 0;   /* JER */
#else
/* No thread-local support, use regular variable (not thread-safe) */
int asn1_encoding_depth = 0;  /* BER/DER */
int uper_encoding_depth = 0;  /* UPER */
int aper_encoding_depth = 0;  /* APER */
int oer_encoding_depth = 0;   /* OER */
int xer_encoding_depth = 0;   /* XER */
int jer_encoding_depth = 0;   /* JER */
#endif

ssize_t
asn__format_to_callback(int (*cb)(const void *, size_t, void *key), void *key,
                        const char *fmt, ...) {
    char scratch[64];
    char *buf = scratch;
    size_t buf_size = sizeof(scratch);
    int wrote;
    int cb_ret;

    do {
        va_list args;
        va_start(args, fmt);

        wrote = vsnprintf(buf, buf_size, fmt, args);
        va_end(args);
        if(wrote < (ssize_t)buf_size) {
            if(wrote < 0) {
                if(buf != scratch) FREEMEM(buf);
                return -1;
            }
            break;
        }

        buf_size <<= 1;
        if(buf == scratch) {
            buf = MALLOC(buf_size);
            if(!buf) {
              return -1;
            }
        } else {
            void *p = REALLOC(buf, buf_size);
            if(!p) {
                FREEMEM(buf);
                return -1;
            }
            buf = p;
        }
    } while(1);

    cb_ret = cb(buf, wrote, key);
    if(buf != scratch) FREEMEM(buf);
    if(cb_ret < 0) {
        return -1;
    }

    return wrote;
}

