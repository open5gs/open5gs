/*
 * Copyright (c) 2017 Lev Walkin <vlm@lionet.info>.
 * All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#include <asn_internal.h>
#include <UTF8String.h>

/*
 * Biased function for randomizing UTF-8 sequences.
 */
static size_t
UTF8String__random_char(uint8_t *b, size_t size) {
    static const struct rnd_value {
        const char *value;
        size_t size;
    } values[] = {{"\0", 1},
                  {"\x01", 1},
                  {"\x7f", 1},
                  {"\xc2\xa2", 2},
                  {"\xe2\x82\xac", 3},
                  {"\xf0\x90\x8d\x88", 4},
                  {"\xf4\x8f\xbf\xbf", 4}};

    const struct rnd_value *v;
    size_t max_idx = 0;

    switch(size) {
    case 0:
        assert(size != 0);
        return 0;
    case 1:
        max_idx = 2;
        break;
    case 2:
        max_idx = 3;
        break;
    default:
    case 4:
        max_idx = sizeof(values) / sizeof(values[0]) - 1;
        break;
    }

    v = &values[asn_random_between(0, max_idx)];
    memcpy(b, v->value, v->size);
    return v->size;
}

asn_random_fill_result_t
UTF8String_random_fill(const asn_TYPE_descriptor_t *td, void **sptr,
                       const asn_encoding_constraints_t *constraints,
                       size_t max_length) {
    asn_random_fill_result_t result_ok = {ARFILL_OK, 1};
    asn_random_fill_result_t result_failed = {ARFILL_FAILED, 0};
    asn_random_fill_result_t result_skipped = {ARFILL_SKIPPED, 0};
    uint8_t *buf;
    uint8_t *bend;
    uint8_t *b;
    size_t rnd_len;
    size_t idx;
    UTF8String_t *st;

    if(max_length == 0 && !*sptr) return result_skipped;

    /* Figure out how far we should go */
    rnd_len = OCTET_STRING_random_length_constrained(td, constraints,
                                                     max_length / 4);

    buf = CALLOC(4, rnd_len + 1);
    if(!buf) return result_failed;

    bend = &buf[4 * rnd_len];

    for(b = buf, idx = 0; idx < rnd_len; idx++) {
        b += UTF8String__random_char(b, (bend - b));
    }
    *(uint8_t *)b = 0;

    if(*sptr) {
        st = *sptr;
        FREEMEM(st->buf);
    } else {
        st = (OCTET_STRING_t *)(*sptr = CALLOC(1, sizeof(UTF8String_t)));
        if(!st) {
            FREEMEM(buf);
            return result_failed;
        }
    }

    st->buf = buf;
    st->size = b - buf;

    assert(UTF8String_length(st) == (ssize_t)rnd_len);

    return result_ok;
}
