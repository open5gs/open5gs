ies = []
ies.append({ "ie_type" : "Cause", "ie_value" : "Cause", "presence" : "M", "tlv_more" : "0", "comment" : "This IE shall indicate the acceptance or the rejection of the corresponding request message."})
ies.append({ "ie_type" : "Offending IE", "ie_value" : "Offending IE", "presence" : "C", "tlv_more" : "0", "comment" : "This IE shall be included if the rejection is due to a conditional or mandatory IE missing or faulty."})
ies.append({ "ie_type" : "Created PDR", "ie_value" : "Created PDR", "presence" : "C", "tlv_more" : "7", "comment" : "This IE shall be present if the cause is set to success, new PDR(s) were requested to be created and the UP function was requested to allocate the local F-TEID or a UE IP address/prefix for the PDR(s).When present, this IE shall contain the PDR information associated to the PFCP session.See Table 7.5.3.2-1."})
ies.append({ "ie_type" : "Load Control Information", "ie_value" : "Load Control Information", "presence" : "O", "tlv_more" : "0", "comment" : "The UP function may include this IE if it supports the load control feature and the feature is activated in the network.See Table 7.5.3.3-1."})
ies.append({ "ie_type" : "Overload Control Information", "ie_value" : "Overload Control Information", "presence" : "O", "tlv_more" : "0", "comment" : "During an overload condition, the UP function may include this IE if it supports the overload control feature and the feature is activated in the network."})
type_list["Usage Report Session Modification Response"]["max_tlv_more"] = "7"
ies.append({ "ie_type" : "Usage Report Session Modification Response", "ie_value" : "Usage Report", "presence" : "C", "tlv_more" : "7", "comment" : "This IE shall be present if:	- the Query URR IE was present or the QAURR flag was set to 1 in the PFCP Session Modification Request,	- traffic usage measurements for that URR are available at the UP function, and	- the UP function decides to return some or all of the requested usage reports in the PFCP Session Modification Response.This IE shall be also present if:	- a URR or the last PDR associated to a URR has been removed,	- non-null traffic usage measurements for that URR are available in the UP function, and	- the UP function decides to return some or all of the related usage reports in the PFCP Session Modification Response (see clause5.2.2.3.1).Several IEs within the same IE type may be present to represent a list of Usage Reports."})
ies.append({ "ie_type" : "Failed Rule ID", "ie_value" : "Failed Rule ID", "presence" : "C", "tlv_more" : "0", "comment" : "This IE shall be included if the Cause IE indicates a rejection due to a rule creation or modification failure."})
ies.append({ "ie_type" : "Additional Usage Reports Information", "ie_value" : "Additional Usage Reports Information", "presence" : "C", "tlv_more" : "0", "comment" : "This IE shall be included if the Query URR IE was present or the QAURR flag was set to 1 in the PFCP Session Modification Request, and usage reports need to be sent in additional PFCP Session Report Request messages (see clause5.2.2.3.1).When present, this IE shall either indicate that additional usage reports will follow, or indicate the total number of usage reports that need to be sent in PFCP Session Report Request messages."})
ies.append({ "ie_type" : "Created Traffic Endpoint", "ie_value" : "Created/Updated Traffic Endpoint", "presence" : "C", "tlv_more" : "0", "comment" : "This IE shall be present if the cause is set to success, Traffic Endpoint(s) were requested to be created or updated, and the UP function was requested to allocate the local F-TEID or a UE IP address/prefix for the Traffic Endpoint(s).If the UP function allocates additional UE IP address/prefix (upon receiving a Create Traffic Endpoint or Update Traffic Endpoint in the corresponding PFCP Session Modification Request message from the CP function), this IE shall be present and shall contain the complete list of UE IP address / prefix assigned by the UP function for this PFCP session.In the 5GC, several IEs with the same IE type may be present to represent multiple UE IP addresses, if the UPF indicated support of the IP6PL feature (see clause5.21).(NOTE 1)When present, this IE shall contain the Traffic Endpoint information associated to the PFCP session.See Table7.5.3.5-1."})
ies.append({ "ie_type" : "TSC Management Information IE within PFCP Session Modification Response", "ie_value" : "TSC Management Information", "presence" : "C", "tlv_more" : "0", "comment" : "This IE shall be present if the UPF needs to send TSC Management information to the SMF.Several IEs within the same IE type may be present to transfer PMICs for different NW-TT ports."})
ies.append({ "ie_type" : "ATSSS Control Parameters", "ie_value" : "ATSSS Control Parameters", "presence" : "C", "tlv_more" : "0", "comment" : "This IE shall be present if ATSSS functionality is required in the request message, and the UPF allocates the resources and parameters corresponding to the required ATSSS functionality.See Table7.5.3.7-1."})
ies.append({ "ie_type" : "Updated PDR", "ie_value" : "Updated PDR", "presence" : "C", "tlv_more" : "0", "comment" : "This IE shall be present if a Update PDR is present in the corresponding PFCP Session Modification Request and UP function is requested to allocate a new F-TEID, e.g. to support the redundant transmission on N3/N9 interfaces, or move the application traffic from a default bearer to a new dedicated bearer, or the UP function is requested to assign additional UE IP Address or Prefix, e.g. a shorter than /64 prefix delegation. See Table7.5.5.5-1.Several IEs within the same IE type may be present to represent a list of PDRs to update."})
ies.append({ "ie_type" : "Packet Rate Status Report IE within PFCP Session Modification Response", "ie_value" : "Packet Rate Status Report", "presence" : "C", "tlv_more" : "0", "comment" : "This IE shall be present if the CP function has requested to report an immediate packet rate status in the PFCP Session Modification Request and the UP function supports the CIOT feature (see clause8.2.25).Several IEs within the same IE type may be present to represent a list of Packet Rate Status Reports."})
msg_list[key]["ies"] = ies
