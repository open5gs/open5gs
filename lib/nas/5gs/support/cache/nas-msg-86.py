ies = []
ies.append({ "iei" : "", "value" : "ngKSI", "type" : "key set identifier", "reference" : "9.11.3.32", "presence" : "M", "format" : "V", "length" : "1/2"})
ies.append({ "iei" : "", "value" : "ABBA", "type" : "ABBA", "reference" : "9.11.3.10", "presence" : "M", "format" : "LV", "length" : "3-n"})
ies.append({ "iei" : "21", "value" : "Authentication parameter RAND", "type" : "Authentication parameter RAND", "reference" : "9.11.3.16", "presence" : "O", "format" : "TV", "length" : "17"})
ies.append({ "iei" : "20", "value" : "Authentication parameter AUTN", "type" : "Authentication parameter AUTN", "reference" : "9.11.3.15", "presence" : "O", "format" : "TLV", "length" : "18"})
ies.append({ "iei" : "78", "value" : "EAP message", "type" : "EAP message", "reference" : "9.11.2.2", "presence" : "O", "format" : "TLV-E", "length" : "7-1503"})
msg_list[key]["ies"] = ies
