ies = []
ies.append({ "iei" : "", "value" : "Selected NAS security algorithms", "type" : "security algorithms", "reference" : "9.9.3.23", "presence" : "M", "format" : "V", "length" : "1"})
ies.append({ "iei" : "", "value" : "NAS key set identifier", "type" : "key set identifier", "reference" : "9.9.3.21", "presence" : "M", "format" : "V", "length" : "1/2"})
ies.append({ "iei" : "", "value" : "Replayed UE security capabilities", "type" : "UE security capability", "reference" : "9.9.3.36", "presence" : "M", "format" : "LV", "length" : "3-6"})
ies.append({ "iei" : "C-", "value" : "IMEISV request", "type" : "IMEISV request", "reference" : "9.9.3.18", "presence" : "O", "format" : "TV", "length" : "1"})
ies.append({ "iei" : "55", "value" : "Replayed nonceUE", "type" : "Nonce", "reference" : "9.9.3.25", "presence" : "O", "format" : "TV", "length" : "5"})
ies.append({ "iei" : "56", "value" : "NonceMME", "type" : "Nonce", "reference" : "9.9.3.25", "presence" : "O", "format" : "TV", "length" : "5"})
msg_list[key]["ies"] = ies
