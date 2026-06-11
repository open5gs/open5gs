ies = []
ies.append({ "iei" : "", "value" : "De-registration type", "type" : "De-registration type", "reference" : "9.11.3.20", "presence" : "M", "format" : "V", "length" : "1/2"})
ies.append({ "iei" : "", "value" : "5GS mobile identity", "type" : "5GS mobile identity", "reference" : "9.11.3.4", "presence" : "M", "format" : "LV-E", "length" : "6-n"})
ies.append({ "iei" : "3C", "value" : "Unavailability information", "type" : "Unavailability information", "reference" : "9.11.2.20", "presence" : "O", "format" : "TLV", "length" : "3-9"})
ies.append({ "iei" : "71", "value" : "NAS message container", "type" : "message container", "reference" : "9.11.3.33", "presence" : "O", "format" : "TLV-E", "length" : "4-n"})
msg_list[key]["ies"] = ies
