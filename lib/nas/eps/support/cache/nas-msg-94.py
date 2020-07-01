ies = []
ies.append({ "iei" : "23", "value" : "IMEISV", "type" : "Mobile identity", "reference" : "9.9.2.3", "presence" : "O", "format" : "TLV", "length" : "11"})
ies.append({ "iei" : "79", "value" : "Replayed NAS message container", "type" : "Replayed NAS message container", "reference" : "9.9.3.51", "presence" : "O", "format" : "TLV-E", "length" : "3-n"})
ies.append({ "iei" : "66", "value" : "UE radio capability ID", "type" : "UE radio capability ID", "reference" : "9.9.3.60", "presence" : "O", "format" : "TLV", "length" : "3-n"})
msg_list[key]["ies"] = ies
