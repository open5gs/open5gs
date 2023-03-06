ies = []
ies.append({ "iei" : "", "value" : "ngKSI", "type" : "key set identifier", "reference" : "9.11.3.32", "presence" : "M", "format" : "V", "length" : "1/2"})
ies.append({ "iei" : "", "value" : "5G-S-TMSI", "type" : "5GS mobile identity", "reference" : "9.11.3.4", "presence" : "M", "format" : "LV-E", "length" : "9"})
ies.append({ "iei" : "40", "value" : "Uplink data status", "type" : "Uplink data status", "reference" : "9.11.3.57", "presence" : "O", "format" : "TLV", "length" : "4-34"})
ies.append({ "iei" : "50", "value" : "PDU session status", "type" : "PDU session status", "reference" : "9.11.3.44", "presence" : "O", "format" : "TLV", "length" : "4-34"})
ies.append({ "iei" : "25", "value" : "Allowed PDU session status", "type" : "Allowed PDU session status", "reference" : "9.11.3.13", "presence" : "O", "format" : "TLV", "length" : "4-34"})
ies.append({ "iei" : "71", "value" : "NAS message container", "type" : "message container", "reference" : "9.11.3.33", "presence" : "O", "format" : "TLV-E", "length" : "4-n"})
ies.append({ "iei" : "29", "value" : "UE request type", "type" : "UE request type", "reference" : "9.11.3.76", "presence" : "O", "format" : "TLV", "length" : "3"})
ies.append({ "iei" : "28", "value" : "Paging restriction", "type" : "Paging restriction", "reference" : "9.11.3.77", "presence" : "O", "format" : "TLV", "length" : "3-35"})
msg_list[key]["ies"] = ies
