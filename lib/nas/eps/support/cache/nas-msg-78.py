ies = []
ies.append({ "iei" : "", "value" : "EMM cause", "type" : "EMM cause", "reference" : "9.9.3.9", "presence" : "M", "format" : "V", "length" : "1"})
ies.append({ "iei" : "5B", "value" : "T3442 value", "type" : "GPRS timer", "reference" : "9.9.3.16", "presence" : "C", "format" : "TV", "length" : "2"})
ies.append({ "iei" : "5F", "value" : "T3346 value", "type" : "GPRS timer 2", "reference" : "9.9.3.16A", "presence" : "O", "format" : "TLV", "length" : "3"})
ies.append({ "iei" : "6B", "value" : "T3448 value", "type" : "GPRS timer 2", "reference" : "9.9.3.16A", "presence" : "O", "format" : "TLV", "length" : "3"})
msg_list[key]["ies"] = ies
