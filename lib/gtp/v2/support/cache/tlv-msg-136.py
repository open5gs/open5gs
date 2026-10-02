ies = []
ies.append({ "ie_type" : "Cause", "ie_value" : "Cause", "presence" : "M", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "Recovery", "ie_value" : "Recovery", "presence" : "O", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "Secondary RAT Usage Data Report", "ie_value" : "Secondary RAT Usage Data Report", "presence" : "CO", "instance" : "0", "comment" : "If the PLMN has configured secondary RAT usage reporting, the MME shall include this IE on the S10 interface if it has received Secondary RAT usage data from the eNodeB in an S1-based handover with MME relocation.Several IEs with the same type and instance value may be included, to represent multiple usage data reports."})
type_list["Secondary RAT Usage Data Report"]["max_instance"] = "1"
ies.append({ "ie_type" : "Secondary RAT Usage Data Report", "ie_value" : "Secondary RAT Usage Data Report from NG-RAN", "presence" : "CO", "instance" : "1", "comment" : "This IE shall be included by a source AMF if it has received Secondary RAT Usage Report Data message from the source NG-RAN after sending a Handover Command message.The AMF shall set the EBI field in the Secondary RAT Usage Data Report to any EBI pertaining to the same PDU session.Several IEs with the same type and instance value may be included, to represent multiple usage data reports."})
msg_list[key]["ies"] = ies
