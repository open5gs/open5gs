Regenerate EPS NAS message files
================================

This document describes how to regenerate the EPS NAS message
encoder/decoder files for Open5GS using the 3GPP TS 24.301 document.

Install dependency
==================

Install `python3-docx`.

```sh
sudo apt install python3-docx
```

Prepare TS 24.301 input file
============================

Prepare the 3GPP TS 24.301 Word document.

Example input file:

```text
24301-j60.docx
```

Create a reduced Word document from the original file. The reduced
file should contain only Chapter 8 and Chapter 9.

Example reduced file:

```text
24301-j60-ch8-ch9.docx
```

Before running the generator, check the message tables in Chapter 8.
Some table cells may need to be adjusted manually so that each row has
the expected IEI, IE name, type/reference, presence, format, and length
fields.

Update table indexes
====================

Update the table indexes in `nas-message.py` when the reduced Word
file changes.

The generator uses Word table indexes to find each NAS message table.
If the generator cannot find a message table, check the reduced Word
file and update the table index again.

Generate files
==============

Remove the old cache files if the input document or table indexes were
changed.

```sh
rm -f cache/nas-msg-*.py
```

Run the generator.

```sh
python3 nas-message.py -f r19.6.0/24301-j60-ch8-ch9.docx -o ..
```

This regenerates the following files:

```text
../ies.h
../ies.c
../message.h
../decoder.c
../encoder.c
```

Update NAS IE types
===================

Add newly introduced NAS IE structures to `../types.h`.

The generator creates message encoder/decoder code, but new IE
structures must be added manually when TS 24.301 introduces new IEs.
Use the existing definitions in `types.h` as references.

Final check
===========

Check the generated changes.

```sh
git diff ../ies.h ../ies.c ../message.h ../decoder.c ../encoder.c ../types.h
```

Build Open5GS.

```sh
ninja -C build
```
