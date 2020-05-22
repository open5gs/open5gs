ies = []
ies.append({ "iei" : "50", "value" : "PDU session status", "type" : "PDU session status", "reference" : "9.11.3.44", "presence" : "O", "format" : "TLV", "length" : "4-34"})
ies.append({ "iei" : "26", "value" : "PDU session reactivation result", "type" : "PDU session reactivation result", "reference" : "9.11.3.42", "presence" : "O", "format" : "TLV", "length" : "4-34"})
ies.append({ "iei" : "72", "value" : "PDU session reactivation result error cause", "type" : "PDU session reactivation result error cause", "reference" : "9.11.3.43", "presence" : "O", "format" : "TLV-E", "length" : "5-515"})
ies.append({ "iei" : "78", "value" : "EAP message", "type" : "EAP message", "reference" : "9.11.2.2", "presence" : "O", "format" : "TLV-E", "length" : "7-1503"})
ies.append({ "iei" : "6B", "value" : "T3448 value", "type" : "GPRS timer 3", "reference" : "9.11.2.4", "presence" : "O", "format" : "TLV", "length" : "3"})
msg_list[key]["ies"] = ies
