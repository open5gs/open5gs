ies = []
ies.append({ "iei" : "", "value" : "EMM cause", "type" : "EMM cause", "reference" : "9.9.3.9", "presence" : "M", "format" : "V", "length" : "1"})
ies.append({ "iei" : "30", "value" : "Authentication failure parameter", "type" : "Authentication failure parameter", "reference" : "9.9.3.1", "presence" : "O", "format" : "TLV", "length" : "16"})
msg_list[key]["ies"] = ies
