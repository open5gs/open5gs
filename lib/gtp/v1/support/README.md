Regenerate GTPv1 TLV message files
===================================

This document describes how to regenerate the GTPv1-C TLV message
encoder/decoder files for Open5GS using the 3GPP TS 29.060 document.

Install dependency
==================

Install `python3-docx`.

```sh
sudo apt install python3-docx
```

Prepare TS 29.060 input file
============================

Prepare the 3GPP TS 29.060 Word document.

Example input file:

```text
r19.0.0/29060-j00.docx
```

The generator reads the Word tables directly from the input document.
For Release 19, the generator is intended to work with the unmodified
3GPP document, so manual adjustment of table cells should not normally
be required.

If the 3GPP document version changes, the Word table layout may also
change. In that case, run the generator with debug output and check the
printed table indexes and parsed cache files.

Generate files
==============

Remove the old cache files if the input document, table indexes, or
generator logic were changed.

```sh
rm -rf cache
```

Run the generator.

```sh
python3 gtp1-tlv.py -f r19.0.0/29060-j00.docx -o ..
```

For debugging, use `-d`.

```sh
python3 gtp1-tlv.py -f r19.0.0/29060-j00.docx -o .. -d
```

This regenerates the following files:

```text
../message.h
../message.c
```

Table indexes
=============

The generator uses Word table indexes to find each GTPv1 message table.
The exact indexes may change when the input Word document changes.

For TS 29.060 V19.0.0, the expected message table mapping is:

```python
msg_list["Echo Request"]["table"] = 12
msg_list["Echo Response"]["table"] = 13
msg_list["Supported Extension Headers Notification"]["table"] = 14
msg_list["Create PDP Context Request"]["table"] = 15
msg_list["Create PDP Context Response"]["table"] = 16
msg_list["Update PDP Context Request"]["table"] = 17
msg_list["Update PDP Context Response"]["table"] = 19
msg_list["Delete PDP Context Request"]["table"] = 21
msg_list["Delete PDP Context Response"]["table"] = 22
msg_list["PDU Notification Request"]["table"] = 23
msg_list["PDU Notification Response"]["table"] = 24
msg_list["PDU Notification Reject Request"]["table"] = 25
msg_list["PDU Notification Reject Response"]["table"] = 26
msg_list["Initiate PDP Context Activation Request"]["table"] = 27
msg_list["Initiate PDP Context Activation Response"]["table"] = 28
msg_list["Send Routeing Information for GPRS Request"]["table"] = 29
msg_list["Send Routeing Information for GPRS Response"]["table"] = 30
msg_list["Failure Report Request"]["table"] = 31
msg_list["Failure Report Response"]["table"] = 32
msg_list["Note MS GPRS Present Request"]["table"] = 33
msg_list["Note MS GPRS Present Response"]["table"] = 34
msg_list["Identification Request"]["table"] = 35
msg_list["Identification Response"]["table"] = 36
msg_list["SGSN Context Request"]["table"] = 37
msg_list["SGSN Context Response"]["table"] = 38
msg_list["SGSN Context Acknowledge"]["table"] = 39
msg_list["Forward Relocation Request"]["table"] = 40
msg_list["Forward Relocation Response"]["table"] = 41
msg_list["Forward Relocation Complete"]["table"] = 42
msg_list["Relocation Cancel Request"]["table"] = 43
msg_list["Relocation Cancel Response"]["table"] = 44
msg_list["Forward Relocation Complete Acknowledge"]["table"] = 45
msg_list["Forward SRNS Context Acknowledge"]["table"] = 46
msg_list["Forward SRNS Context"]["table"] = 47
msg_list["RAN Information Relay"]["table"] = 48
msg_list["UE Registration Query Request"]["table"] = 49
msg_list["UE Registration Query Response"]["table"] = 50
msg_list["MBMS Notification Request"]["table"] = 51
msg_list["MBMS Notification Response"]["table"] = 52
msg_list["MBMS Notification Reject Request"]["table"] = 53
msg_list["MBMS Notification Reject Response"]["table"] = 54
msg_list["Create MBMS Context Request"]["table"] = 55
msg_list["Create MBMS Context Response"]["table"] = 56
msg_list["Update MBMS Context Request"]["table"] = 57
msg_list["Update MBMS Context Response"]["table"] = 58
msg_list["Delete MBMS Context Request"]["table"] = 59
msg_list["Delete MBMS Context Response"]["table"] = 60
msg_list["MBMS Registration Request"]["table"] = 61
msg_list["MBMS Registration Response"]["table"] = 62
msg_list["MBMS De-Registration Request"]["table"] = 63
msg_list["MBMS De-Registration Response"]["table"] = 64
msg_list["MBMS Session Start Request"]["table"] = 65
msg_list["MBMS Session Start Response"]["table"] = 66
msg_list["MBMS Session Stop Request"]["table"] = 67
msg_list["MBMS Session Stop Response"]["table"] = 68
msg_list["MBMS Session Update Request"]["table"] = 69
msg_list["MBMS Session Update Response"]["table"] = 70
```

Parsing notes
=============

The generator parses the IE Type List and the message tables from the
Word document.

Some Word tables use merged cells. The generator normalizes merged cells
before parsing so that rows such as the MBMS tables can be processed
without manually editing the input document.

Some rows may have unusual formatting or empty cells in the Word document.
The generator includes compatibility handling for known cases such as:

```text
Teardown Ind
GSN Address
MS Time Zone
```

The generated cache files are useful for checking parser output:

```text
cache/tlv-msg-list.py
cache/tlv-type-list.py
cache/tlv-msg-*.py
```

Final check
===========

Check the generated changes.

```sh
git diff ../message.h ../message.c
```

Check for generation errors in the cache.

```sh
grep -R "Traceback\|ValueError\|no IE type found" -n cache
```

Build Open5GS.

```sh
ninja -C build
```
