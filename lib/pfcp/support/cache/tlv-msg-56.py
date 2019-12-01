ies = []
ies.append({ "ie_type" : "Report Type", "ie_value" : "Report Type", "presence" : "M", "instance" : "0", "comment" : "This IE shall indicate the type of the report."})
ies.append({ "ie_type" : "Downlink Data Report", "ie_value" : "Downlink Data Report", "presence" : "C", "instance" : "0", "comment" : "This IE shall be present if the Report Type indicates a Downlink Data Report. "})
ies.append({ "ie_type" : "Usage Report Session Report Request", "ie_value" : "Usage Report", "presence" : "C", "instance" : "0", "comment" : "This IE shall be present if the Report Type indicates a Usage Report. Several IEs within the same IE type may be present to represent a list of Usage Reports."})
ies.append({ "ie_type" : "Error Indication Report", "ie_value" : "Error Indication Report", "presence" : "C", "instance" : "0", "comment" : "This IE shall be present if the Report Type indicates an Error Indication Report. "})
ies.append({ "ie_type" : "Load Control Informationp", "ie_value" : "Load Control Information", "presence" : "O", "instance" : "0", "comment" : "The UP function may include this IE if it supports the load control feature and the feature is activated in the network.See Table 7.5.3.3-1."})
ies.append({ "ie_type" : "Overload Control Informationp", "ie_value" : "Overload Control Information", "presence" : "O", "instance" : "0", "comment" : "During an overload condition, the UP function may include this IE if it supports the overload control feature and the feature is activated in the network.See Table 7.5.3.4-1."})
msg_list[key]["ies"] = ies
