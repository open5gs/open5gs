ies = []
ies.append({ "iei" : "", "value" : "Selected NAS security algorithms", "type" : "security algorithms", "reference" : "9.11.3.34", "presence" : "M", "format" : "V", "length" : "1"})
ies.append({ "iei" : "", "value" : "ngKSI", "type" : "key set identifier", "reference" : "9.11.3.32", "presence" : "M", "format" : "V", "length" : "1/2"})
ies.append({ "iei" : "", "value" : "Replayed UE security capabilities", "type" : "UE security capability", "reference" : "9.11.3.54", "presence" : "M", "format" : "LV", "length" : "3-9"})
ies.append({ "iei" : "E-", "value" : "IMEISV request", "type" : "IMEISV request", "reference" : "9.11.3.28", "presence" : "O", "format" : "TV", "length" : "1"})
ies.append({ "iei" : "57", "value" : "Selected EPS NAS security algorithms", "type" : "EPS NAS security algorithms", "reference" : "9.11.3.25", "presence" : "O", "format" : "TV", "length" : "2"})
ies.append({ "iei" : "36", "value" : "Additional 5G security information", "type" : "Additional 5G security information", "reference" : "9.11.3.12", "presence" : "O", "format" : "TLV", "length" : "3"})
ies.append({ "iei" : "78", "value" : "EAP message", "type" : "EAP message", "reference" : "9.11.2.2", "presence" : "O", "format" : "TLV-E", "length" : "7-1503"})
ies.append({ "iei" : "38", "value" : "ABBA", "type" : "ABBA", "reference" : "9.11.3.10", "presence" : "O", "format" : "TLV", "length" : "4-n"})
ies.append({ "iei" : "19", "value" : "Replayed S1 UE security capabilities", "type" : "S1 UE security capability", "reference" : "9.11.3.48A", "presence" : "O", "format" : "TLV", "length" : "4-7"})
msg_list[key]["ies"] = ies
