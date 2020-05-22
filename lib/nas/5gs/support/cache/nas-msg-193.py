ies = []
ies.append({ "iei" : "", "value" : "Control plane service type", "type" : "Control plane service type", "reference" : "9.11.3.18D", "presence" : "M", "format" : "V", "length" : "1/2"})
ies.append({ "iei" : "6F", "value" : "CIoT small data container", "type" : "CIoT small data container", "reference" : "9.11.3.18B", "presence" : "O", "format" : "TLV", "length" : "4-257"})
ies.append({ "iei" : "8-", "value" : "Payload container type", "type" : "Payload container type", "reference" : "9.11.3.40", "presence" : "O", "format" : "TV", "length" : "1"})
ies.append({ "iei" : "7B", "value" : "Payload container", "type" : "Payload container", "reference" : "9.11.3.39", "presence" : "O", "format" : "TLV-E", "length" : "4-65538"})
ies.append({ "iei" : "12", "value" : "PDU session ID", "type" : "PDU session identity 2", "reference" : "9.11.3.41", "presence" : "C", "format" : "TV", "length" : "2"})
ies.append({ "iei" : "50", "value" : "PDU session status", "type" : "PDU session status", "reference" : "9.11.3.44", "presence" : "O", "format" : "TLV", "length" : "4-34"})
ies.append({ "iei" : "F-", "value" : "Release assistance indication", "type" : "Release assistance indication", "reference" : "9.11.3.46A", "presence" : "O", "format" : "TV", "length" : "1"})
ies.append({ "iei" : "40", "value" : "Uplink data status", "type" : "Uplink data status", "reference" : "9.11.3.57", "presence" : "O", "format" : "TLV", "length" : "4-34"})
ies.append({ "iei" : "71", "value" : "NAS message container", "type" : "message container", "reference" : "9.11.3.33", "presence" : "O", "format" : "TLV-E", "length" : "4-n"})
ies.append({ "iei" : "24", "value" : "Additional information", "type" : "Additional information", "reference" : "9.11.2.1", "presence" : "O", "format" : "TLV", "length" : "3-n"})
msg_list[key]["ies"] = ies
