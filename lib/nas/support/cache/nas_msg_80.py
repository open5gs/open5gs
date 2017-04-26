ies = []
ies.append({ "iei" : "", "value" : "GUTI", "type" : "EPS mobile identity", "reference" : "9.9.3.12", "presence" : "M", "format" : "LV", "length" : "12"})
ies.append({ "iei" : "54", "value" : "TAI list", "type" : "Tracking area identity list", "reference" : "9.9.3.33", "presence" : "O", "format" : "TLV", "length" : "8-98"})
msg_list[key]["ies"] = ies
