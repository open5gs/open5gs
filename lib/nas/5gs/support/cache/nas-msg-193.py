ies = []
ies.append({ "iei" : "", "value" : "Integrity protection maximum data rate", "type" : "Integrity protection maximum data rate", "reference" : "9.11.4.7", "presence" : "M", "format" : "V", "length" : "2"})
ies.append({ "iei" : "9-", "value" : "PDU session type", "type" : "PDU session type", "reference" : "9.11.4.11", "presence" : "O", "format" : "TV", "length" : "1"})
ies.append({ "iei" : "A-", "value" : "SSC mode", "type" : "SSC mode", "reference" : "9.11.4.16", "presence" : "O", "format" : "TV", "length" : "1"})
ies.append({ "iei" : "28", "value" : "5GSM capability", "type" : "5GSM capability", "reference" : "9.11.4.1", "presence" : "O", "format" : "TLV", "length" : "3-15"})
ies.append({ "iei" : "55", "value" : "Maximum number of supported packet filters", "type" : "Maximum number of supported packet filters", "reference" : "9.11.4.9", "presence" : "O", "format" : "TV", "length" : "3"})
ies.append({ "iei" : "B-", "value" : "Always-on PDU session requested", "type" : "Always-on PDU session requested", "reference" : "9.11.4.4", "presence" : "O", "format" : "TV", "length" : "1"})
ies.append({ "iei" : "39", "value" : "SM PDU DN request container", "type" : "SM PDU DN request container", "reference" : "9.11.4.15", "presence" : "O", "format" : "TLV", "length" : "3-255"})
ies.append({ "iei" : "7B", "value" : "Extended protocol configuration options", "type" : "Extended protocol configuration options", "reference" : "9.11.4.6", "presence" : "O", "format" : "TLV-E", "length" : "4-65538"})
ies.append({ "iei" : "66", "value" : "Header compression configuration", "type" : "Header compression configuration", "reference" : "9.11.4.24", "presence" : "O", "format" : "TLV", "length" : "5-257"})
ies.append({ "iei" : "6E", "value" : "DS-TT Ethernet port MAC address", "type" : "DS-TT Ethernet port MAC address", "reference" : "9.11.4.25", "presence" : "O", "format" : "TLV", "length" : "8"})
ies.append({ "iei" : "6F", "value" : "UE-DS-TT residence time", "type" : "UE-DS-TT residence time", "reference" : "9.11.4.26", "presence" : "O", "format" : "TLV", "length" : "10"})
ies.append({ "iei" : "7C", "value" : "Port management information container", "type" : "Port management information container", "reference" : "9.11.4.27", "presence" : "O", "format" : "TLV-E", "length" : "4-65538"})
msg_list[key]["ies"] = ies
