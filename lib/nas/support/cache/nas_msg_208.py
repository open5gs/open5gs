ies = []
ies.append({ "iei" : "", "value" : "Request type", "type" : "Request type", "reference" : "9.9.4.14", "presence" : "M", "format" : "V", "length" : "1/2"})
ies.append({ "iei" : "D-", "value" : "ESM information transfer flag", "type" : "ESM information transfer flag", "reference" : "9.9.4.5", "presence" : "O", "format" : "TV", "length" : "1"})
ies.append({ "iei" : "28", "value" : "Access point name", "type" : "Access point name", "reference" : "9.9.4.1", "presence" : "O", "format" : "TLV", "length" : "3-102"})
ies.append({ "iei" : "27", "value" : "Protocol configuration options", "type" : "Protocol configuration options", "reference" : "9.9.4.11", "presence" : "O", "format" : "TLV", "length" : "3-253"})
ies.append({ "iei" : "C-", "value" : "Device properties", "type" : "Device properties", "reference" : "9.9.2.0A", "presence" : "O", "format" : "TV", "length" : "1"})
ies.append({ "iei" : "33", "value" : "NBIFOM container", "type" : "NBIFOM container", "reference" : "9.9.4.19", "presence" : "O", "format" : "TLV", "length" : "3-257"})
ies.append({ "iei" : "66", "value" : "Header compression configuration", "type" : "Header compression configuration", "reference" : "9.9.4.22", "presence" : "O", "format" : "TLV", "length" : "5-257"})
ies.append({ "iei" : "7B", "value" : "Extended protocol configuration options", "type" : "Extended protocol configuration options", "reference" : "9.9.4.26", "presence" : "O", "format" : "TLV-E", "length" : "4-65538"})
msg_list[key]["ies"] = ies
