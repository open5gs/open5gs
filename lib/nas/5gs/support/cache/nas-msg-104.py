ies = []
ies.append({ "iei" : "", "value" : "Payload container type", "type" : "Payload container type", "reference" : "9.11.3.40", "presence" : "M", "format" : "V", "length" : "1/2"})
ies.append({ "iei" : "", "value" : "Payload container", "type" : "Payload container", "reference" : "9.11.3.39", "presence" : "M", "format" : "LV-E", "length" : "3-65537"})
ies.append({ "iei" : "12", "value" : "PDU session ID", "type" : "PDU session identity 2", "reference" : "9.11.3.41", "presence" : "C", "format" : "TV", "length" : "2"})
ies.append({ "iei" : "24", "value" : "Additional information", "type" : "Additional information", "reference" : "9.11.2.1", "presence" : "O", "format" : "TLV", "length" : "3-n"})
ies.append({ "iei" : "58", "value" : "5GMM cause", "type" : "5GMM cause", "reference" : "9.11.3.2", "presence" : "O", "format" : "TV", "length" : "2"})
ies.append({ "iei" : "37", "value" : "Back-off timer value", "type" : "GPRS timer 3", "reference" : "9.11.2.5", "presence" : "O", "format" : "TLV", "length" : "3"})
msg_list[key]["ies"] = ies
