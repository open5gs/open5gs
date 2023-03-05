ies = []
ies.append({ "ie_value" : "IMSI", "presence" : "Conditional", "reference" : "7.7.2"})
ies.append({ "ie_value" : "Tunnel Endpoint Identifier Control Plane", "presence" : "Conditional", "reference" : "7.7.14"})
ies.append({ "ie_value" : "End User Address", "presence" : "Conditional", "reference" : "7.7.27"})
ies.append({ "ie_value" : "Access Point Name", "presence" : "Conditional", "reference" : "7.7.30"})
ies.append({ "ie_value" : "MBMS Protocol Configuration Options", "presence" : "Optional", "reference" : "7.7.58"})
ies.append({ "ie_value" : "Enhanced NSAPI", "presence" : "Conditional", "reference" : "7.7.67"})
msg_list[key]["ies"] = ies
