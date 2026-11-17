#!/bin/sh
#
# Regenerate the TS 32.298 CHF CDR ASN.1 encoder/decoder.
#
#   standard/  pristine 3GPP modules, exactly as published on 3GPP Forge
#   patched/   the only hand-written ASN.1: the records trimmed to 5G SA,
#              plus corrections for defects in the published module
#   closure.py standard + patched -> modified/  (a self-contained module)
#   modified/  the asn1c input, committed so review can read it
#   asn1c      modified/ -> lib/asn1c/cdr/
#
# Usage:
#   ASN1C=/path/to/asn1c/asn1c/asn1c ./generator.sh
#
# See ../README.md ("Regenerate the CHF CDR ASN.1 files") for how to build
# asn1c and which commit to use.
#
set -e

HERE=$(cd "$(dirname "$0")" && pwd)
OUT=$(cd "$HERE/../../cdr" 2>/dev/null && pwd) || OUT="$HERE/../../cdr"

ASN1C=${ASN1C:-asn1c}
if ! command -v "$ASN1C" > /dev/null 2>&1 && [ ! -x "$ASN1C" ]; then
    echo "error: asn1c not found. Set ASN1C=/path/to/asn1c/asn1c/asn1c" >&2
    exit 1
fi

MODULE=32298-j60-chf-5gsa.asn

# ---------------------------------------------------------------------
# 1. standard/ + patched/ -> modified/
#
# The CHF record reaches 353 types across seven sibling charging modules,
# TS 29.002 MAP, CAMEL CAP and ITU-T CMIP/ACSE/X.721.  Nothing on the 5G SA
# PDU-session path needs any of it, so closure.py keeps only what the
# trimmed records actually reach - 26 types, of which 21 are copied
# verbatim from standard/.
# ---------------------------------------------------------------------
echo "closure: standard/ + patched/ -> modified/$MODULE"
python3 "$HERE/closure.py" \
    --root CHFRecord \
    --module "$HERE/standard/TS32298_CHFChargingDataTypes.asn" \
             "$HERE/standard/TS32298_GenericChargingDataTypes.asn" \
             "$HERE/standard/TS32298_GPRSChargingDataTypes.asn" \
    --override "$HERE/patched/chf-5gsa-records.asn" \
               "$HERE/patched/spec-defects.asn" \
    --emit-module "$HERE/standard/TS32298_CHFChargingDataTypes.asn" \
    > "$HERE/modified/$MODULE"

# ---------------------------------------------------------------------
# 2. modified/ -> lib/asn1c/cdr/
#
# BER is the one encoding kept: TS 32.298 clause 6.1 requires it on Bx
# ("Data Record Format" 1).  asn1c's encoder emits DER, which is a
# canonical subset of BER, so any BER decoder reads what we write.
#
# NGAP and S1AP are generated with -no-gen-BER and keep APER instead; the
# CDR codec is the mirror image, so it cannot share their runtime.  See
# ../../cdr/meson.build.
#
# asn1c records the input path as given in every file's banner, so it is
# passed relative to $OUT, the way NGAP and S1AP pass theirs.
# ---------------------------------------------------------------------
echo "asn1c: modified/$MODULE -> $OUT"
mkdir -p "$OUT"
cd "$OUT"
"$ASN1C" -pdu=all \
    -fprefix=CHFCDR_ -fcompound-names -findirect-choice -fno-include-deps \
    -no-gen-XER -no-gen-JER -no-gen-CBOR \
    -no-gen-OER -no-gen-UPER -no-gen-APER \
    -no-gen-random-fill -no-gen-example \
    "../support/$(basename "$HERE")/modified/$MODULE"

# ---------------------------------------------------------------------
# 3. Drop the fragments asn1c leaves behind, and keep the build host out
#    of the banners
#
# ASN1C-UsefulInformationObjectClasses is built into asn1c, which finds it
# beside its own binary, so that banner names wherever asn1c was built.
# Keep only the path inside asn1c's tree.
# ---------------------------------------------------------------------
rm -f Makefile.am.asn1convert Makefile.am.libasncodec \
      converter-example.mk converter-example.c pdu_collection.c

for f in $(grep -l 'found in ".*/skeletons/standard-modules/' CHFCDR_*); do
    sed 's|found in ".*/skeletons/standard-modules/|found in "skeletons/standard-modules/|' \
        "$f" > "$f.tmp"
    mv "$f.tmp" "$f"
done

# ---------------------------------------------------------------------
# 4. Route allocation through ogs_malloc
#
# asn_internal.h is the one skeleton open5gs modifies (see ../README.md).
# lib/asn1c/common already carries the patched form of the same file, so
# we copy it rather than re-deriving the patch here, and keep the patch
# itself in one place.
#
# This directory deliberately keeps its own full copy of the asn1c
# runtime instead of sharing lib/asn1c/common.  asn_config.h is written
# by asn1c from the -no-gen-* flags above, so common's copy has BER off
# and this one has it on - and a C quote-include resolves against the
# including file's own directory first.  Sharing common's sources would
# therefore compile them against common's asn_config.h, giving the shared
# runtime and the generated code two different layouts for
# asn_TYPE_operation_s.  That mismatch links cleanly and crashes later,
# so the duplication buys a real guarantee.
# ---------------------------------------------------------------------
COMMON="$HERE/../../common"

if ! grep -q "modified by acetcom" "$COMMON/asn_internal.h" 2>/dev/null; then
    echo "error: $COMMON/asn_internal.h is not the open5gs-patched copy." >&2
    echo "       Re-check the allocator wrappers before trusting this build." >&2
    exit 1
fi
cp "$COMMON/asn_internal.h" "$OUT/asn_internal.h"

echo
echo "Generated $(ls "$OUT" | grep -c '\.c$') .c and $(ls "$OUT" | grep -c '\.h$') .h files."

# Everything else asn1c copied should be the stock skeleton. Point out
# any that drifted from common's, so a release bump does not slip past.
drifted=""
for f in "$OUT"/*.c "$OUT"/*.h; do
    b=$(basename "$f")
    case "$b" in CHFCDR_*|asn_config.h|asn_internal.h) continue ;; esac
    [ -f "$COMMON/$b" ] || continue
    cmp -s "$f" "$COMMON/$b" || drifted="$drifted $b"
done
if [ -n "$drifted" ]; then
    echo
    echo "These skeletons differ from lib/asn1c/common - check whether that is"
    echo "an asn1c version difference before committing:"
    for b in $drifted; do echo "  $b"; done
fi

echo
echo "Now sync the source list in $OUT/meson.build and the header list in"
echo "lib/asn1c/cdr/ogs-asn1c-cdr.h by hand - neither is generated."
