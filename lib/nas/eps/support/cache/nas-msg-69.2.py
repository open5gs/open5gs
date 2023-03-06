ies = []
ies.append({ "iei" : "", "value" : "Detach type", "type" : "Detach type", "reference" : "9.9.3.7", "presence" : "M", "format" : "V", "length" : "1/2"})
ies.append({ "iei" : "53", "value" : "EMM cause", "type" : "EMM cause", "reference" : "9.9.3.9", "presence" : "O", "format" : "TV", "length" : "2"})
ies.append({ "iei" : "1C", "value" : "Lower bound timer value", "type" : "GPRS timer 3", "reference" : "9.9.3.16B", "presence" : "O", "format" : "TLV", "length" : "3"})
ies.append({ "iei" : "1D", "value" : "Forbidden TAI for the list of forbidden tracking areas for roaming", "type" : "Tracking area identity list", "reference" : "9.9.3.33", "presence" : "O", "format" : "TLV", "length" : "9-98"})
ies.append({ "iei" : "1E", "value" : "Forbidden TAI for the list of forbidden tracking areas forregional provision of service", "type" : "Tracking area identity list", "reference" : "9.9.3.33", "presence" : "O", "format" : "TLV", "length" : "9-98"})
msg_list[key]["ies"] = ies
