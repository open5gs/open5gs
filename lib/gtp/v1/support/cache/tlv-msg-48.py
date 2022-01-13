ies = []
ies.append({ "ie_value" : "Routeing Area Identity", "presence" : "Mandatory", "reference" : "7.7.3"})
ies.append({ "ie_value" : "Packet TMSI", "presence" : "Mandatory", "reference" : "7.7.5"})
ies.append({ "ie_value" : "P-TMSI Signature", "presence" : "Conditional", "reference" : "7.7.9"})
ies.append({ "ie_value" : "SGSN Address for Control Plane", "presence" : "Optional", "reference" : "7.7.32"})
ies.append({ "ie_value" : "Hop Counter", "presence" : "Optional", "reference" : "7.7.63"})
msg_list[key]["ies"] = ies
