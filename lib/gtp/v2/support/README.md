Regenerate GTPv2 TLV message files
===================================

This document describes how to regenerate the GTPv2-C TLV message
encoder/decoder files for Open5GS using the 3GPP TS 29.274 document.

Install dependency
==================

Install `python3-docx`.

On Debian/Ubuntu, prefer the distribution package:

```sh
sudo apt install python3-docx
```

If the package is not available, install it with pip:

```sh
python3 -m pip install python-docx
```

Prepare TS 29.274 input file
============================

Prepare the 3GPP TS 29.274 Word document.

Example input file:

```text
r19.6.0/29274-j60.docx
```

The generator reads the Word tables directly from the input document.
When the 3GPP document version changes, the table layout may also change.
Some table cells may need to be adjusted manually so that each row has the
expected IE name, presence, comment, IE type, and instance fields.

Generate files
==============

Move to the GTPv2 support directory.

```sh
cd lib/gtp/v2/support
```

Remove the old cache files if the input document, table indexes, or generator
logic were changed.

```sh
rm -rf cache
```

Run the generator.

```sh
python3 gtp-tlv.py -f r19.6.0/29274-j60.docx -o ..
```

This regenerates the following files:

```text
../message.h
../message.c
```

Debug generation
================

Run the generator with debug output when updating to a new TS 29.274 version
or when a table cannot be parsed.

```sh
python3 gtp-tlv.py -d -f r19.6.0/29274-j60.docx -o ..
```

The debug output prints the detected message list, IE type list, grouped IE
list, and message table indexes.

Check the generated cache files when parsing fails.

```sh
grep -R "Unknown IE type\|Traceback\|AssertionError" -n cache
```

Table indexes
=============

`gtp-tlv.py` uses Word table indexes to find GTPv2 message tables.
The exact indexes may change when the 3GPP Word document changes.

If the generator fails to find or parse a message table, run with `-d`, check
the printed table indexes, and update the table assignments in `gtp-tlv.py`.

For TS 29.274 V19.6.0, the generator is expected to read tables such as:

```python
msg_list["Echo Request"]["table"] = 8
msg_list["Echo Response"]["table"] = 9
msg_list["Create Session Request"]["table"] = 10
msg_list["Create Session Response"]["table"] = 15
msg_list["Create Bearer Request"]["table"] = 21
msg_list["Create Bearer Response"]["table"] = 26
msg_list["Bearer Resource Command"]["table"] = 29
msg_list["Bearer Resource Failure Indication"]["table"] = 31
msg_list["Modify Bearer Request"]["table"] = 33
msg_list["Modify Bearer Response"]["table"] = 37
msg_list["Delete Session Request"]["table"] = 43
msg_list["Delete Bearer Request"]["table"] = 45
msg_list["Delete Session Response"]["table"] = 50
msg_list["Delete Bearer Response"]["table"] = 53
msg_list["Downlink Data Notification"]["table"] = 56
msg_list["Downlink Data Notification Acknowledge"]["table"] = 59
msg_list["Downlink Data Notification Failure Indication"]["table"] = 60
msg_list["Delete Indirect Data Forwarding Tunnel Request"]["table"] = 61
msg_list["Delete Indirect Data Forwarding Tunnel Response"]["table"] = 62
msg_list["Modify Bearer Command"]["table"] = 63
msg_list["Modify Bearer Failure Indication"]["table"] = 66
msg_list["Update Bearer Request"]["table"] = 68
msg_list["Update Bearer Response"]["table"] = 73
msg_list["Delete Bearer Command"]["table"] = 76
msg_list["Delete Bearer Failure Indication"]["table"] = 79
msg_list["Create Indirect Data Forwarding Tunnel Request"]["table"] = 82
msg_list["Create Indirect Data Forwarding Tunnel Response"]["table"] = 84
msg_list["Release Access Bearers Request"]["table"] = 86
msg_list["Release Access Bearers Response"]["table"] = 87
msg_list["Modify Access Bearers Request"]["table"] = 91
msg_list["Modify Access Bearers Response"]["table"] = 94
```

Grouped IE tables
=================

The generator scans grouped IE tables from the Word document.
Grouped IEs such as `Bearer Context`, `PDN Connection`,
`Overload Control Information`, `Load Control Information`,
`Remote UE Context`, `PC5 QoS Parameters`, and `PGW Change Info` must be
parsed as grouped IE structures.

If a grouped IE is not found, it may be generated as a plain
`ogs_tlv_octet_t`, which will cause build errors when existing code accesses
child IEs.

For example, this is wrong for a grouped IE:

```c
typedef ogs_tlv_octet_t ogs_gtp2_tlv_bearer_context_t;
```

It should be generated as a structure:

```c
typedef struct ogs_gtp2_tlv_bearer_context_s {
    ogs_tlv_presence_t presence;
    ...
} ogs_gtp2_tlv_bearer_context_t;
```

IE name aliases
===============

Some IE rows use a long name with an abbreviation in parentheses while the IE
type table uses only the abbreviation. The generator should normalize these
aliases before lookup.

Examples:

```text
Additional Protocol Configuration Options (APCO) -> APCO
Local Distinguished Name (LDN) -> LDN
IPv4 Configuration Parameters (IP4CP) -> IP4CP
Procedure Transaction ID (PTI) -> PTI
```

If generation fails with `Unknown IE type`, check whether the IE is an alias
or a renamed IE in the TS 29.274 document and update `gtp-tlv.py` accordingly.

Final check
===========

Check the generated changes.

```sh
git diff ../message.h ../message.c
```

Check the cache diff if needed.

```sh
git diff cache/tlv-type-list.py cache/tlv-group-list.py cache/tlv-msg-*.py
```

Search for obvious generation failures.

```sh
grep -R "Unknown IE type\|Traceback\|AssertionError" -n cache
```

Build Open5GS.

```sh
ninja -C build
```
