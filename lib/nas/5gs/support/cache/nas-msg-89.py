ies = []
ies.append({ "iei" : "", "value" : "5GMM cause", "type" : "5GMM cause", "reference" : "9.11.3.2", "presence" : "M", "format" : "V", "length" : "1"})
ies.append({ "iei" : "30", "value" : "Authentication failure parameter", "type" : "Authentication failure parameter", "reference" : "9.11.3.14", "presence" : "O", "format" : "TLV", "length" : "16"})
msg_list[key]["ies"] = ies
