ies = []
ies.append({ "iei" : "", "value" : "S-NSSAI", "type" : "S-NSSAI", "reference" : "9.11.2.8", "presence" : "M", "format" : "LV", "length" : "2-5"})
ies.append({ "iei" : "", "value" : "EAP message", "type" : "EAP message", "reference" : "9.11.2.2", "presence" : "M", "format" : "LV-E", "length" : "6-1502"})
msg_list[key]["ies"] = ies
