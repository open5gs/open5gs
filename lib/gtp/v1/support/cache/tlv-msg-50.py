ies = []
ies.append({ "ie_value" : "IMSI", "presence" : "Conditional", "reference" : "7.7.2"})
ies.append({ "ie_value" : "Routeing Area Identity", "presence" : "Mandatory", "reference" : "7.7.3"})
ies.append({ "ie_value" : "Temporary Logical Link Identifier", "presence" : "Conditional", "reference" : "7.7.4"})
ies.append({ "ie_value" : "Packet TMSI", "presence" : "Conditional", "reference" : "7.7.5"})
ies.append({ "ie_value" : "P-TMSI Signature", "presence" : "Conditional", "reference" : "7.7.9"})
ies.append({ "ie_value" : "MS Validated", "presence" : "Optional", "reference" : "7.7.10"})
ies.append({ "ie_value" : "Tunnel Endpoint Identifier Control Plane", "presence" : "Mandatory", "reference" : "7.7.14"})
ies.append({ "ie_value" : "SGSN Address for Control Plane", "presence" : "Mandatory", "reference" : "7.7.32"})
ies.append({ "ie_value" : "Alternative SGSN Address for Control Plane", "presence" : "Optional", "reference" : "7.7.32"})
ies.append({ "ie_value" : "SGSN Number", "presence" : "Optional", "reference" : "7.7.47"})
ies.append({ "ie_value" : "RAT Type", "presence" : "Optional", "reference" : "7.7.50"})
ies.append({ "ie_value" : "Hop Counter", "presence" : "Optional", "reference" : "7.7.63"})
msg_list[key]["ies"] = ies
