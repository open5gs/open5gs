ies = []
ies.append({ "iei" : "", "value" : "Linked EPS bearer identity", "type" : "Linked EPS bearer identity", "reference" : "9.9.4.6", "presence" : "M", "format" : "V", "length" : "1/2"})
ies.append({ "iei" : "", "value" : "Traffic flow aggregate", "type" : "Traffic flow aggregate description", "reference" : "9.9.4.15", "presence" : "M", "format" : "LV", "length" : "2-256"})
ies.append({ "iei" : "", "value" : "Required traffic flow QoS", "type" : "EPS quality of service", "reference" : "9.9.4.3", "presence" : "M", "format" : "LV", "length" : "2-14"})
ies.append({ "iei" : "27", "value" : "Protocol configuration options", "type" : "Protocol configuration options", "reference" : "9.9.4.11", "presence" : "O", "format" : "TLV", "length" : "3-253"})
ies.append({ "iei" : "C-", "value" : "Device properties", "type" : "Device properties", "reference" : "9.9.2.0A", "presence" : "O", "format" : "TV", "length" : "1"})
ies.append({ "iei" : "33", "value" : "NBIFOM container", "type" : "NBIFOM container", "reference" : "9.9.4.19", "presence" : "O", "format" : "TLV", "length" : "3-257"})
ies.append({ "iei" : "7B", "value" : "Extended protocol configuration options", "type" : "Extended protocol configuration options", "reference" : "9.9.4.26", "presence" : "O", "format" : "TLV-E", "length" : "4-65538"})
ies.append({ "iei" : "5C", "value" : "Extended EPS QoS", "type" : "Extended quality of service", "reference" : "9.9.4.30", "presence" : "O", "format" : "TLV", "length" : "12"})
msg_list[key]["ies"] = ies
