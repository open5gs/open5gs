ies = []
ies.append({ "ie_value" : "Cause", "presence" : "Mandatory", "reference" : "7.7.1"})
ies.append({ "ie_value" : "IMSI", "presence" : "Mandatory", "reference" : "7.7.2"})
ies.append({ "ie_value" : "Selected PLMN ID", "presence" : "Conditional", "reference" : "7.7.64"})
msg_list[key]["ies"] = ies
