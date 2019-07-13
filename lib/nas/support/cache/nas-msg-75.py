ies = []
ies.append({ "iei" : "", "value" : "EMM cause", "type" : "EMM cause", "reference" : "9.9.3.9", "presence" : "M", "format" : "V", "length" : "1"})
ies.append({ "iei" : "5F", "value" : "T3346 value", "type" : "GPRS timer 2", "reference" : "9.9.3.16A", "presence" : "O", "format" : "TLV", "length" : "3"})
ies.append({ "iei" : "A-", "value" : "Extended EMM cause", "type" : "Extended EMM cause", "reference" : "9.9.3.26A", "presence" : "O", "format" : "TV", "length" : "1"})
msg_list[key]["ies"] = ies
