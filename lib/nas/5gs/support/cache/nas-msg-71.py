ies = []
ies.append({ "iei" : "", "value" : "De-registration type", "type" : "De-registration type", "reference" : "9.11.3.20", "presence" : "M", "format" : "V", "length" : "1/2"})
ies.append({ "iei" : "58", "value" : "5GMM cause", "type" : "5GMM cause", "reference" : "9.11.3.2", "presence" : "O", "format" : "TV", "length" : "2"})
ies.append({ "iei" : "5F", "value" : "T3346 value", "type" : "GPRS timer 2", "reference" : "9.11.2.4", "presence" : "O", "format" : "TLV", "length" : "3"})
ies.append({ "iei" : "6D", "value" : "Rejected NSSAI", "type" : "Rejected NSSAI", "reference" : "9.11.3.46", "presence" : "O", "format" : "TLV", "length" : "4-42"})
ies.append({ "iei" : "75", "value" : "CAG information list", "type" : "CAG information list", "reference" : "9.11.3.18A", "presence" : "O", "format" : "TLV-E", "length" : "3-n"})
ies.append({ "iei" : "68", "value" : "Extended rejected NSSAI", "type" : "Extended rejected NSSAI", "reference" : "9.11.3.75", "presence" : "O", "format" : "TLV", "length" : "5-90"})
ies.append({ "iei" : "2C", "value" : "Disaster return wait range", "type" : "Registration wait range", "reference" : "9.11.3.84", "presence" : "O", "format" : "TLV", "length" : "4"})
ies.append({ "iei" : "71", "value" : "Extended CAG information list", "type" : "Extended CAG information list", "reference" : "9.11.3.86", "presence" : "O", "format" : "TLV-E", "length" : "3-n"})
ies.append({ "iei" : "3A", "value" : "Lower bound timer value", "type" : "GPRS timer 3", "reference" : "9.11.2.5", "presence" : "O", "format" : "TLV", "length" : "3"})
ies.append({ "iei" : "1D", "value" : "Forbidden TAI for the list of 5GS forbidden tracking areas for roaming", "type" : "5GS tracking area identity list", "reference" : "9.11.3.9", "presence" : "O", "format" : "TLV", "length" : "9-114"})
ies.append({ "iei" : "1E", "value" : "Forbidden TAI for the list of 5GS forbidden tracking areas forregional provision of service", "type" : "5GS tracking area identity list", "reference" : "9.11.3.9", "presence" : "O", "format" : "TLV", "length" : "9-114"})
msg_list[key]["ies"] = ies
