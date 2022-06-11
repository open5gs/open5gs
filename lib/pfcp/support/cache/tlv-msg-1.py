ies = []
ies.append({ "ie_type" : "Recovery Time Stamp", "ie_value" : "Recovery Time Stamp", "presence" : "M", "tlv_more" : "0", "comment" : "This IE shall contain the time stamp when the PFCP entity was started see clause19A of 3GPPTS23.007[24]."})
ies.append({ "ie_type" : "Source IP Address", "ie_value" : "Source IP Address", "presence" : "O", "tlv_more" : "0", "comment" : "This IE may be included when a Network Address Translation device is deployed in the network. See clause19a in 3GPPTS23.007[24]. "})
msg_list[key]["ies"] = ies
