ies = []
ies.append({ "ie_value" : "Cause", "presence" : "Mandatory", "reference" : "7.7.1"})
ies.append({ "ie_value" : "Tunnel Endpoint Identifier Control Plane", "presence" : "Mandatory", "reference" : "7.7.14"})
ies.append({ "ie_value" : "End User Address", "presence" : "Mandatory", "reference" : "7.7.27"})
ies.append({ "ie_value" : "Access Point Name", "presence" : "Mandatory", "reference" : "7.7.30"})
ies.append({ "ie_value" : "Protocol Configuration Options", "presence" : "Optional", "reference" : "7.7.31"})
msg_list[key]["ies"] = ies
