Regenerate NGAP/S1AP ASN.1 files
================================

This document describes how to regenerate the NGAP/S1AP ASN.1
encoder/decoder files for Open5GS using `asn1c`.

The generated files are based on the 3GPP ASN.1 specifications and the
`mouse07410/asn1c` `vlm_master` branch.

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
