ies = []
ies.append({ "iei" : "", "value" : "NAS key set identifierASME", "type" : "key set identifier", "reference" : "9.9.3.21", "presence" : "M", "format" : "V", "length" : "1/2"})
ies.append({ "iei" : "", "value" : "Authentication parameter RAND", "type" : "Authentication parameter RAND", "reference" : "9.9.3.3", "presence" : "M", "format" : "V", "length" : "16"})
ies.append({ "iei" : "", "value" : "Authentication parameter AUTN", "type" : "Authentication parameter AUTN", "reference" : "9.9.3.2", "presence" : "M", "format" : "LV", "length" : "17"})
msg_list[key]["ies"] = ies
