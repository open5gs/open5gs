ies = []
ies.append({ "ie_value" : "IMSI", "presence" : "Mandatory", "reference" : "7.7.2"})
ies.append({ "ie_value" : "Tunnel Endpoint Identifier Control Plane", "presence" : "Mandatory", "reference" : "7.7.14"})
ies.append({ "ie_value" : "End User Address", "presence" : "Mandatory", "reference" : "7.7.27"})
ies.append({ "ie_value" : "Access Point Name", "presence" : "Mandatory", "reference" : "7.7.30"})
ies.append({ "ie_value" : "Protocol Configuration Options", "presence" : "Optional", "reference" : "7.7.31"})
ies.append({ "ie_value" : "GGSN Address for Control Plane", "presence" : "Mandatory", "reference" : "7.7.32"})
msg_list[key]["ies"] = ies
