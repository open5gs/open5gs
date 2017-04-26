ies = []
ies.append({ "iei" : "", "value" : "KSI and sequence number", "type" : "KSI and sequence number", "reference" : "9.9.3.19", "presence" : "M", "format" : "V", "length" : "1"})
ies.append({ "iei" : "", "value" : "Message authentication code", "type" : "Short MAC", "reference" : "9.9.3.28", "presence" : "M", "format" : "V", "length" : "2"})
msg_list[key]["ies"] = ies
