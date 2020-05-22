ies = []
ies.append({ "iei" : "", "value" : "5GMM cause", "type" : "5GMM cause", "reference" : "9.11.3.2", "presence" : "M", "format" : "V", "length" : "1"})
ies.append({ "iei" : "50", "value" : "PDU session status", "type" : "PDU session status", "reference" : "9.11.3.44", "presence" : "O", "format" : "TLV", "length" : "4-34"})
ies.append({ "iei" : "5F", "value" : "T3346 value", "type" : "GPRS timer 2", "reference" : "9.11.2.4", "presence" : "O", "format" : "TLV", "length" : "3"})
ies.append({ "iei" : "78", "value" : "EAP message", "type" : "EAP message", "reference" : "9.11.2.2", "presence" : "O", "format" : "TLV-E", "length" : "7-1503"})
ies.append({ "iei" : "6B", "value" : "T3448 value", "type" : "GPRS timer 3", "reference" : "9.11.2.4", "presence" : "O", "format" : "TLV", "length" : "3"})
msg_list[key]["ies"] = ies
