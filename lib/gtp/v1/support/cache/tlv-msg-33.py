ies = []
ies.append({ "ie_value" : "Cause", "presence" : "Mandatory", "reference" : "7.7.1"})
ies.append({ "ie_value" : "IMSI", "presence" : "Mandatory", "reference" : "7.7.2"})
ies.append({ "ie_value" : "MAP Cause", "presence" : "Optional", "reference" : "7.7.8"})
ies.append({ "ie_value" : "MS not Reachable Reason", "presence" : "Optional", "reference" : "7.7.25A"})
ies.append({ "ie_value" : "GSN Address", "presence" : "Optional", "reference" : "7.7.32"})
msg_list[key]["ies"] = ies
