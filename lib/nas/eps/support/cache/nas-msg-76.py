ies = []
ies.append({ "iei" : "", "value" : "Service type", "type" : "Service type", "reference" : "9.9.3.27", "presence" : "M", "format" : "V", "length" : "1/2"})
ies.append({ "iei" : "", "value" : "M-TMSI", "type" : "Mobile identity", "reference" : "9.9.2.3", "presence" : "M", "format" : "LV", "length" : "6"})
ies.append({ "iei" : "B-", "value" : "CSFB response", "type" : "CSFB response", "reference" : "9.9.3.5", "presence" : "O", "format" : "TV", "length" : "1"})
ies.append({ "iei" : "57", "value" : "EPS bearer context status", "type" : "EPS bearer context status", "reference" : "9.9.2.1", "presence" : "O", "format" : "TLV", "length" : "4"})
ies.append({ "iei" : "D-", "value" : "Device properties", "type" : "Device properties", "reference" : "9.9.2.0A", "presence" : "O", "format" : "TV", "length" : "1"})
msg_list[key]["ies"] = ies
