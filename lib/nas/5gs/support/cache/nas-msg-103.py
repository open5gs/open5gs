ies = []
ies.append({ "iei" : "", "value" : "Payload container type", "type" : "Payload container type", "reference" : "9.11.3.40", "presence" : "M", "format" : "V", "length" : "1/2"})
ies.append({ "iei" : "", "value" : "Payload container", "type" : "Payload container", "reference" : "9.11.3.39", "presence" : "M", "format" : "LV-E", "length" : "3-65537"})
ies.append({ "iei" : "12", "value" : "PDU session ID", "type" : "PDU session identity 2", "reference" : "9.11.3.41", "presence" : "C", "format" : "TV", "length" : "2"})
ies.append({ "iei" : "59", "value" : "Old PDU session ID", "type" : "PDU session identity 2", "reference" : "9.11.3.41", "presence" : "O", "format" : "TV", "length" : "2"})
ies.append({ "iei" : "8-", "value" : "Request type", "type" : "Request type", "reference" : "9.11.3.47", "presence" : "O", "format" : "TV", "length" : "1"})
ies.append({ "iei" : "22", "value" : "S-NSSAI", "type" : "S-NSSAI", "reference" : "9.11.2.8", "presence" : "O", "format" : "TLV", "length" : "3-10"})
ies.append({ "iei" : "25", "value" : "DNN", "type" : "DNN", "reference" : "9.11.2.1B", "presence" : "O", "format" : "TLV", "length" : "3-102"})
ies.append({ "iei" : "24", "value" : "Additional information", "type" : "Additional information", "reference" : "9.11.2.1", "presence" : "O", "format" : "TLV", "length" : "3-n"})
ies.append({ "iei" : "A-", "value" : "MA PDU session information", "type" : "MA PDU session information", "reference" : "9.11.3.31A", "presence" : "O", "format" : "TV", "length" : "1"})
ies.append({ "iei" : "F-", "value" : "Release assistance indication", "type" : "Release assistance indication", "reference" : "9.11.3.46A", "presence" : "O", "format" : "TV", "length" : "1"})
msg_list[key]["ies"] = ies
