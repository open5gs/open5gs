Regenerate NGAP/S1AP/CHF-CDR ASN.1 files
========================================

This document describes how to regenerate the NGAP/S1AP ASN.1
encoder/decoder files for Open5GS using `asn1c`.

The generated files are based on the 3GPP ASN.1 specifications and the
`mouse07410/asn1c` `vlm_master` branch.

There is a third bundle, `cdr-r19.6.0/`, holding the TS 32.298 CHF CDR
records. It uses the same `asn1c` at the same commit, but its input is
assembled by a script rather than prepared by hand, and it is generated for
BER rather than APER. See "Regenerate the CHF CDR ASN.1 files" below; the
NGAP/S1AP steps in between do not apply to it.

Use mouse07410/vlm_master asn1c
===============================

Use the `vlm_master` branch from `mouse07410/asn1c`.

Target commit:

```text
b7926896e87934cb57bd3c53243a91ebd9fcec90
```

This commit includes the following merge:

```text
Merge pull request #498 from mouse07410/codex/apply-fprefix-anonymous-typedefs

[WIP] Fix apply -fprefix to anonymous typedef names
```

Clone and build `asn1c`:

```sh
cd ~/Documents/git/my

git clone https://github.com/mouse07410/asn1c.git
cd asn1c
git checkout b7926896e87934cb57bd3c53243a91ebd9fcec90

autoreconf -fi
./configure
make -j8
```

Alternatively, the Open5GS fork may be used:

```sh
cd ~/Documents/git/my

git clone https://github.com/open5gs/asn1c.git
cd asn1c

autoreconf -fi
./configure
make -j8
```

Prepare S1AP ASN.1 input file
=============================

Create the modified S1AP ASN.1 input file from the 3GPP text file.

```sh
cd ~/Documents/git/open5gs/lib/asn1c/support/s1ap-r19.1.0
```

The original 3GPP text file is:

```text
36413-j10.txt
```

The modified ASN.1 input file should be:

```text
36413-j10.asn
```

Check the difference between the original text file and the modified
ASN.1 file:

```sh
diff 36413-j10.txt 36413-j10.asn
```

Prepare NGAP ASN.1 input file
=============================

Create the modified NGAP ASN.1 input file from the 3GPP text file.

```sh
cd ~/Documents/git/open5gs/lib/asn1c/support/ngap-r19.2.0
```

The original 3GPP text file is:

```text
38413-j20.txt
```

The modified ASN.1 input file should be:

```text
38413-j20.asn
```

Check the difference between the original text file and the modified
ASN.1 file:

```sh
diff 38413-j20.txt 38413-j20.asn
```

Generate S1AP ASN.1 encoder/decoder
====================================

Generate the S1AP ASN.1 encoder/decoder files from the modified S1AP
ASN.1 file.

```sh
cd ~/Documents/git/open5gs/lib/asn1c/s1ap

../../../../my/asn1c/asn1c/asn1c -pdu=all \
    -fprefix=S1AP_ -fcompound-names -findirect-choice -fno-include-deps \
    -no-gen-BER -no-gen-XER -no-gen-JER -no-gen-CBOR \
    -no-gen-OER -no-gen-UPER \
    ../support/s1ap-r19.1.0/36413-j10.asn
```

Generate NGAP ASN.1 encoder/decoder
====================================

Generate the NGAP ASN.1 encoder/decoder files from the modified NGAP
ASN.1 file.

```sh
cd ~/Documents/git/open5gs/lib/asn1c/ngap

../../../../my/asn1c/asn1c/asn1c -pdu=all \
    -fprefix=NGAP_ -fcompound-names -findirect-choice -fno-include-deps \
    -no-gen-BER -no-gen-XER -no-gen-JER -no-gen-CBOR \
    -no-gen-OER -no-gen-UPER \
    ../support/ngap-r19.2.0/38413-j20.asn
```

Check common ASN.1 runtime file
===============================

After regenerating the files, check the common ASN.1 runtime header.

```sh
cd ~/Documents/git/open5gs/lib/asn1c/common

git diff asn_internal.h
```

Compare it with the upstream `asn1c` skeleton file if needed:

```sh
diff -u asn_internal.h ~/Documents/git/my/asn1c/skeletons/asn_internal.h
```

The allocator macros in `asn_internal.h` are intentionally modified for
Open5GS.

Keep the Open5GS-specific memory allocation wrappers:

```c
#include "proto/ogs-proto.h"

#define MALLOC(size) ogs_malloc(size)
#define CALLOC(nmemb, size) ogs_calloc((nmemb), (size))
#define REALLOC(oldptr, size) ogs_realloc((oldptr), (size))
#define FREEMEM(ptr) ogs_free(ptr)
```

Do not replace them with the default libc allocation functions:

```c
#define CALLOC(nmemb, size) calloc(nmemb, size)
#define MALLOC(size) malloc(size)
#define REALLOC(oldptr, size) realloc(oldptr, size)
#define FREEMEM(ptr) free(ptr)
```

Update generated ASN.1 source/header lists
==========================================

Update the NGAP/S1AP generated source and header lists using
`Makefile.am.libasncodec`.

The generated `Makefile.am.libasncodec` file contains the list of
generated `.c` and `.h` files.

First, remove trailing line-continuation backslashes from
`Makefile.am.libasncodec`.

In `vim`, use:

```vim
%s/\s*\\\s*$//g
```

Then update the Open5GS build and header files as follows.

Add generated `.c` files to the corresponding Meson build files:

```text
lib/asn1c/ngap/meson.build
lib/asn1c/s1ap/meson.build
```

Add generated `.h` files to the corresponding public ASN.1 header files:

```text
lib/ngap/ogs-ngap.h
lib/s1ap/ogs-s1ap.h
```

However, do not add the following unused generated files to
`meson.build`, `ogs-ngap.h`, or `ogs-s1ap.h`.

Exclude these S1AP files:

```text
S1AP_E-RAB-IE-ContainerPairList.c
S1AP_ProtocolError-IE-ContainerList.c
S1AP_ProtocolIE-ContainerPair.c
S1AP_ProtocolIE-FieldPair.c
S1AP_ProtocolIE-ContainerPairList.c
```

Exclude these NGAP files:

```text
NGAP_ProtocolIE-ContainerPair.c
NGAP_ProtocolIE-FieldPair.c
NGAP_ProtocolIE-ContainerPairList.c
NGAP_ProtocolIE-ContainerPair.h
NGAP_ProtocolIE-FieldPair.h
NGAP_ProtocolIE-ContainerPairList.h
```

Remove unnecessary generated files
==================================

Remove unnecessary files generated by `asn1c`.

Remove unused S1AP generated files:

```sh
cd ~/Documents/git/open5gs/lib/asn1c/s1ap

rm -f \
    S1AP_E-RAB-IE-ContainerPairList.c \
    S1AP_ProtocolError-IE-ContainerList.c \
    S1AP_ProtocolIE-ContainerPair.c \
    S1AP_ProtocolIE-FieldPair.c \
    S1AP_ProtocolIE-ContainerPairList.c
```

Remove unused NGAP generated files:

```sh
cd ~/Documents/git/open5gs/lib/asn1c/ngap

rm -f \
    NGAP_ProtocolIE-ContainerPair.c \
    NGAP_ProtocolIE-FieldPair.c \
    NGAP_ProtocolIE-ContainerPairList.c \
    NGAP_ProtocolIE-ContainerPair.h \
    NGAP_ProtocolIE-ContainerPairList.h \
    NGAP_ProtocolIE-FieldPair.h
```

Remove temporary `asn1c` makefile fragments:

```sh
cd ~/Documents/git/open5gs/lib/asn1c/s1ap

rm -f \
    Makefile.am.asn1convert \
    Makefile.am.libasncodec
```

Remove unused converter/example files:

```sh
cd ~/Documents/git/open5gs/lib/asn1c/s1ap

rm -f \
    converter-example.mk \
    converter-example.c \
    pdu_collection.c
```

Regenerate the CHF CDR ASN.1 files
==================================

The CHF charging records in `lib/asn1c/cdr/` come from
`lib/asn1c/support/cdr-r19.6.0/`. Unlike S1AP and NGAP there is no hand-made
input file to prepare: `generator.sh` assembles the module, runs `asn1c` and
cleans up after it.

The ASN.1 is not in the specification
-------------------------------------

TS 32.298 Rel-19 clause 5.2.5 says only that the ASN.1 definitions are
specified in 3GPP Forge, so there is no `.txt` to extract from a `.doc` as
there is for S1AP and NGAP. `standard/` holds `TS32298_CHFChargingDataTypes.asn`
and its two dependencies, taken verbatim from Forge:

```text
https://forge.3gpp.org/rep/sa5/CH   branch Rel-19, directory ASN
commit 3e24b9aa
```

Do not edit anything in `standard/`. Corrections and trims live in
`patched/`, which is the only hand-written ASN.1 in the bundle:

```text
standard/        pristine 3GPP, exactly as published
patched/         chf-5gsa-records.asn  - the records trimmed to the 5G SA path
                 spec-defects.asn      - corrections for defects in the published module
closure.py       standard + patched -> modified/
modified/        the asn1c input, committed so a reviewer can read it
generator.sh     modified/ -> lib/asn1c/cdr/
```

`patched/spec-defects.asn` currently carries one entry: the published
`NetworkFunctionality` is missing the comma after `aIOTF (23)`, so the
`ENUMERATED` does not parse and the module will not compile as published. The
corrected definition is copied verbatim apart from that comma. Check whether
it is still needed on a release bump, and delete it when 3GPP fixes it.

Run the generator
-----------------

```sh
cd lib/asn1c/support/cdr-r19.6.0

ASN1C=/path/to/asn1c/asn1c/asn1c ./generator.sh
```

`ASN1C` is the `mouse07410/asn1c` build described above, the same one NGAP and
S1AP use. The original `https://github.com/vlm/asn1c` cannot generate this
codec as it stands: it has no `-fprefix`, which the `CHFCDR_` names come from,
and its only codec switches are `-no-gen-OER` and `-no-gen-PER`, so it cannot
leave out XER as `generator.sh` does.

It prints the number of `.c` and `.h` files written, and warns if any skeleton
it copied differs from `lib/asn1c/common`'s — check whether that is an `asn1c`
version difference before committing it.

BER, not APER
-------------

TS 32.298 clause 6.1 requires BER on the Bx interface, which is the mirror
image of S1AP and NGAP: those are generated with `-no-gen-BER` and keep APER,
and the CDR codec keeps BER and drops APER. `asn1c` writes that choice into
`asn_config.h`.

That is why `lib/asn1c/cdr/` keeps its own full copy of the `asn1c` runtime
instead of sharing `lib/asn1c/common/`. A C quote-include resolves against the
including file's own directory first, so sharing `common`'s sources would
compile them against `common`'s BER-disabled `asn_config.h` while the generated
code used the BER-enabled one — and the two disagree about the layout of
`asn_TYPE_operation_s`. That mismatch links cleanly and fails at run time, so
the duplication is deliberate. `generator.sh` copies the Open5GS-patched
`asn_internal.h` from `common` rather than re-deriving the allocator patch
described under "Check common ASN.1 runtime file", and refuses to run if
`common`'s copy is not the patched one.

Update generated source/header lists
------------------------------------

Neither `lib/asn1c/cdr/meson.build` nor `lib/asn1c/cdr/ogs-asn1c-cdr.h` is
generated, and both are synced by hand. The `Makefile.am.libasncodec` method
used for NGAP/S1AP above is not available here, because `generator.sh` deletes
that file along with the other fragments `asn1c` leaves behind. List the files
directly instead:

```sh
cd lib/asn1c/cdr

ls *.c | sort | sed "s|^|    |"
ls CHFCDR_*.h | sort | sed "s|^|#include \"|; s|$|\"|"
```

The Python CDR decoder needs no action: `src/chf/cdr-decode.py.in` is filled
from `modified/` at build time, so it cannot drift from the encoder.

Rationale for the trim
----------------------

`CHFRecord` reaches 353 types across seven sibling charging modules, TS 29.002
MAP, CAMEL CAP and ITU-T CMIP/ACSE/X.721 — none of which a PDU-session CDR
needs, and some of which 3GPP does not publish. `closure.py` keeps only what
the trimmed records reach: 26 types, 21 of them copied verbatim. Because only
`OPTIONAL` fields were dropped and no tag moved, a decoder built from the
complete TS 32.298 still reads our records.

The fuller account, including what an R20 upgrade involves, is section 5.2 of
`docs/CHF-Technical-Overview.md`.

Final check
===========

Check the final generated changes:

```sh
cd ~/Documents/git/open5gs

git status
git diff
```

Then build Open5GS to make sure the regenerated files compile correctly:

```sh
ninja -C build
```
