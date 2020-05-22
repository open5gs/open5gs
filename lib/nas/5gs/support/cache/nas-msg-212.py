ies = []
ies.append({ "iei" : "", "value" : "5GSM cause", "type" : "5GSM cause", "reference" : "9.11.4.2", "presence" : "M", "format" : "V", "length" : "1"})
ies.append({ "iei" : "7B", "value" : "Extended protocol configuration options", "type" : "Extended protocol configuration options", "reference" : "9.11.4.6", "presence" : "O", "format" : "TLV-E", "length" : "4-65538"})
msg_list[key]["ies"] = ies
