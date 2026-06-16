ies = []
ies.append({ "ie_type" : "Cause", "ie_value" : "Cause", "presence" : "M", "tlv_more" : "0", "comment" : "This IE shall indicate the acceptance, partial accetance or  rejection of the corresponding request message."})
ies.append({ "ie_type" : "Offending IE", "ie_value" : "Offending IE", "presence" : "C", "tlv_more" : "0", "comment" : "This IE shall be included if the rejection is due to a conditional or mandatory IE missing or faulty."})
ies.append({ "ie_type" : "Node ID", "ie_value" : "Node ID", "presence" : "O", "tlv_more" : "0", "comment" : "When present, this IE shall contain the unique identifier of the sending Node."})
ies.append({ "ie_type" : "PFD Partial Failure Information", "ie_value" : "PFD Partial Failure Information", "presence" : "C", "tlv_more" : "0", "comment" : "This IE shall be present if the Cause IE indicates partial acceptance of the request to provide failure information related to a failed provisioning of one or more Application IDs PFDs. See Table Table 7.4.3.2-2.Several IEs within the same IE type may be present to report failures related to multiple Application IDs PFDs."})
msg_list[key]["ies"] = ies
