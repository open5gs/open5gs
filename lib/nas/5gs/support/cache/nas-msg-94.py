ies = []
ies.append({ "iei" : "77", "value" : "IMEISV", "type" : "5GS mobile identity", "reference" : "9.11.3.4", "presence" : "O", "format" : "TLV-E", "length" : "12"})
ies.append({ "iei" : "71", "value" : "NAS message container", "type" : "message container", "reference" : "9.11.3.33", "presence" : "O", "format" : "TLV-E", "length" : "4-n"})
ies.append({ "iei" : "78", "value" : "non-IMEISV PEI", "type" : "5GS mobile identity", "reference" : "9.11.3.4", "presence" : "O", "format" : "TLV-E", "length" : "7-n"})
msg_list[key]["ies"] = ies
