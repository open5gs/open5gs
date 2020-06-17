ies = []
ies.append({ "iei" : "", "value" : "EAP message", "type" : "EAP message", "reference" : "9.11.2.2", "presence" : "M", "format" : "LV-E", "length" : "6-1502"})
ies.append({ "iei" : "7B", "value" : "Extended protocol configuration options", "type" : "Extended protocol configuration options", "reference" : "9.11.4.6", "presence" : "O", "format" : "TLV-E", "length" : "4-65538"})
msg_list[key]["ies"] = ies
