ies = []
ies.append({ "ie_type" : "Cause", "ie_value" : "Cause", "presence" : "M", "tlv_more" : "0", "comment" : "This IE shall indicate the acceptance or the rejection of the corresponding request message."})
ies.append({ "ie_type" : "Offending IE", "ie_value" : "Offending IE", "presence" : "C", "tlv_more" : "0", "comment" : "This IE shall be included if the rejection is due to an conditional or mandatory IE missing or faulty."})
ies.append({ "ie_type" : "Load Control Information", "ie_value" : "Load Control Information", "presence" : "O", "tlv_more" : "0", "comment" : "The UP function may include this IE if it supports the load control feature and the feature is activated in the network.See Table 7.5.3.3-1."})
ies.append({ "ie_type" : "Overload Control Information", "ie_value" : "Overload Control Information", "presence" : "O", "tlv_more" : "0", "comment" : "During an overload condition, the UP function may include this IE if it supports the overload control feature and the feature is activated in the network.See Table 7.5.3.4-1."})
type_list["Usage Report Session Deletion Response"]["max_tlv_more"] = "7"
ies.append({ "ie_type" : "Usage Report Session Deletion Response", "ie_value" : "Usage Report", "presence" : "C", "tlv_more" : "7", "comment" : "This IE shall be present if a URR had been provisioned in the UP function for the PFCP session being deleted and traffic usage measurements for that URR are available at the UP function.Several IEs within the same IE type may be present to represent a list of Usage Reports."})
msg_list[key]["ies"] = ies
