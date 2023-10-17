ies = []
ies.append({ "iei" : "", "value" : "EMM cause", "type" : "EMM cause", "reference" : "9.9.3.9", "presence" : "M", "format" : "V", "length" : "1"})
ies.append({ "iei" : "5F", "value" : "T3346 value", "type" : "GPRS timer 2", "reference" : "9.9.3.16A", "presence" : "O", "format" : "TLV", "length" : "3"})
ies.append({ "iei" : "A-", "value" : "Extended EMM cause", "type" : "Extended EMM cause", "reference" : "9.9.3.26A", "presence" : "O", "format" : "TV", "length" : "1"})
ies.append({ "iei" : "1C", "value" : "Lower bound timer value", "type" : "GPRS timer 3", "reference" : "9.9.3.16B", "presence" : "O", "format" : "TLV", "length" : "3"})
ies.append({ "iei" : "1D", "value" : "Forbidden TAI for the list of forbidden tracking areas for roaming", "type" : "Tracking area identity list", "reference" : "9.9.3.33", "presence" : "O", "format" : "TLV", "length" : "9-98"})
ies.append({ "iei" : "1E", "value" : "Forbidden TAI for the list of forbidden tracking areas forregional provision of service", "type" : "Tracking area identity list", "reference" : "9.9.3.33", "presence" : "O", "format" : "TLV", "length" : "9-98"})
msg_list[key]["ies"] = ies
