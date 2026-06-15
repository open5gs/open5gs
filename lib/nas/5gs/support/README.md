Regenerate 5GS NAS message files
================================

This document describes how to regenerate the 5GS NAS message
encoder/decoder files for Open5GS using the 3GPP TS 24.501 document.

Install dependency
==================

Install `python3-docx`.

```sh
sudo apt install python3-docx
```

Prepare TS 24.501 input file
============================

Prepare the 3GPP TS 24.501 Word document.

Example input file:

```text
r19.6.2/24501-j62.docx
```

Create a reduced Word document from the original file. The reduced
document should contain only Chapter 8 and Chapter 9.

Example reduced file:

```text
r19.6.2/24501-j62-ch8-ch9.docx
```

Before running the generator, check the message tables in Chapter 8.
Some table cells may need to be adjusted manually so that each row has
the expected IEI, IE name, type/reference, presence, format, and length
fields.

Update table indexes
====================

Update the table indexes in `nas-message.py`.

The generator uses Word table indexes to find each NAS message table.

```python
# Table number for Message List
msg_list["AUTHENTICATION REQUEST"]["table"] = 0
msg_list["AUTHENTICATION RESPONSE"]["table"] = 1
msg_list["AUTHENTICATION RESULT"]["table"] = 2
msg_list["AUTHENTICATION FAILURE"]["table"] = 3
msg_list["AUTHENTICATION REJECT"]["table"] = 4
msg_list["REGISTRATION REQUEST"]["table"] = 5
msg_list["REGISTRATION ACCEPT"]["table"] = 6
msg_list["REGISTRATION COMPLETE"]["table"] = 8
msg_list["REGISTRATION REJECT"]["table"] = 9
msg_list["UL NAS TRANSPORT"]["table"] = 10
msg_list["DL NAS TRANSPORT"]["table"] = 11
msg_list["DEREGISTRATION REQUEST FROM UE"]["table"] = 12
msg_list["DEREGISTRATION ACCEPT FROM UE"]["table"] = 13
msg_list["DEREGISTRATION REQUEST TO UE"]["table"] = 14
msg_list["DEREGISTRATION ACCEPT TO UE"]["table"] = 15
msg_list["SERVICE REQUEST"]["table"] = 16
msg_list["SERVICE ACCEPT"]["table"] = 17
msg_list["SERVICE REJECT"]["table"] = 18
msg_list["CONFIGURATION UPDATE COMMAND"]["table"] = 19
msg_list["CONFIGURATION UPDATE COMPLETE"]["table"] = 20
msg_list["IDENTITY REQUEST"]["table"] = 21
msg_list["IDENTITY RESPONSE"]["table"] = 22
msg_list["NOTIFICATION"]["table"] = 23
msg_list["NOTIFICATION RESPONSE"]["table"] = 24
msg_list["SECURITY MODE COMMAND"]["table"] = 25
...
```

The exact indexes may change when the Word document is edited. If the
generator cannot find a message table, check the reduced document and
update the table index again.

Generate files
==============

Remove the old cache files if the input document or table indexes were
changed.

```sh
rm -f cache/nas-msg-*.py
```

Run the generator.

```sh
python3 nas-message.py -f r19.6.2/24501-j62-ch8-ch9.docx -o ..
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
structures must be added manually when TS 24.501 introduces new IEs.

Use the existing definitions in `types.h` as references.

Example:

```c
/* 9.11.x.y Example IE
 * O TLV-E 6-n */
typedef struct ogs_nas_example_ie_s {
    uint16_t length;
    void *buffer;
} __attribute__ ((packed)) ogs_nas_example_ie_t;
```

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
