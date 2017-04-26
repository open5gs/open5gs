ies = []
ies.append({ "iei" : "", "value" : "Generic message container type", "type" : "Generic message container type", "reference" : "9.9.3.42", "presence" : "M", "format" : "V", "length" : "1"})
ies.append({ "iei" : "", "value" : "Generic message container", "type" : "Generic message container", "reference" : "9.9.3.43", "presence" : "M", "format" : "LV-E", "length" : "3-n"})
ies.append({ "iei" : "65", "value" : "Additional information", "type" : "Additional information", "reference" : "9.9.2.0", "presence" : "O", "format" : "TLV", "length" : "3-n"})
msg_list[key]["ies"] = ies
