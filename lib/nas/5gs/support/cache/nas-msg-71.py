ies = []
ies.append({ "iei" : "", "value" : "De-registration type", "type" : "De-registration type", "reference" : "9.11.3.20", "presence" : "M", "format" : "V", "length" : "1/2"})
ies.append({ "iei" : "58", "value" : "5GMM cause", "type" : "5GMM cause", "reference" : "9.11.3.2", "presence" : "O", "format" : "TV", "length" : "2"})
ies.append({ "iei" : "5F", "value" : "T3346 value", "type" : "GPRS timer 2", "reference" : "9.11.2.4", "presence" : "O", "format" : "TLV", "length" : "3"})
ies.append({ "iei" : "6D", "value" : "Rejected NSSAI", "type" : "Rejected NSSAI", "reference" : "9.11.3.46", "presence" : "O", "format" : "TLV", "length" : "4-42"})
msg_list[key]["ies"] = ies
