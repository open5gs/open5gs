ies = []
ies.append({ "iei" : "", "value" : "ngKSI", "type" : "key set identifier", "reference" : "9.11.3.32", "presence" : "M", "format" : "V", "length" : "1/2"})
ies.append({ "iei" : "", "value" : "EAP message", "type" : "EAP message", "reference" : "9.11.2.2", "presence" : "M", "format" : "LV-E", "length" : "6-1502"})
ies.append({ "iei" : "38", "value" : "ABBA", "type" : "ABBA", "reference" : "9.11.3.10", "presence" : "O", "format" : "TLV", "length" : "4-n"})
msg_list[key]["ies"] = ies
