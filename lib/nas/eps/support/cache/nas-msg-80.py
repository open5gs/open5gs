ies = []
ies.append({ "iei" : "", "value" : "GUTI", "type" : "EPS mobile identity", "reference" : "9.9.3.12", "presence" : "M", "format" : "LV", "length" : "12"})
ies.append({ "iei" : "54", "value" : "TAI list", "type" : "Tracking area identity list", "reference" : "9.9.3.33", "presence" : "O", "format" : "TLV", "length" : "8-98"})
ies.append({ "iei" : "65", "value" : "DCN-ID", "type" : "DCN-ID", "reference" : "9.9.3.48", "presence" : "O", "format" : "TLV", "length" : "4"})
ies.append({ "iei" : "66", "value" : "UE radio capability ID", "type" : "UE radio capability ID", "reference" : "9.9.3.60", "presence" : "O", "format" : "TLV", "length" : "3-n"})
ies.append({ "iei" : "B-", "value" : "UE radio capability ID deletion indication", "type" : "UE radio capability ID deletion indication", "reference" : "9.9.3.61", "presence" : "O", "format" : "TV", "length" : "1"})
msg_list[key]["ies"] = ies
