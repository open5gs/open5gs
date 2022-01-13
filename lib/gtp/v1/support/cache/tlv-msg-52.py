ies = []
ies.append({ "ie_value" : "Cause", "presence" : "Mandatory", "reference" : "7.7.1"})
ies.append({ "ie_value" : "Tunnel Endpoint Identifier Data II", "presence" : "Conditional", "reference" : "7.7.15"})
ies.append({ "ie_value" : "SGSN Address for user traffic", "presence" : "Conditional", "reference" : "7.7.32"})
ies.append({ "ie_value" : "SGSN Number", "presence" : "Optional", "reference" : "7.7.47"})
ies.append({ "ie_value" : "Node Identifier", "presence" : "Optional", "reference" : "7.7.119"})
msg_list[key]["ies"] = ies
