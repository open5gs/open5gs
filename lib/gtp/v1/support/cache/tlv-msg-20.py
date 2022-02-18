ies = []
ies.append({ "ie_value" : "Cause", "presence" : "Optional", "reference" : "7.7.1"})
ies.append({ "ie_value" : "Teardown Ind", "presence" : "Conditional", "reference" : "7.7.16"})
ies.append({ "ie_value" : "NSAPI", "presence" : "Mandatory", "reference" : "7.7.17"})
ies.append({ "ie_value" : "Protocol Configuration Options", "presence" : "Optional", "reference" : "7.7.31"})
ies.append({ "ie_value" : "User Location Information", "presence" : "Optional", "reference" : "7.7.51"})
ies.append({ "ie_value" : "MS Time Zone", "presence" : "Optional", "reference" : "7.7.52"})
ies.append({ "ie_value" : "Extended Common Flags", "presence" : "Optional", "reference" : "7.7.93"})
ies.append({ "ie_value" : "ULI Timestamp", "presence" : "Optional", "reference" : "7.7.114"})
msg_list[key]["ies"] = ies
