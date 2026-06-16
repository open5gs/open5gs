Regenerate PFCP TLV message files
=================================

This document describes how to regenerate the PFCP TLV message
encoder/decoder files for Open5GS using the 3GPP TS 29.244 document.

Install dependency
==================

Install `python3-docx`.

```sh
sudo apt install python3-docx
```

Prepare TS 29.244 input file
============================

Prepare the 3GPP TS 29.244 Word document.

Example input file:

```text
r19.5.0/29244-j50.docx
```

The generator reads the Word tables directly from the input document.
When the 3GPP document version changes, the table layout may also change.
Some table cells may need to be adjusted manually so that each row has the
expected IE name, presence, comment, and IE type fields.

Update table indexes
====================

Update the table indexes in `pfcp-tlv.py` if the input Word document has
changed.

The generator uses Word table indexes to find each PFCP message table.

```python
# Table number for Message List
msg_list["PFCP Association Setup Request"]["table"] = 16
msg_list["PFCP Association Setup Response"]["table"] = 23
msg_list["PFCP Association Update Request"]["table"] = 24
msg_list["PFCP Association Update Response"]["table"] = 26
msg_list["PFCP Association Release Request"]["table"] = 27
msg_list["PFCP Association Release Response"]["table"] = 28
msg_list["PFCP Node Report Request"]["table"] = 29
msg_list["PFCP Node Report Response"]["table"] = 37
msg_list["PFCP Session Set Deletion Request"]["table"] = 38
msg_list["PFCP Session Set Deletion Response"]["table"] = 39
msg_list["PFCP Session Set Modification Request"]["table"] = 40
msg_list["PFCP Session Set Modification Response"]["table"] = 42
msg_list["PFCP Session Establishment Request"]["table"] = 43
msg_list["PFCP Session Establishment Response"]["table"] = 84
msg_list["PFCP Session Modification Request"]["table"] = 100
msg_list["PFCP Session Modification Response"]["table"] = 127
msg_list["PFCP Session Deletion Request"]["table"] = 132
msg_list["PFCP Session Deletion Response"]["table"] = 133
msg_list["PFCP Session Report Request"]["table"] = 137
msg_list["PFCP Session Report Response"]["table"] = 153
```

The exact indexes may change when the Word document is edited. If the
generator cannot find a message table, run the generator with debug output,
check the printed table indexes, and update `pfcp-tlv.py` again.

```sh
python3 pfcp-tlv.py -d -f r19.5.0/29244-j50.docx -o ..
```

Grouped IE tables
=================

The generator also scans grouped IE tables from the Word document.

Grouped IEs such as `Create URR`, `Update URR`, `Create FAR`, `Update FAR`,
and Access Forwarding Action Information must be parsed as grouped IE
structures. If a grouped IE is not found, it may be generated as a plain
`ogs_tlv_octet_t`, which will cause build errors when existing code accesses
child IEs.

For example, this is wrong for a grouped IE:

```c
typedef ogs_tlv_octet_t ogs_pfcp_tlv_create_urr_t;
```

It should be generated as a structure:

```c
typedef struct ogs_pfcp_tlv_create_urr_s {
    ogs_tlv_presence_t presence;
    ...
} ogs_pfcp_tlv_create_urr_t;
```

When the Word document contains rows such as `Same IEs and requirements as
defined in Table ...`, update the generator so that the referenced IE list is
copied instead of treating the row as a normal IE.

Generate files
==============

Remove the old cache files if the input document, table indexes, or generator
logic were changed.

```sh
rm -rf cache
```

Run the generator.

```sh
python3 pfcp-tlv.py -f r19.5.0/29244-j50.docx -o ..
```

This regenerates the following files:

```text
../message.h
../message.c
```

Update PFCP IE support code
===========================

If TS 29.244 removes or changes an IE, update the hand-written PFCP support
code as needed.

For example, if a removed IE is no longer generated in `message.h`, remove
references to that IE from files such as:

```text
../build.c
../handler.c
../types.c
../types.h
../conv.c
../conv.h
```

Use existing PFCP IE helper functions and structures as references when adding
new IEs or removing obsolete ones.

Final check
===========

Check the generated changes.

```sh
git diff ../message.h ../message.c
```

Also check any hand-written files that were updated together with the generated
files.

```sh
git diff ../build.c ../handler.c ../types.c ../types.h ../conv.c ../conv.h
```

Build Open5GS.

```sh
ninja -C build
```
