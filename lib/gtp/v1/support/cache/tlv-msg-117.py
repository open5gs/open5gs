ies = []
ies.append({ "ie_value" : "Cause", "presence" : "Mandatory", "reference" : "7.7.1"})
ies.append({ "ie_value" : "Recovery", "presence" : "Optional", "reference" : "7.7.11"})
ies.append({ "ie_value" : "Tunnel Endpoint Identifier Data I", "presence" : "Conditional", "reference" : "7.7.13"})
ies.append({ "ie_value" : "Tunnel Endpoint Identifier Control Plane", "presence" : "Conditional", "reference" : "7.7.14"})
ies.append({ "ie_value" : "SGSN Address for Control Plane", "presence" : "Conditional", "reference" : "7.7.32"})
ies.append({ "ie_value" : "SGSN Address for user traffic", "presence" : "Conditional", "reference" : "7.7.32"})
ies.append({ "ie_value" : "Alternative SGSN Address for user traffic", "presence" : "Optional", "reference" : "7.7.32"})
ies.append({ "ie_value" : "MBMS Distribution Acknowledgement", "presence" : "Optional", "reference" : "7.7.86"})
msg_list[key]["ies"] = ies
