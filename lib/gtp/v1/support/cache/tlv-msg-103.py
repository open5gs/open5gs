ies = []
ies.append({ "ie_value" : "Cause", "presence" : "Mandatory", "reference" : "7.7.1"})
ies.append({ "ie_value" : "Recovery", "presence" : "Optional", "reference" : "7.7.11"})
ies.append({ "ie_value" : "Tunnel Endpoint Identifier Control Plane", "presence" : "Conditional", "reference" : "7.7.14"})
ies.append({ "ie_value" : "Charging ID", "presence" : "Conditional", "reference" : "7.7.26"})
ies.append({ "ie_value" : "GGSN Address for Control Plane", "presence" : "Conditional", "reference" : "7.7.32"})
ies.append({ "ie_value" : "Alternative GGSN Address for Control Plane", "presence" : "Conditional", "reference" : "7.7.32"})
ies.append({ "ie_value" : "Charging Gateway Address", "presence" : "Optional", "reference" : "7.7.44"})
ies.append({ "ie_value" : "Alternative Charging Gateway Address", "presence" : "Optional", "reference" : "7.7.44"})
msg_list[key]["ies"] = ies
