ies = []
ies.append({ "ie_value" : "Cause", "presence" : "Mandatory", "reference" : "7.7.1"})
ies.append({ "ie_value" : "IMSI", "presence" : "Conditional", "reference" : "7.7.2"})
ies.append({ "ie_value" : "Authentication Triplet", "presence" : "Conditional", "reference" : "7.7.7"})
ies.append({ "ie_value" : "Authentication Quintuplet", "presence" : "Conditional", "reference" : "7.7.35"})
ies.append({ "ie_value" : "UE Usage Type", "presence" : "Optional", "reference" : "7.7.117"})
ies.append({ "ie_value" : "IOV_updates counter", "presence" : "Optional", "reference" : "7.7.122"})
msg_list[key]["ies"] = ies
