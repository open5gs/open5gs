ies = []
ies.append({ "ie_value" : "Cause", "presence" : "Mandatory", "reference" : "7.7.1"})
ies.append({ "ie_value" : "Tunnel Endpoint Identifier Data I", "presence" : "Optional", "reference" : "7.7.13"})
ies.append({ "ie_value" : "Tunnel Endpoint Identifier Control Plane", "presence" : "Optional", "reference" : "7.7.14"})
ies.append({ "ie_value" : "SGSN Address for Data I", "presence" : "Optional", "reference" : "7.7.32"})
ies.append({ "ie_value" : "SGSN Address for Control Plane", "presence" : "Optional", "reference" : "7.7.32"})
msg_list[key]["ies"] = ies
