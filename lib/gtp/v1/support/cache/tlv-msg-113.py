ies = []
ies.append({ "ie_value" : "Cause", "presence" : "Mandatory", "reference" : "7.7.1"})
ies.append({ "ie_value" : "Tunnel Endpoint Identifier Control Plane", "presence" : "Conditional", "reference" : "7.7.14"})
ies.append({ "ie_value" : "GGSN Address for Control Plane", "presence" : "Conditional", "reference" : "7.7.32"})
ies.append({ "ie_value" : "Temporary Mobile Group Identity", "presence" : "Conditional", "reference" : "7.7.56"})
ies.append({ "ie_value" : "Required MBMS bearer capabilities", "presence" : "Conditional", "reference" : "7.7.76"})
msg_list[key]["ies"] = ies
