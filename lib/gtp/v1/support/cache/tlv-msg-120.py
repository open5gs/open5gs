ies = []
ies.append({ "ie_value" : "Tunnel Endpoint Identifier Control Plane", "presence" : "Optional", "reference" : "7.7.14"})
ies.append({ "ie_value" : "End User Address", "presence" : "Mandatory", "reference" : "7.7.27"})
ies.append({ "ie_value" : "Access Point Name", "presence" : "Mandatory", "reference" : "7.7.30"})
ies.append({ "ie_value" : "GGSN Address for Control Plane", "presence" : "Optional", "reference" : "7.7.32"})
ies.append({ "ie_value" : "Temporary Mobile Group Identity", "presence" : "Mandatory", "reference" : "7.7.56"})
ies.append({ "ie_value" : "MBMS Session Duration", "presence" : "Mandatory", "reference" : "7.7.59"})
ies.append({ "ie_value" : "MBMS Service Area", "presence" : "Mandatory", "reference" : "7.7.60"})
ies.append({ "ie_value" : "MBMS Session Identifier", "presence" : "Optional", "reference" : "7.7.65"})
ies.append({ "ie_value" : "MBMS Session Repetition Number", "presence" : "Optional", "reference" : "7.7.69"})
ies.append({ "ie_value" : "MBMS Flow Identifier", "presence" : "Optional", "reference" : "7.7.84"})
msg_list[key]["ies"] = ies
