ies = []
ies.append({ "ie_value" : "Tunnel Endpoint Identifier Control Plane", "presence" : "Conditional", "reference" : "7.7.14"})
ies.append({ "ie_value" : "End User Address", "presence" : "Mandatory", "reference" : "7.7.27"})
ies.append({ "ie_value" : "Access Point Name", "presence" : "Mandatory", "reference" : "7.7.30"})
ies.append({ "ie_value" : "SGSN Address for Control Plane", "presence" : "Conditional", "reference" : "7.7.32"})
ies.append({ "ie_value" : "Alternative SGSN Address for Control Plane", "presence" : "Optional", "reference" : "7.7.32"})
msg_list[key]["ies"] = ies
