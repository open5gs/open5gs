ies = []
ies.append({ "ie_value" : "IMSI", "presence" : "Conditional", "reference" : "7.7.2"})
ies.append({ "ie_value" : "IMEI", "presence" : "Conditional", "reference" : "7.7.53"})
ies.append({ "ie_value" : "Extended Common Flags", "presence" : "Optional", "reference" : "7.7.93"})
ies.append({ "ie_value" : "Extended RANAP Cause", "presence" : "Optional", "reference" : "7.7.111"})
msg_list[key]["ies"] = ies
