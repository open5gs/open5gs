# [Table 7.2.1-2: Bearer Context to be created within Create Session Request] Index = 11
ies = []
paragraph = "Table 7.2.1-2: Bearer Context to be created within Create Session Request"
ies.append({ "ie_type" : "EBI", "ie_value" : "EPS Bearer ID", "presence" : "M", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "Bearer TFT", "ie_value" : "TFT", "presence" : "O", "instance" : "0", "comment" : "This IE may be included on the S4/S11 interfaces."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "S1-U eNodeB F-TEID", "presence" : "C", "instance" : "0", "comment" : "This IE shall be included on the S11 interface for X2-based handover with SGW relocation."})
type_list["F-TEID"]["max_instance"] = "1"
ies.append({ "ie_type" : "F-TEID", "ie_value" : "S4-U SGSN F-TEID", "presence" : "C", "instance" : "1", "comment" : "This IE shall be included on the S4 interface if the S4-U interface is used."})
type_list["F-TEID"]["max_instance"] = "2"
ies.append({ "ie_type" : "F-TEID", "ie_value" : "S5/S8-U SGW F-TEID", "presence" : "C", "instance" : "2", "comment" : "This IE shall be included on the S5/S8 interface for an E-UTRAN Initial Attach, a Handover from Trusted or Untrusted Non-3GPP IP Access to E-UTRAN, a PDP Context Activation, a Handover from Trusted or Untrusted Non-3GPP IP Access to UTRAN/GERAN or a UE Requested PDN Connectivity."})
type_list["F-TEID"]["max_instance"] = "3"
ies.append({ "ie_type" : "F-TEID", "ie_value" : "S5/S8-U PGW F-TEID", "presence" : "C", "instance" : "3", "comment" : "This IE shall be included on the S4 and S11 interfaces for the TAU/RAU/Handover cases when the GTP-based S5/S8 is used."})
type_list["F-TEID"]["max_instance"] = "4"
ies.append({ "ie_type" : "F-TEID", "ie_value" : "S12 RNC F-TEID", "presence" : "CO", "instance" : "4", "comment" : "This IE shall be included on the S4 interface if the S12 interface is used in the Enhanced serving RNS relocation with SGW relocation procedure."})
type_list["F-TEID"]["max_instance"] = "5"
ies.append({ "ie_type" : "F-TEID", "ie_value" : "S2b-U ePDG F-TEID", "presence" : "C", "instance" : "5", "comment" : "This IE shall be included on the S2b interface for an Attach with GTP on S2b, a UE initiated Connectivity to Additional PDN with GTP on S2b, a Handover to Untrusted Non-3GPP IP Access with GTP on S2b and an Initial Attach for emergency session (GTP on S2b)."})
type_list["F-TEID"]["max_instance"] = "6"
ies.append({ "ie_type" : "F-TEID", "ie_value" : "S2a-U TWAN F-TEID", "presence" : "C", "instance" : "6", "comment" : "This IE shall be included on the S2a interface for an Initial Attach in WLAN on GTP S2a, an Initial Attach in WLAN for Emergency Service on GTP S2a, a UE initiated Connectivity to Additional PDN with GTP on S2a and a Handover to TWAN with GTP on S2a."})
ies.append({ "ie_type" : "Bearer QoS", "ie_value" : "Bearer Level QoS", "presence" : "M", "instance" : "0", "comment" : ""})
type_list["F-TEID"]["max_instance"] = "7"
ies.append({ "ie_type" : "F-TEID", "ie_value" : "S11-U MME F-TEID", "presence" : "CO", "instance" : "7", "comment" : "This IE shall be sent on the S11 interface, if S11-U is being used, during the E-UTRAN Initial Attach and UE requested PDN connectivity procedures. This IE may also be sent on the S11 interface, if S11-U is being used, during a Tracking Area Update procedure with Serving GW change, if the MME needs to establish the S11-U tunnel. See NOTE 2."})
group_list[("Bearer Context", "Create Session Request", "created")] = { "index" : "193", "type" : "93", "context" : "Create Session Request", "action" : "created", "paragraph" : paragraph, "ies" : ies }
# [Table 7.2.1-3: Bearer Context to be removed within Create Session Request] Index = 12
ies = []
paragraph = "Table 7.2.1-3: Bearer Context to be removed within Create Session Request"
ies.append({ "ie_type" : "EBI", "ie_value" : "EPS Bearer ID", "presence" : "M", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "S4-U SGSN F-TEID", "presence" : "C", "instance" : "0", "comment" : "This IE shall be sent on the S4 interface if the S4-U interface is used. See NOTE 1."})
group_list[("Bearer Context", "Create Session Request", "removed")] = { "index" : "193", "type" : "93", "context" : "Create Session Request", "action" : "removed", "paragraph" : paragraph, "ies" : ies }
# [Table 7.2.1-4: Overload Control Information within Create Session Request] Index = 13
ies = []
paragraph = "Table 7.2.1-4: Overload Control Information within Create Session Request"
ies.append({ "ie_type" : "Sequence Number", "ie_value" : "Overload Control Sequence Number", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.1 for the description and use of this parameter."})
ies.append({ "ie_type" : "Metric", "ie_value" : "Overload Reduction Metric", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.3 for the description and use of this parameter."})
ies.append({ "ie_type" : "EPC Timer", "ie_value" : "Period of Validity", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.2 for the description and use of this parameter.This IE should be set to 0 if the Overload Reduction Metric is null. This IE shall be ignored by the receiver if the Overload Reduction Metric is null."})
group_list[("Overload Control Information", "Create Session Request", "")] = { "index" : "280", "type" : "180", "context" : "Create Session Request", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 7.2.1-5: Remote UE Context Connected within Create Session Request] Index = 14
ies = []
paragraph = "Table 7.2.1-5: Remote UE Context Connected within Create Session Request"
ies.append({ "ie_type" : "Remote User ID", "ie_value" : "Remote User ID", "presence" : "M", "instance" : "0", "comment" : "See clause 8.123 for the description and use of this parameter"})
ies.append({ "ie_type" : "Remote UE IP Information", "ie_value" : "Remote UE IP Information", "presence" : "M", "instance" : "0", "comment" : "See clause 8.124 for the description and use of this parameter"})
group_list[("Remote UE Context", "Create Session Request", "")] = { "index" : "291", "type" : "191", "context" : "Create Session Request", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 7.2.2-2: Bearer Context Created within Create Session Response] Index = 16
ies = []
paragraph = "Table 7.2.2-2: Bearer Context Created within Create Session Response"
ies.append({ "ie_type" : "EBI", "ie_value" : "EPS Bearer ID", "presence" : "M", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "Cause", "ie_value" : "Cause", "presence" : "M", "instance" : "0", "comment" : "This IE shall indicate if the bearer handling was successful, and if not, it gives information on the reason. (NOTE 1, NOTE 2, NOTE 3)"})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "S1-U SGW F-TEID", "presence" : "C", "instance" : "0", "comment" : "This IE shall be included on the S11 interface if the S1-U interface is used, i.e. if the S11-U Tunnel flag was not set in the Create Session Request. . See NOTE 6."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "S4-U SGW F-TEID", "presence" : "C", "instance" : "1", "comment" : "This IE shall be included on the S4 interface if the S4-U interface is used."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "S5/S8-U PGW F-TEID", "presence" : "C", "instance" : "2", "comment" : "For GTP-based S5/S8, this User Plane IE shall be included on S4/S11 and S5/S8 interfaces during the E-UTRAN Initial Attach, a Handover from Trusted or Untrusted Non-3GPP IP Access to E-UTRAN, a PDP Context Activation, a Handover from Trusted or Untrusted Non-3GPP IP Access to UTRAN/GERAN or a UE Requested PDN Connectivity."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "S12 SGW F-TEID", "presence" : "C", "instance" : "3", "comment" : "This IE shall be included on the S4 interface if the S12 interface is used."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "S2b-U PGW F-TEID", "presence" : "C", "instance" : "4", "comment" : "This IE (for user plane) shall be included on the S2b interface during the Attach with GTP on S2b, UE initiated Connectivity to Additional PDN with GTP on S2b,  Handover to Untrusted Non-3GPP IP Access with GTP on S2b, and Initial Attach for emergency session (GTP on S2b)."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "S2a-U PGW F-TEID", "presence" : "C", "instance" : "5", "comment" : "This IE (for user plane) shall be included on the S2a interface during the Initial Attach in WLAN on GTP S2a, an Initial Attach in WLAN for Emergency Service on GTP S2a, UE initiated Connectivity to Additional PDN with GTP on S2a, and Handover to TWAN with GTP on S2a."})
ies.append({ "ie_type" : "Bearer QoS", "ie_value" : "Bearer Level QoS", "presence" : "C", "instance" : "0", "comment" : "This IE shall be included on the S5/S8, S4/S11 and S2a/S2b interfaces if the received QoS parameters have been modified."})
ies.append({ "ie_type" : "Charging ID", "ie_value" : "Charging Id", "presence" : "C", "instance" : "0", "comment" : "This IE shall be included on the S5/S8 interface for an E-UTRAN initial attach, a Handover from Trusted or Untrusted Non-3GPP IP Access to E-UTRAN, a PDP Context Activation, a Handover from Trusted or Untrusted Non-3GPP IP Access to UTRAN/GERAN and a UE requested PDN connectivity."})
ies.append({ "ie_type" : "Bearer Flags", "ie_value" : "Bearer Flags", "presence" : "O", "instance" : "0", "comment" : "Applicable flags are:PPC (Prohibit Payload Compression) : this flag may be set on the S5/S8 and S4 interfaces."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "S11-U SGW F-TEID", "presence" : "C", "instance" : "6", "comment" : "This IE shall be included on the S11 interface if the S11-U interface is used, i.e. if the S11-U Tunnel flag was set in the Create Session Request.If the SGW supports both IP address types, the SGW shall send both IP addresses within the F-TEID IE. If only one IP address is included, then the MME shall assume that the SGW does not support the other IP address type."})
group_list[("Bearer Context", "Create Session Response", "created")] = { "index" : "193", "type" : "93", "context" : "Create Session Response", "action" : "created", "paragraph" : paragraph, "ies" : ies }
# [Table 7.2.2-3: Bearer Context marked for removal within a Create Session Response] Index = 17
ies = []
paragraph = "Table 7.2.2-3: Bearer Context marked for removal within a Create Session Response"
ies.append({ "ie_type" : "EBI", "ie_value" : "EPS Bearer ID", "presence" : "M", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "Cause", "ie_value" : "Cause", "presence" : "M", "instance" : "0", "comment" : "This IE shall indicate if the bearer handling was successful, and if not, gives the information on the reason."})
group_list[("Bearer Context", "Create Session Response", "removal")] = { "index" : "193", "type" : "93", "context" : "Create Session Response", "action" : "removal", "paragraph" : paragraph, "ies" : ies }
# [Table 7.2.2-4: Load Control Information within Create Session Response] Index = 18
ies = []
paragraph = "Table 7.2.2-4: Load Control Information within Create Session Response"
ies.append({ "ie_type" : "Sequence Number", "ie_value" : "Load Control Sequence Number", "presence" : "M", "instance" : "0", "comment" : "See clause 12.2.5.1.2.1 for the description and use of this parameter."})
ies.append({ "ie_type" : "Metric", "ie_value" : "Load Metric", "presence" : "M", "instance" : "0", "comment" : "See clauses 12.2.5.1.2.2 and 12.2.5.1.2.3 for the description and use of this parameter."})
ies.append({ "ie_type" : "APN and Relative Capacity", "ie_value" : "List of APN and Relative Capacity", "presence" : "CO", "instance" : "0", "comment" : "The IE shall (only) be present in the PGWs APN level Load Control Information IE.For indicating the APN level load, the PGW shall include one or more instances of this IE, up to maximum of 10, with the same type and instance value, representing a list of APN(s) & its respective Relative Capacity (sharing the same Load Metric).See clause 12.2.5.1.2.3 for the description and use of this parameter.See NOTE 1."})
group_list[("Load Control Information", "Create Session Response", "")] = { "index" : "281", "type" : "181", "context" : "Create Session Response", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 7.2.2-5: Overload Control Information within Create Session Response] Index = 19
ies = []
paragraph = "Table 7.2.2-5: Overload Control Information within Create Session Response"
ies.append({ "ie_type" : "Sequence Number", "ie_value" : "Overload Control Sequence Number", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.1 for the description and use of this parameter."})
ies.append({ "ie_type" : "Metric", "ie_value" : "Overload Reduction Metric", "presence" : "M", "instance" : "0", "comment" : "See clauses 12.3.5.1.2.3 and 12.3.5.1.2.4 for the description and use of this parameter."})
ies.append({ "ie_type" : "EPC Timer", "ie_value" : "Period of Validity", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.2 for the description and use of this parameter.This IE should be set to 0 if the Overload Reduction Metric is null. This IE shall be ignored by the receiver if the Overload Reduction Metric is null."})
ies.append({ "ie_type" : "APN", "ie_value" : "List of Access Point Name", "presence" : "CO", "instance" : "0", "comment" : "The IE may (only) be present in the PGWs Overload Control Information IE.For indicating the APN level overload, the PGW shall include one or more instances of this IE, up to maximum of 10, with the same type and instance value, representing a list of APN(s) (sharing the same Overload Reduction Metric and Period of Validity). See NOTE 1."})
group_list[("Overload Control Information", "Create Session Response", "")] = { "index" : "280", "type" : "180", "context" : "Create Session Response", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 7.2.3-2: Bearer Context within Create Bearer Request] Index = 21
ies = []
paragraph = "Table 7.2.3-2: Bearer Context within Create Bearer Request"
ies.append({ "ie_type" : "EBI", "ie_value" : "EPS Bearer ID", "presence" : "M", "instance" : "0", "comment" : "This IE shall be set to 0."})
ies.append({ "ie_type" : "Bearer TFT", "ie_value" : "TFT", "presence" : "M", "instance" : "0", "comment" : "This IE can contain both uplink and downlink packet filters to be sent to the UE or the TWAN/ePDG."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "S1-U SGW F-TEID", "presence" : "C", "instance" : "0", "comment" : "This IE shall be sent on the S11 interface if the S1-U interface is used. If SGW supports both IPv4 and IPv6, it shall send both an IPv4 address and an IPv6 address within the S1-U SGW F-TEID IE.See NOTE 1."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "S5/8-U PGW F-TEID", "presence" : "C", "instance" : "1", "comment" : "This IE shall be sent on the S4, S5/S8 and S11 interfaces for GTP-based S5/S8 interface. The MME/SGSN shall ignore the IE on S11/S4 for PMIP-based S5/S8 interface."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "S12 SGW F-TEID", "presence" : "C", "instance" : "2", "comment" : "This IE shall be sent on the S4 interface if the S12 interface is used. See NOTE 1."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "S4-U SGW F-TEID", "presence" : "C", "instance" : "3", "comment" : "This IE shall be sent on the S4 interface if the S4-U interface is used. See NOTE 1."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "S2b-U PGW F-TEID", "presence" : "C", "instance" : "4", "comment" : "This IE (for user plane) shall be sent on the S2b interface."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "S2a-U PGW F-TEID", "presence" : "C", "instance" : "5", "comment" : "This IE (for user plane) shall be sent on the S2a interface."})
ies.append({ "ie_type" : "Bearer QoS", "ie_value" : "Bearer Level QoS", "presence" : "M", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "Charging ID", "ie_value" : "Charging Id", "presence" : "C", "instance" : "0", "comment" : "This IE shall be sent on the S5/S8 interface."})
ies.append({ "ie_type" : "Bearer Flags", "ie_value" : "Bearer Flags", "presence" : "O", "instance" : "0", "comment" : "Applicable flags are:PPC (Prohibit Payload Compression) : this flag may be set on the S5/S8 and S4 interfaces.vSRVCC indicator: This IE may be included by the PGW on the S5/S8 interface according to 3GPP TS 23.216 [43]. When received from S5/S8, SGW shall forward on the S11 interface."})
ies.append({ "ie_type" : "PCO", "ie_value" : "Protocol Configuration Options", "presence" : "O", "instance" : "0", "comment" : "This IE may be sent on the S5/S8 and S4/S11 interfaces if ePCO is not supported by the UE or the network. This bearer level IE takes precedence over the PCO IE in the message body if they both exist."})
ies.append({ "ie_type" : "ePCO", "ie_value" : "Extended Protocol Configuration Options", "presence" : "O", "instance" : "0", "comment" : "This IE may be sent on the S5/S8 and S11 interfaces if the UE and the network support ePCO. "})
ies.append({ "ie_type" : "Maximum Packet Loss Rate", "ie_value" : "Maximum Packet Loss Rate", "presence" : "O", "instance" : "0", "comment" : "This IE may be included on the S5/S8 interfaces if the PGW needs to send Maximum Packet Loss Rate as specified in clause 5.4.1 of 3GPP TS 23.401 [3]. This IE is only applicable for QCI 1. "})
group_list[("Bearer Context", "Create Bearer Request", "")] = { "index" : "193", "type" : "93", "context" : "Create Bearer Request", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 7.2.3-3: Load Control Information within Create Bearer Request] Index = 22
ies = []
paragraph = "Table 7.2.3-3: Load Control Information within Create Bearer Request"
ies.append({ "ie_type" : "Sequence Number", "ie_value" : "Load Control Sequence Number", "presence" : "M", "instance" : "0", "comment" : "See clause 12.2.5.1.2.1 for the description and use of this parameter."})
ies.append({ "ie_type" : "Metric", "ie_value" : "Load Metric", "presence" : "M", "instance" : "0", "comment" : "See clauses 12.2.5.1.2.2 and 12.2.5.1.2.3 for the description and use of this parameter."})
ies.append({ "ie_type" : "APN and Relative Capacity", "ie_value" : "List of APN and Relative Capacity", "presence" : "CO", "instance" : "0", "comment" : "The IE shall (only) be present in the PGWs APN level Load Control Information IE.For indicating the APN level load, the PGW shall include one or more instances of this IE, up to maximum of 10, with the same type and instance value, representing a list of APN(s) & its respective Relative Capacity (sharing the same Load Metric).See clause 12.2.5.1.2.3 for the description and use of this parameter.See NOTE 1."})
group_list[("Load Control Information", "Create Bearer Request", "")] = { "index" : "281", "type" : "181", "context" : "Create Bearer Request", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 7.2.3-4: Overload Control Information within Create Bearer Request] Index = 23
ies = []
paragraph = "Table 7.2.3-4: Overload Control Information within Create Bearer Request"
ies.append({ "ie_type" : "Sequence Number", "ie_value" : "Overload Control Sequence Number", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.1 for the description and use of this parameter."})
ies.append({ "ie_type" : "Metric", "ie_value" : "Overload Reduction Metric", "presence" : "M", "instance" : "0", "comment" : "See clauses 12.3.5.1.2.3 and 12.3.5.1.2.4 for the description and use of this parameter."})
ies.append({ "ie_type" : "EPC Timer", "ie_value" : "Period of Validity", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.2 for the description and use of this parameter.This IE should be set to 0 if the Overload Reduction Metric is null. This IE shall be ignored by the receiver if the Overload Reduction Metric is null."})
ies.append({ "ie_type" : "APN", "ie_value" : "List of Access Point Name", "presence" : "CO", "instance" : "0", "comment" : "The IE may (only) be present in the PGWs Overload Control Information IE.For indicating the APN level overload, the PGW shall include one or more instances of this IE, up to maximum of 10, with the same type and instance value, representing a list of APN(s) (sharing the same Overload Reduction Metric and Period of Validity).See NOTE 1."})
group_list[("Overload Control Information", "Create Bearer Request", "")] = { "index" : "281", "type" : "181", "context" : "Create Bearer Request", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 7.2.4-2: Bearer Context within Create Bearer Response] Index = 25
ies = []
paragraph = "Table 7.2.4-2: Bearer Context within Create Bearer Response"
ies.append({ "ie_type" : "EBI", "ie_value" : "EPS Bearer ID", "presence" : "M", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "Cause", "ie_value" : "Cause", "presence" : "M", "instance" : "0", "comment" : "This IE shall indicate if the bearer handling was successful, and if not, it gives information on the reason."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "S1-U eNodeB F-TEID", "presence" : "C", "instance" : "0", "comment" : "This IE shall be sent on the S11 interface if the S1-U interface is used."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "S1-U SGW F-TEID", "presence" : "C", "instance" : "1", "comment" : "This IE shall be sent on the S11 interface. It shall be used to correlate the bearers with those in the Create Bearer Request."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "S5/8-U SGW F-TEID", "presence" : "C", "instance" : "2", "comment" : "This IE shall be sent on the S5/S8 interfaces."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "S5/8-U PGW F-TEID", "presence" : "C", "instance" : "3", "comment" : "This IE shall be sent on the S5/S8 interfaces. It shall be used to correlate the bearers with those in the Create Bearer Request."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "S12 RNC F-TEID", "presence" : "C", "instance" : "4", "comment" : "This IE shall be sent on the S4 interface if the S12 interface is used. See NOTE1."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "S12 SGW F-TEID", "presence" : "C", "instance" : "5", "comment" : "This IE shall be sent on the S4 interface. It shall be used to correlate the bearers with those in the Create Bearer Request. See NOTE1."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "S4-U SGSN F-TEID", "presence" : "C", "instance" : "6", "comment" : "This IE shall be sent on the S4 interface if the S4-U interface is used. See NOTE1."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "S4-U SGW F-TEID", "presence" : "C", "instance" : "7", "comment" : "This IE shall be sent on the S4 interface. It shall be used to correlate the bearers with those in the Create Bearer Request. See NOTE1."})
type_list["F-TEID"]["max_instance"] = "8"
ies.append({ "ie_type" : "F-TEID", "ie_value" : "S2b-U ePDG F-TEID", "presence" : "C", "instance" : "8", "comment" : "This IE shall be sent on the S2b interface."})
type_list["F-TEID"]["max_instance"] = "9"
ies.append({ "ie_type" : "F-TEID", "ie_value" : "S2b-U PGW F-TEID", "presence" : "C", "instance" : "9", "comment" : "This IE shall be sent on the S2b interface. It shall be used to correlate the bearers with those in the Create Bearer Request."})
type_list["F-TEID"]["max_instance"] = "10"
ies.append({ "ie_type" : "F-TEID", "ie_value" : "S2a-U TWAN F-TEID", "presence" : "C", "instance" : "10", "comment" : "This IE shall be sent on the S2a interface."})
type_list["F-TEID"]["max_instance"] = "11"
ies.append({ "ie_type" : "F-TEID", "ie_value" : "S2a-U PGW F-TEID", "presence" : "C", "instance" : "11", "comment" : "This IE shall be sent on the S2a interface. It shall be used to correlate the bearers with those in the Create Bearer Request."})
ies.append({ "ie_type" : "PCO", "ie_value" : "Protocol Configuration Options", "presence" : "CO", "instance" : "0", "comment" : "If the UE includes the PCO IE in the corresponding message, then the MME/SGSN shall copy the content of this IE transparently from the PCO IE included by the UE. If the SGW receives PCO from MME/SGSN, SGW shall forward it to the PGW. This bearer level IE takes precedence over the PCO IE in the message body if they both exist."})
ies.append({ "ie_type" : "RAN/NAS Cause", "ie_value" : "RAN/NAS Cause", "presence" : "CO", "instance" : "0", "comment" : "If the bearer creation failed, the MME shall include this IE on the S11 interface to indicate the RAN cause and/or the NAS cause of the bearer creation failure, if available and if this information is permitted to be sent to the PGW operator according to MME operators policy. If both a RAN cause and a NAS cause are generated, then several IEs with the same type and instance value shall be included to represent a list of causes.The SGW shall include this IE on the S5/S8 interface if it receives it from the MME."})
ies.append({ "ie_type" : "ePCO", "ie_value" : "Extended Protocol Configuration Options", "presence" : "CO", "instance" : "0", "comment" : "If the UE includes the ePCO IE, then the MME shall copy the content of this IE transparently from the ePCO IE included by the UE. If the SGW receives ePCO from the MME, the SGW shall forward it to the PGW."})
group_list[("Bearer Context", "Create Bearer Response", "")] = { "index" : "193", "type" : "93", "context" : "Create Bearer Response", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 7.2.4-3: Overload Control Information within Create Bearer Response] Index = 26
ies = []
paragraph = "Table 7.2.4-3: Overload Control Information within Create Bearer Response"
ies.append({ "ie_type" : "Sequence Number", "ie_value" : "Overload Control Sequence Number", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.1 for the description and use of this parameter."})
ies.append({ "ie_type" : "Metric", "ie_value" : "Overload Reduction Metric", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.3 for the description and use of this parameter."})
ies.append({ "ie_type" : "EPC Timer", "ie_value" : "Period of Validity", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.2 for the description and use of this parameter.This IE should be set to 0 if the Overload Reduction Metric is null. This IE shall be ignored by the receiver if the Overload Reduction Metric is null."})
group_list[("Overload Control Information", "Create Bearer Response", "")] = { "index" : "280", "type" : "180", "context" : "Create Bearer Response", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 7.2.5-2: Overload Control Information within Bearer Resource Command] Index = 28
ies = []
paragraph = "Table 7.2.5-2: Overload Control Information within Bearer Resource Command"
ies.append({ "ie_type" : "Sequence Number", "ie_value" : "Overload Control Sequence Number", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.1 for the description and use of this parameter."})
ies.append({ "ie_type" : "Metric", "ie_value" : "Overload Reduction Metric", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.3 for the description and use of this parameter."})
ies.append({ "ie_type" : "EPC Timer", "ie_value" : "Period of Validity", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.2 for the description and use of this parameter.This IE should be set to 0 if the Overload Reduction Metric is null. This IE shall be ignored by the receiver if the Overload Reduction Metric is null."})
group_list[("Overload Control Information", "Bearer Resource Command", "")] = { "index" : "280", "type" : "180", "context" : "Bearer Resource Command", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 7.2.6-2: Overload Control Information within Bearer Resource Failure Indication] Index = 30
ies = []
paragraph = "Table 7.2.6-2: Overload Control Information within Bearer Resource Failure Indication"
ies.append({ "ie_type" : "Sequence Number", "ie_value" : "Overload Control Sequence Number", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.1 for the description and use of this parameter."})
ies.append({ "ie_type" : "Metric", "ie_value" : "Overload Reduction Metric", "presence" : "M", "instance" : "0", "comment" : "See clauses 12.3.5.1.2.3 and 12.3.5.1.2.4 for the description and use of this parameter."})
ies.append({ "ie_type" : "EPC Timer", "ie_value" : "Period of Validity", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.2 for the description and use of this parameter.This IE should be set to 0 if the Overload Reduction Metric is null. This IE shall be ignored by the receiver if the Overload Reduction Metric is null."})
ies.append({ "ie_type" : "APN", "ie_value" : "List of Access Point Name", "presence" : "CO", "instance" : "0", "comment" : "The IE may (only) be present in the PGWs Overload Control Information IE.For indicating the APN level overload, the PGW shall include one or more instances of this IE, up to maximum of 10, with the same type and instance value, representing a list of APN(s) (sharing the same Overload Reduction Metric and Period of Validity).See NOTE 1."})
group_list[("Overload Control Information", "Bearer Resource Failure Indication", "")] = { "index" : "280", "type" : "180", "context" : "Bearer Resource Failure Indication", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 7.2.7-2: Bearer Context to be modified within Modify Bearer Request] Index = 32
ies = []
paragraph = "Table 7.2.7-2: Bearer Context to be modified within Modify Bearer Request"
ies.append({ "ie_type" : "EBI", "ie_value" : "EPS Bearer ID", "presence" : "M", "instance" : "0", "comment" : "See NOTE 1, NOTE 2."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "S1 eNodeB F-TEID", "presence" : "C", "instance" : "0", "comment" : "This IE shall be sent on the S11 interface if the S1-U is being used:for an E-UTRAN initial attach;for a Handover from Trusted or Untrusted Non-3GPP IP Access to E-UTRAN;for an UE triggered Service Request;for an UE initiated Connection Resume procedure;in all S1-U GTP-U tunnel setup procedure during a TAU procedure (see 3GPP TS 24.301 [23]) /handover cases;in all procedures where the UE is already in ECM-CONNECTED state, e.g. E-UTRAN Initiated E-RAB modification procedure, possibly HSS-based P-CSCF restoration for 3GPP access. See NOTE 4;in the Establishment of S1-U bearer during Data Transport in Control Plane CIoT EPS optimisation procedure. See NOTE 7.If an MME is aware that the eNodeB supports both IP address types, the MME shall send both IP addresses within an F-TEID IE. If only one IP address is included, then the SGW shall assume that the eNodeB does not support the other IP address type. See NOTE 2, NOTE 5, NOTE 6."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "S5/8-U SGW F-TEID", "presence" : "C", "instance" : "1", "comment" : "This IE shall be sent on the S5/S8 interfaces for a Handover or a TAU/RAU with a SGW change."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "S12 RNC F-TEID", "presence" : "C", "instance" : "2", "comment" : "If available, this IE shall be included if the message is sent on the S4 interface if S12 interface is being used. If an S4-SGSN is aware that the RNC supports both IP address types, the S4-SGSN shall send both IP addresses within an F-TEID IE. If only one IP address is included, then the SGW shall assume that the RNC does not support the other IP address type.See NOTE 2, NOTE 6."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "S4-U SGSN F-TEID", "presence" : "C", "instance" : "3", "comment" : "If available, this IE shall be included if the message is sent on the S4 interface, if S4-U is being used. If an S4-SGSN supports both IP address types, the S4-SGSN shall send both IP addresses within an F-TEID IE. If only one IP address is included, then the SGW shall assume that the S4-SGSN does not support the other IP address type. See , NOTE 6."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "S11-U MME F-TEID", "presence" : "CO", "instance" : "4", "comment" : "This IE shall be sent on the S11 interface if S11-U is being used, i.e. for the following procedures: Mobile Originated Data transport in Control Plane CIoT EPS optimisation with P-GW connectivityMobile Terminated Data Transport in Control Plane CIoT EPS optimisation with P-GW connectivityin all procedures where the S11-U tunnel is already established, e.g. when reporting a change of User Location Information. TAU/RAU with SGW change procedure and data forwarding of DL data buffered in the old SGW (see clause 5.3.3.1A of 3GPP TS 23.401 [3]) for a Control Plane Only PDN connection. See NOTE 6.This IE may also be sent on the S11 interface, if S11-U is being used, during a E-UTRAN Tracking Area Update without SGW Change, if the MME needs to establish the S11-U tunnel.See NOTE 8."})
group_list[("Bearer Context", "Modify Bearer Request", "modified")] = { "index" : "193", "type" : "93", "context" : "Modify Bearer Request", "action" : "modified", "paragraph" : paragraph, "ies" : ies }
# [Table 7.2.7-3: Bearer Context to be removed within Modify Bearer Request] Index = 33
ies = []
paragraph = "Table 7.2.7-3: Bearer Context to be removed within Modify Bearer Request"
ies.append({ "ie_type" : "EBI", "ie_value" : "EPS Bearer ID", "presence" : "M", "instance" : "0", "comment" : ""})
group_list[("Bearer Context", "Modify Bearer Request", "removed")] = { "index" : "193", "type" : "93", "context" : "Modify Bearer Request", "action" : "removed", "paragraph" : paragraph, "ies" : ies }
# [Table 7.2.7-4: Overload Control Information within Modify Bearer Request] Index = 34
ies = []
paragraph = "Table 7.2.7-4: Overload Control Information within Modify Bearer Request"
ies.append({ "ie_type" : "Sequence Number", "ie_value" : "Overload Control Sequence Number", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.1 for the description and use of this parameter."})
ies.append({ "ie_type" : "Metric", "ie_value" : "Overload Reduction Metric", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.3 for the description and use of this parameter."})
ies.append({ "ie_type" : "EPC Timer", "ie_value" : "Period of Validity", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.2 for the description and use of this parameter.This IE should be set to 0 if the Overload Reduction Metric is null. This IE shall be ignored by the receiver if the Overload Reduction Metric is null."})
group_list[("Overload Control Information", "Modify Bearer Request", "")] = { "index" : "280", "type" : "180", "context" : "Modify Bearer Request", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 7.2.8-2: Bearer Context modified within Modify Bearer Response] Index = 36
ies = []
paragraph = "Table 7.2.8-2: Bearer Context modified within Modify Bearer Response"
ies.append({ "ie_type" : "EBI", "ie_value" : "EPS Bearer ID", "presence" : "M", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "Cause", "ie_value" : "Cause", "presence" : "M", "instance" : "0", "comment" : "This IE shall indicate if the bearer handling was successful, and if not, gives information on the reason."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "S1-U SGW F-TEID", "presence" : "C", "instance" : "0", "comment" : "This IE shall be used on the S11 interface, if the S1 interface is used, i.e. if the S11-U Tunnel flag was not set in the Modify Bearer Request. If the Change F-TEID support Indication flag was set to 1 in the Modify Bearer Request and the SGW needs to change the F-TEID, the SGW shall include the new GTP-U F-TEID value. Otherwise, the SGW shall return the currently allocated GTP-U F-TEID value. See NOTE 1"})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "S12 SGW F-TEID", "presence" : "C", "instance" : "1", "comment" : "This IE shall be included on the S4 interface if the S12 interface is being used. If the Change F-TEID support Indication flag was set to 1 in the Modify Bearer Request and the SGW needs to change the F-TEID, the SGW shall include the new GTP-U F-TEID value. Otherwise, the SGW shall return the currently allocated GTP-U F-TEID value. See NOTE 1"})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "S4-U SGW F-TEID", "presence" : "C", "instance" : "2", "comment" : "This IE shall be present if used on the S4 interface if the S4-U interface is being used. If the Change F-TEID support Indication flag was set to 1 in the Modify Bearer Request and the SGW needs to change the F-TEID, the SGW shall include the new GTP-U F-TEID value. Otherwise, the SGW shall return the currently allocated GTP-U F-TEID value. See NOTE 1"})
ies.append({ "ie_type" : "Charging ID", "ie_value" : "Charging ID", "presence" : "C", "instance" : "0", "comment" : "This IE shall be present on the S5/S8 interface if this message is triggered due to one of the following procedures:TAU/RAU/HO with SGW relocationTAU/RAU/HO from Gn/Gp SGSN to MME/S4-SGSN   "})
ies.append({ "ie_type" : "Bearer Flags", "ie_value" : "Bearer Flags", "presence" : "CO", "instance" : "0", "comment" : "Applicable flags are:PPC (Prohibit Payload Compression): This flag shall be sent on the S5/S8 and the S4 interfaces at S4-SGSN relocation."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "S11-U SGW F-TEID", "presence" : "C", "instance" : "3", "comment" : "This IE shall be present on the S11 interface if S11-U is being used, i.e. if the S11-U Tunnel flag was set in the Modify Bearer Request. If the Change F-TEID support Indication flag was set to 1 in the Modify Bearer Request and the SGW needs to change the F-TEID, the SGW shall include the new GTP-U F-TEID value. Otherwise, the SGW shall return the currently allocated GTP-U F-TEID value. "})
group_list[("Bearer Context", "Modify Bearer Response", "modified")] = { "index" : "193", "type" : "93", "context" : "Modify Bearer Response", "action" : "modified", "paragraph" : paragraph, "ies" : ies }
# [Table 7.2.8-3: Bearer Context marked for removal within Modify Bearer Response] Index = 37
ies = []
paragraph = "Table 7.2.8-3: Bearer Context marked for removal within Modify Bearer Response"
ies.append({ "ie_type" : "EBI", "ie_value" : "EPS Bearer ID", "presence" : "M", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "Cause", "ie_value" : "Cause", "presence" : "M", "instance" : "0", "comment" : "This IE shall indicate if the bearer handling was successful, and if not, gives information on the reason."})
group_list[("Bearer Context", "Modify Bearer Response", "removal")] = { "index" : "193", "type" : "93", "context" : "Modify Bearer Response", "action" : "removal", "paragraph" : paragraph, "ies" : ies }
# [Table 7.2.8-4: Load Control Information within Modify Bearer Response] Index = 38
ies = []
paragraph = "Table 7.2.8-4: Load Control Information within Modify Bearer Response"
ies.append({ "ie_type" : "Sequence Number", "ie_value" : "Load Control Sequence Number", "presence" : "M", "instance" : "0", "comment" : "See clause 12.2.5.1.2.1 for the description and use of this parameter."})
ies.append({ "ie_type" : "Metric", "ie_value" : "Load Metric", "presence" : "M", "instance" : "0", "comment" : "See clauses 12.2.5.1.2.2 and 12.2.5.1.2.3 for the description and use of this parameter."})
ies.append({ "ie_type" : "APN and Relative Capacity", "ie_value" : "List of APN and Relative Capacity", "presence" : "CO", "instance" : "0", "comment" : "The IE shall (only) be present in the PGWs APN level Load Control Information IE.For indicating the APN level load, the PGW shall include one or more instances of this IE, up to maximum of 10, with the same type and instance value, representing a list of APN(s) & its respective Relative Capacity (sharing the same Load Metric).See clause 12.2.5.1.2.3 for the description and use of this parameter.See NOTE 1."})
group_list[("Load Control Information", "Modify Bearer Response", "")] = { "index" : "281", "type" : "181", "context" : "Modify Bearer Response", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 7.2.8-5: Overload Control Information within Modify Bearer Response] Index = 39
ies = []
paragraph = "Table 7.2.8-5: Overload Control Information within Modify Bearer Response"
ies.append({ "ie_type" : "Sequence Number", "ie_value" : "Overload Control Sequence Number", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.1 for the description and use of this parameter."})
ies.append({ "ie_type" : "Metric", "ie_value" : "Overload Reduction Metric", "presence" : "M", "instance" : "0", "comment" : "See clauses 12.3.5.1.2.3 and 12.3.5.1.2.4 for the description and use of this parameter."})
ies.append({ "ie_type" : "EPC Timer", "ie_value" : "Period of Validity", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.2 for the description and use of this parameter.This IE should be set to 0 if the Overload Reduction Metric is null. This IE shall be ignored by the receiver if the Overload Reduction Metric is null."})
ies.append({ "ie_type" : "APN", "ie_value" : "List of Access Point Name", "presence" : "CO", "instance" : "0", "comment" : "The IE may (only) be present in the PGWs Overload Control Information IE.For indicating the APN level overload, the PGW shall include one or more instances of this IE, up to maximum of 10, with the same type and instance value, representing a list of APN(s) (sharing the same Overload Reduction Metric and Period of Validity). See NOTE 1."})
group_list[("Overload Control Information", "Modify Bearer Response", "")] = { "index" : "280", "type" : "180", "context" : "Modify Bearer Response", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 7.2.9.1-2: Overload Control Information within Delete Session Request] Index = 41
ies = []
paragraph = "Table 7.2.9.1-2: Overload Control Information within Delete Session Request"
ies.append({ "ie_type" : "Sequence Number", "ie_value" : "Overload Control Sequence Number", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.1 for the description and use of this parameter."})
ies.append({ "ie_type" : "Metric", "ie_value" : "Overload Reduction Metric", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.3 for the description and use of this parameter."})
ies.append({ "ie_type" : "EPC Timer", "ie_value" : "Period of Validity", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.2 for the description and use of this parameter.This IE should be set to 0 if the Overload Reduction Metric is null. This IE shall be ignored by the receiver if the Overload Reduction Metric is null."})
group_list[("Overload Control Information", "Delete Session Request", "")] = { "index" : "280", "type" : "180", "context" : "Delete Session Request", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 7.2.9.2-2: Bearer Context within Delete Bearer Request] Index = 43
ies = []
paragraph = "Table 7.2.9.2-2: Bearer Context within Delete Bearer Request"
ies.append({ "ie_type" : "EBI", "ie_value" : "EPS Bearer ID", "presence" : "M", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "Cause", "ie_value" : "Cause", "presence" : "M", "instance" : "0", "comment" : "This IE shall indicate the reason of the unsuccessful handling of the bearer."})
group_list[("Bearer Context", "Delete Bearer Request", "")] = { "index" : "193", "type" : "93", "context" : "Delete Bearer Request", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 7.2.9-3: Load Control Information within Delete Bearer Request] Index = 44
ies = []
paragraph = "Table 7.2.9-3: Load Control Information within Delete Bearer Request"
ies.append({ "ie_type" : "Sequence Number", "ie_value" : "Load Control Sequence Number", "presence" : "M", "instance" : "0", "comment" : "See clause 12.2.5.1.2.1 for the description and use of this parameter."})
ies.append({ "ie_type" : "Metric", "ie_value" : "Load Metric", "presence" : "M", "instance" : "0", "comment" : "See clauses 12.2.5.1.2.2 and 12.2.5.1.2.3 for the description and use of this parameter."})
ies.append({ "ie_type" : "APN and Relative Capacity", "ie_value" : "List of APN and Relative Capacity", "presence" : "CO", "instance" : "0", "comment" : "The IE shall (only) be present in the PGWs APN level Load Control Information IE.For indicating the APN level load, the PGW shall include one or more instances of this IE, up to maximum of 10, with the same type and instance value, representing a list of APN(s) & its respective Relative Capacity (sharing the same Load Metric).See clause 12.2.5.1.2.3 for the description and use of this parameter.See NOTE 1."})
group_list[("Load Control Information", "Delete Bearer Request", "")] = { "index" : "281", "type" : "181", "context" : "Delete Bearer Request", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 7.2.9-4: Overload Control Information within Delete Bearer Request] Index = 45
ies = []
paragraph = "Table 7.2.9-4: Overload Control Information within Delete Bearer Request"
ies.append({ "ie_type" : "Sequence Number", "ie_value" : "Overload Control Sequence Number", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.1 for the description and use of this parameter."})
ies.append({ "ie_type" : "Metric", "ie_value" : "Overload Reduction Metric", "presence" : "M", "instance" : "0", "comment" : "See clauses 12.3.5.1.2.3 and 12.3.5.1.2.4 for the description and use of this parameter."})
ies.append({ "ie_type" : "EPC Timer", "ie_value" : "Period of Validity", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.2 for the description and use of this parameter.This IE should be set to 0 if the Overload Reduction Metric is null. This IE shall be ignored by the receiver if the Overload Reduction Metric is null."})
ies.append({ "ie_type" : "APN", "ie_value" : "List of Access Point Name", "presence" : "CO", "instance" : "0", "comment" : "The IE may (only) be present in the PGWs Overload Control Information IE.For indicating the APN level overload, the PGW shall include one or more instances of this IE, up to maximum of 10, with the same type and instance value, representing a list of APN(s) (sharing the same Overload Reduction Metric and Period of Validity). See NOTE 1."})
group_list[("Overload Control Information", "Delete Bearer Request", "")] = { "index" : "280", "type" : "180", "context" : "Delete Bearer Request", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 7.2.10.1-2: Load Control Information within Delete Session Response] Index = 47
ies = []
paragraph = "Table 7.2.10.1-2: Load Control Information within Delete Session Response"
ies.append({ "ie_type" : "Sequence Number", "ie_value" : "Load Control Sequence Number", "presence" : "M", "instance" : "0", "comment" : "See clause 12.2.5.1.2.1 for the description and use of this parameter."})
ies.append({ "ie_type" : "Metric", "ie_value" : "Load Metric", "presence" : "M", "instance" : "0", "comment" : "See clause 12.2.5.1.2.2 for the description and use of this parameter."})
ies.append({ "ie_type" : "APN and Relative Capacity", "ie_value" : "List of APN and Relative Capacity", "presence" : "CO", "instance" : "0", "comment" : "The IE shall (only) be present in the PGWs APN level Load Control Information IE.For indicating the APN level load, the PGW shall include one or more instances of this IE, up to maximum of 10, with the same type and instance value, representing a list of APN(s) & its respective Relative Capacity (sharing the same Load Metric).See clause 12.2.5.1.2.3 for the description and use of this parameter.See NOTE 1."})
group_list[("Load Control Information", "Delete Session Response", "")] = { "index" : "281", "type" : "181", "context" : "Delete Session Response", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 7.2.10.1-3: Overload Control Information within Delete Session Response] Index = 48
ies = []
paragraph = "Table 7.2.10.1-3: Overload Control Information within Delete Session Response"
ies.append({ "ie_type" : "Sequence Number", "ie_value" : "Overload Control Sequence Number", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.1 for the description and use of this parameter."})
ies.append({ "ie_type" : "Metric", "ie_value" : "Overload Reduction Metric", "presence" : "M", "instance" : "0", "comment" : "See clauses 12.3.5.1.2.3 and 12.3.5.1.2.4 for the description and use of this parameter."})
ies.append({ "ie_type" : "EPC Timer", "ie_value" : "Period of Validity", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.2 for the description and use of this parameter.This IE should be set to 0 if the Overload Reduction Metric is null. This IE shall be ignored by the receiver if the Overload Reduction Metric is null."})
ies.append({ "ie_type" : "APN", "ie_value" : "List of Access Point Name", "presence" : "CO", "instance" : "0", "comment" : "The IE may (only) be present in the PGWs Overload Control Information IE.For indicating the APN level overload, the PGW shall include one or more instances of this IE, up to maximum of 10, with the same type and instance value, representing a list of APN(s) (sharing the same Overload Reduction Metric and Period of Validity).See NOTE 1."})
group_list[("Overload Control Information", "Delete Session Response", "")] = { "index" : "280", "type" : "180", "context" : "Delete Session Response", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 7.2.10.2-2: Bearer Context within Delete Bearer Response] Index = 50
ies = []
paragraph = "Table 7.2.10.2-2: Bearer Context within Delete Bearer Response"
ies.append({ "ie_type" : "EBI", "ie_value" : "EPS Bearer ID", "presence" : "M", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "Cause", "ie_value" : "Cause", "presence" : "M", "instance" : "0", "comment" : "This IE shall indicate if the bearer handling was successful, and if not, gives information on the reason."})
ies.append({ "ie_type" : "PCO", "ie_value" : "Protocol Configuration Options", "presence" : "CO", "instance" : "0", "comment" : "An MME/SGSN shall include the PCO IE if such information was received from the UE. If the SGW receives this IE, the SGW shall forward it to PGW on the S5/S8 interface.This bearer level IE takes precedence over the PCO IE in the message body if they both exist."})
ies.append({ "ie_type" : "RAN/NAS Cause", "ie_value" : "RAN/NAS Cause", "presence" : "CO", "instance" : "0", "comment" : "The MME shall include this IE on the S11 interface to indicate the RAN release cause and/or NAS release cause to release the bearer, if this information is available and is permitted to be sent to the PGW operator according to the MME operators policy. If both a RAN release cause and a NAS release cause are generated, then several IEs with the same type and instance value shall be included to represent a list of causes.The SGW shall include this IE on the S5/S8 interface if it receives it from the MME. See NOTE 1."})
ies.append({ "ie_type" : "ePCO", "ie_value" : "Extended Protocol Configuration Options", "presence" : "CO", "instance" : "0", "comment" : "An MME shall include the ePCO IE if such information is received from the UE. If the SGW receives this IE, the SGW shall forward it to the PGW on the S5/S8 interface."})
group_list[("Bearer Context", "Delete Bearer Response", "")] = { "index" : "193", "type" : "93", "context" : "Delete Bearer Response", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 7.2.10.2-3: Overload Control Information within Delete Bearer Response] Index = 51
ies = []
paragraph = "Table 7.2.10.2-3: Overload Control Information within Delete Bearer Response"
ies.append({ "ie_type" : "Sequence Number", "ie_value" : "Overload Control Sequence Number", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.1 for the description and use of this parameter."})
ies.append({ "ie_type" : "Metric", "ie_value" : "Overload Reduction Metric", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.3 for the description and use of this parameter."})
ies.append({ "ie_type" : "EPC Timer", "ie_value" : "Period of Validity", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.2 for the description and use of this parameter.This IE should be set to 0 if the Overload Reduction Metric is null. This IE shall be ignored by the receiver if the Overload Reduction Metric is null."})
group_list[("Overload Control Information", "Delete Bearer Response", "")] = { "index" : "280", "type" : "180", "context" : "Delete Bearer Response", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 7.2.11.1-2: Load Control Information within Downlink Data Notification] Index = 53
ies = []
paragraph = "Table 7.2.11.1-2: Load Control Information within Downlink Data Notification"
ies.append({ "ie_type" : "Sequence Number", "ie_value" : "Load Control Sequence Number", "presence" : "M", "instance" : "0", "comment" : "See clause 12.2.5.1.2.1 for the description and use of this parameter."})
ies.append({ "ie_type" : "Metric", "ie_value" : "Load Metric", "presence" : "M", "instance" : "0", "comment" : "See clause 12.2.5.1.2.2 for the description and use of this parameter."})
group_list[("Load Control Information", "Downlink Data Notification", "")] = { "index" : "281", "type" : "181", "context" : "Downlink Data Notification", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 7.2.11.1-3: Overload Control Information within Downlink Data Notification] Index = 54
ies = []
paragraph = "Table 7.2.11.1-3: Overload Control Information within Downlink Data Notification"
ies.append({ "ie_type" : "Sequence Number", "ie_value" : "Overload Control Sequence Number", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.1 for the description and use of this parameter."})
ies.append({ "ie_type" : "Metric", "ie_value" : "Overload Reduction Metric", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.3 for the description and use of this parameter."})
ies.append({ "ie_type" : "EPC Timer", "ie_value" : "Period of Validity", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.2 for the description and use of this parameter.This IE should be set to 0 if the Overload Reduction Metric is null. This IE shall be ignored by the receiver if the Overload Reduction Metric is null."})
group_list[("Overload Control Information", "Downlink Data Notification", "")] = { "index" : "280", "type" : "180", "context" : "Downlink Data Notification", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 7.2.14.1-2: Bearer Context within Modify Bearer Command] Index = 60
ies = []
paragraph = "Table 7.2.14.1-2: Bearer Context within Modify Bearer Command"
ies.append({ "ie_type" : "EBI", "ie_value" : "EPS Bearer ID", "presence" : "M", "instance" : "0", "comment" : "This IE shall contain the default bearer ID."})
ies.append({ "ie_type" : "Bearer QoS", "ie_value" : "Bearer Level QoS", "presence" : "C", "instance" : "0", "comment" : "Mandatory if other parameters than the APN-AMBR have been changed"})
group_list[("Bearer Context", "Modify Bearer Command", "")] = { "index" : "193", "type" : "93", "context" : "Modify Bearer Command", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 7.2.14-3: Overload Control Information within Modify Bearer Command] Index = 61
ies = []
paragraph = "Table 7.2.14-3: Overload Control Information within Modify Bearer Command"
ies.append({ "ie_type" : "Sequence Number", "ie_value" : "Overload Control Sequence Number", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.1 for the description and use of this parameter."})
ies.append({ "ie_type" : "Metric", "ie_value" : "Overload Reduction Metric", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.3 for the description and use of this parameter."})
ies.append({ "ie_type" : "EPC Timer", "ie_value" : "Period of Validity", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.2 for the description and use of this parameter.This IE should be set to 0 if the Overload Reduction Metric is null. This IE shall be ignored by the receiver if the Overload Reduction Metric is null."})
group_list[("Overload Control Information", "Modify Bearer Command", "")] = { "index" : "281", "type" : "181", "context" : "Modify Bearer Command", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 7.2.14-2: Overload Control Information within Modify Bearer Failure Indication] Index = 63
ies = []
paragraph = "Table 7.2.14-2: Overload Control Information within Modify Bearer Failure Indication"
ies.append({ "ie_type" : "Sequence Number", "ie_value" : "Overload Control Sequence Number", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.1 for the description and use of this parameter."})
ies.append({ "ie_type" : "Metric", "ie_value" : "Overload Reduction Metric", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.3 for the description and use of this parameter."})
ies.append({ "ie_type" : "EPC Timer", "ie_value" : "Period of Validity", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.2 for the description and use of this parameter.This IE should be set to 0 if the Overload Reduction Metric is null. This IE shall be ignored by the receiver if the Overload Reduction Metric is null."})
ies.append({ "ie_type" : "APN", "ie_value" : "List of Access Point Name", "presence" : "CO", "instance" : "0", "comment" : "The IE may (only) be present in the PGWs Overload Control Information IE.For indicating the APN level overload, the PGW shall include one or more instances of this IE, up to maximum of 10, with the same type and instance value, representing a list of APN(s) (sharing the same Overload Reduction Metric).See NOTE 1."})
group_list[("Overload Control Information", "Modify Bearer Failure Indication", "")] = { "index" : "280", "type" : "180", "context" : "Modify Bearer Failure Indication", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 7.2.15-2: Bearer Context within Update Bearer Request] Index = 65
ies = []
paragraph = "Table 7.2.15-2: Bearer Context within Update Bearer Request"
ies.append({ "ie_type" : "EBI", "ie_value" : "EPS Bearer ID", "presence" : "M", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "Bearer TFT", "ie_value" : "TFT", "presence" : "C", "instance" : "0", "comment" : "This IE shall be included on the S5/S8, S4/S11 and S2a/S2b interfaces if message relates to Bearer Modification and TFT change."})
ies.append({ "ie_type" : "Bearer QoS", "ie_value" : "Bearer Level QoS", "presence" : "C", "instance" : "0", "comment" : "This IE shall be included on the S5/S8, S4/S11 and S2a/S2b interfaces if QoS modification is requested "})
ies.append({ "ie_type" : "Bearer Flags", "ie_value" : "Bearer Flags", "presence" : "O", "instance" : "0", "comment" : "Applicable flags:PPC (Prohibit Payload Compression): this flag may be set on the S5/S8 and S4 interfaces.vSRVCC indicator: This IE may be included by the PGW on the S5/S8 interface according to 3GPP TS 23.216 [43]. When received from S5/S8, SGW shall forward on the S11 interface."})
ies.append({ "ie_type" : "PCO", "ie_value" : "Protocol Configuration Options", "presence" : "CO", "instance" : "0", "comment" : "PGW shall include Protocol Configuration Options (PCO) IE on the S5/S8 interface, if available and if ePCO is not supported by the UE or the network. The PCO IE shall carry a P-CSCF address list only when the UE is required to perform an IMS registration, e.g during the P-CSCF restoration procedure as defined in clause 5.1 of 3GPP TS 23.380 [61]. This bearer level IE takes precedence over the PCO IE in the message body if they both exist. If the SGW receives this IE, the SGW shall forward it to the SGSN/MME on the S4/S11 interface."})
ies.append({ "ie_type" : "APCO", "ie_value" : "Additional Protocol Configuration Options", "presence" : "CO", "instance" : "0", "comment" : "The PGW shall include the Additional Prococol Configuration Options (APCO) IE on the S2b interface, including the list of available P-CSCF addresses, as part of the P-CSCF restoration extension procedure for the untrusted WLAN access, as specified in 3GPP TS 23.380 [61]."})
ies.append({ "ie_type" : "ePCO", "ie_value" : "Extended Protocol Configuration Options", "presence" : "CO", "instance" : "0", "comment" : "The PGW shall include Extended Protocol Configuration Options (ePCO) IE on the S5/S8 interface, if available and if the UE and the network support ePCO. The ePCO IE shall carry a P-CSCF address list only when the UE is required to perform an IMS registration, e.g during the P-CSCF restoration procedure as defined in clause 5.1 of 3GPP TS 23.380 [61]. If the SGW receives this IE, the SGW shall forward it to the MME on the S11 interface."})
ies.append({ "ie_type" : "Maximum Packet Loss Rate", "ie_value" : "Maximum Packet Loss Rate", "presence" : "O", "instance" : "0", "comment" : "This IE may be included on the S5/S8 interfaces if the PGW needs to send Maximum Packet Loss Rate as specified in clause 5.4.2.1 of 3GPP TS 23.401 [3]. This IE is only applicable for QCI 1."})
group_list[("Bearer Context", "Update Bearer Request", "")] = { "index" : "193", "type" : "93", "context" : "Update Bearer Request", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 7.2.15-3: Load Control Information within Update Bearer Request] Index = 66
ies = []
paragraph = "Table 7.2.15-3: Load Control Information within Update Bearer Request"
ies.append({ "ie_type" : "Sequence Number", "ie_value" : "Load Control Sequence Number", "presence" : "M", "instance" : "0", "comment" : "See clause 12.2.5.1.2.1 for the description and use of this parameter."})
ies.append({ "ie_type" : "Metric", "ie_value" : "Load Metric", "presence" : "M", "instance" : "0", "comment" : "See clauses 12.2.5.1.2.2 and 12.2.5.1.2.3 for the description and use of this parameter."})
ies.append({ "ie_type" : "APN and Relative Capacity", "ie_value" : "List of APN and Relative Capacity", "presence" : "CO", "instance" : "0", "comment" : "The IE shall (only) be present in the PGWs APN level Load Control Information IE.For indicating the APN level load, the PGW shall include one or more instances of this IE, up to maximum of 10, with the same type and instance value, representing a list of APN(s) & its respective Relative Capacity (sharing the same Load Metric).See clause 12.2.5.1.2.3 for the description and use of this parameter.See NOTE 1."})
group_list[("Load Control Information", "Update Bearer Request", "")] = { "index" : "281", "type" : "181", "context" : "Update Bearer Request", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 7.2.15-4: Overload Control Information within Update Bearer Request] Index = 67
ies = []
paragraph = "Table 7.2.15-4: Overload Control Information within Update Bearer Request"
ies.append({ "ie_type" : "Sequence Number", "ie_value" : "Overload Control Sequence Number", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.1 for the description and use of this parameter."})
ies.append({ "ie_type" : "Metric", "ie_value" : "Overload Reduction Metric", "presence" : "M", "instance" : "0", "comment" : "See clauses 12.3.5.1.2.3 and 12.3.5.1.2.4 for the description and use of this parameter."})
ies.append({ "ie_type" : "EPC Timer", "ie_value" : "Period of Validity", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.2 for the description and use of this parameter.This IE should be set to 0 if the Overload Reduction Metric is null. This IE shall be ignored by the receiver if the Overload Reduction Metric is null."})
ies.append({ "ie_type" : "APN", "ie_value" : "List of Access Point Name", "presence" : "CO", "instance" : "0", "comment" : "The IE may (only) be present in the PGWs Overload Control Information IE.For indicating the APN level overload, the PGW shall include one or more instances of this IE, up to maximum of 10, with the same type and instance value, representing a list of APN(s) (sharing the same Overload Reduction Metric and Period of Validity).See NOTE 1."})
group_list[("Overload Control Information", "Update Bearer Request", "")] = { "index" : "280", "type" : "180", "context" : "Update Bearer Request", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 7.2.16-2: Bearer Context within Update Bearer Response] Index = 69
ies = []
paragraph = "Table 7.2.16-2: Bearer Context within Update Bearer Response"
ies.append({ "ie_type" : "EBI", "ie_value" : "EPS Bearer ID", "presence" : "M", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "Cause", "ie_value" : "Cause", "presence" : "M", "instance" : "0", "comment" : "This IE Indicates if the bearer handling was successful, and if not, gives information on the reason."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "S4-U SGSN F-TEID", "presence" : "C", "instance" : "0", "comment" : "This IE shall be included on the S4 interface when direct tunnel is not established. See NOTE 1."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "S12 RNC F-TEID", "presence" : "C", "instance" : "1", "comment" : "This IE shall be included on the S4 interface when direct tunnel flag is set to 1. See NOTE 1."})
ies.append({ "ie_type" : "PCO", "ie_value" : "Protocol Configuration Options", "presence" : "CO", "instance" : "0", "comment" : "An MME/SGSN shall include the PCO IE if such information was received from the UE. If the SGW receives this IE, the SGW shall forward it to PGW on the S5/S8 interface.This bearer level IE takes precedence over the PCO IE in the message body if they both exist."})
ies.append({ "ie_type" : "RAN/NAS Cause", "ie_value" : "RAN/NAS Cause", "presence" : "CO", "instance" : "0", "comment" : "If the bearer modification failed, the MME shall include this IE on the S11 interface to indicate the RAN cause and/or the NAS cause of the bearer modification failure, if available and if this information is permitted to be sent to the PGW operator according to MME operators policy. If both a RAN cause and a NAS cause are generated, then several IEs with the same type and instance value shall be included to represent a list of causes.The SGW shall include this IE on the S5/S8 interface if it receives it from the MME."})
ies.append({ "ie_type" : "ePCO", "ie_value" : "Extended Protocol Configuration Options", "presence" : "CO", "instance" : "0", "comment" : "The MME shall include the ePCO IE if such information is received from the UE. If the SGW receives this IE, the SGW shall forward it to PGW on the S5/S8 interface."})
group_list[("Bearer Context", "Update Bearer Response", "")] = { "index" : "193", "type" : "93", "context" : "Update Bearer Response", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 7.2.16-3: Overload Control Information within Update Bearer Response] Index = 70
ies = []
paragraph = "Table 7.2.16-3: Overload Control Information within Update Bearer Response"
ies.append({ "ie_type" : "Sequence Number", "ie_value" : "Overload Control Sequence Number", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.1 for the description and use of this parameter."})
ies.append({ "ie_type" : "Metric", "ie_value" : "Overload Reduction Metric", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.3 for the description and use of this parameter."})
ies.append({ "ie_type" : "EPC Timer", "ie_value" : "Period of Validity", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.2 for the description and use of this parameter.This IE should be set to 0 if the Overload Reduction Metric is null. This IE shall be ignored by the receiver if the Overload Reduction Metric is null."})
group_list[("Overload Control Information", "Update Bearer Response", "")] = { "index" : "280", "type" : "180", "context" : "Update Bearer Response", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 7.2.17.1-2: Bearer Context within Delete Bearer Command] Index = 72
ies = []
paragraph = "Table 7.2.17.1-2: Bearer Context within Delete Bearer Command"
ies.append({ "ie_type" : "EBI", "ie_value" : "EPS Bearer ID", "presence" : "M", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "Bearer Flags", "ie_value" : "Bearer Flags", "presence" : "CO", "instance" : "0", "comment" : "Applicable flags are:VB (Voice Bearer) indicator shall be set to indicate a voice bearer for PS-to-CS (v)SRVCC handover.Vind (vSRVCC indicator) indicator shall be set to indicate a video bearer for PS-to-CS vSRVCC handover."})
ies.append({ "ie_type" : "RAN/NAS Cause", "ie_value" : "RAN/NAS Release Cause", "presence" : "CO", "instance" : "0", "comment" : "The MME shall include this IE on the S11 interface to indicate the RAN release cause and/or NAS release cause to release the bearer, if available and this information is permitted to be sent to the PGW operator according to MME operators policy. If both a RAN release cause and a NAS release cause are generated, then several IEs with the same type and instance value shall be included to represent a list of causes.The SGW shall include this IE on the S5/S8 interface if it receives it from the MME."})
group_list[("Bearer Context", "Delete Bearer Command", "")] = { "index" : "193", "type" : "93", "context" : "Delete Bearer Command", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 7.2.17.1-3: Overload Control Information within Delete Bearer Command] Index = 73
ies = []
paragraph = "Table 7.2.17.1-3: Overload Control Information within Delete Bearer Command"
ies.append({ "ie_type" : "Sequence Number", "ie_value" : "Overload Control Sequence Number", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.1 for the description and use of this parameter."})
ies.append({ "ie_type" : "Metric", "ie_value" : "Overload Reduction Metric", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.3 for the description and use of this parameter."})
ies.append({ "ie_type" : "EPC Timer", "ie_value" : "Period of Validity", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.2 for the description and use of this parameter.This IE should be set to 0 if the Overload Reduction Metric is null. This IE shall be ignored by the receiver if the Overload Reduction Metric is null."})
group_list[("Overload Control Information", "Delete Bearer Command", "")] = { "index" : "280", "type" : "180", "context" : "Delete Bearer Command", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 7.2.17.2-2: Bearer Context within Delete Bearer Failure Indication] Index = 75
ies = []
paragraph = "Table 7.2.17.2-2: Bearer Context within Delete Bearer Failure Indication"
ies.append({ "ie_type" : "EBI", "ie_value" : "EPS Bearer ID", "presence" : "M", "instance" : "0", "comment" : "See clause 6.1.1 Presence requirements of Information Elements."})
ies.append({ "ie_type" : "Cause", "ie_value" : "Cause", "presence" : "M", "instance" : "0", "comment" : "This IE shall indicate the reason of the unsuccessful handling of the bearer."})
group_list[("Bearer Context", "Delete Bearer Failure Indication", "")] = { "index" : "193", "type" : "93", "context" : "Delete Bearer Failure Indication", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 7.2.17-3: Overload Control Information within Delete Bearer Failure Indication] Index = 76
ies = []
paragraph = "Table 7.2.17-3: Overload Control Information within Delete Bearer Failure Indication"
ies.append({ "ie_type" : "Sequence Number", "ie_value" : "Overload Control Sequence Number", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.1 for the description and use of this parameter."})
ies.append({ "ie_type" : "Metric", "ie_value" : "Overload Reduction Metric", "presence" : "M", "instance" : "0", "comment" : "See clauses 12.3.5.1.2.3 and 12.3.5.1.2.4 for the description and use of this parameter."})
ies.append({ "ie_type" : "EPC Timer", "ie_value" : "Period of Validity", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.2 for the description and use of this parameter.This IE should be set to 0 if the Overload Reduction Metric is null. This IE shall be ignored by the receiver if the Overload Reduction Metric is null."})
ies.append({ "ie_type" : "APN", "ie_value" : "List of Access Point Name", "presence" : "CO", "instance" : "0", "comment" : "The IE may (only) be present in the PGWs Overload Control Information IE.For indicating the APN level overload, the PGW shall include one or more instances of this IE, up to maximum of 10, with the same type and instance value, representing a list of APN(s) (sharing the same Overload Reduction Metric and Period of Validity).See NOTE 1."})
group_list[("Overload Control Information", "Delete Bearer Failure Indication", "")] = { "index" : "280", "type" : "180", "context" : "Delete Bearer Failure Indication", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 7.2.18-2: Bearer Context within Create Indirect Data Forwarding Tunnel Request] Index = 78
ies = []
paragraph = "Table 7.2.18-2: Bearer Context within Create Indirect Data Forwarding Tunnel Request"
ies.append({ "ie_type" : "EBI", "ie_value" : "EPS Bearer ID", "presence" : "M", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "eNodeB F-TEID for DL data forwarding", "presence" : "C", "instance" : "0", "comment" : "Target eNodeB F-TEID. This IE shall be present in the message sent from the target MME to the SGW selected by the target MME for indirect data forwarding, or shall be included in the message sent from the source SGSN/MME to the SGW selected by the source MME for indirect data forwarding if the eNodeB F-TEID for DL data forwarding is included in the Forward Relocation Response message."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "SGW/UPF F-TEID for DL data forwarding", "presence" : "C", "instance" : "1", "comment" : "Target SGW F-TEIDThis IE shall be present in the message sent from the source MME/SGSN to the SGW selected by the source MME for indirect data forwarding if SGW F-TEID for DL data forwarding is included in the Forward Relocation Response message. This F-TEID is assigned by the SGW that the target MME/SGSN selects for indirect data forwarding."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "SGSN F-TEID for DL data forwarding", "presence" : "C", "instance" : "2", "comment" : "Target SGSN F-TEIDThis IE shall be present in the message sent from the target SGSN to the SGW selected by the target SGSN for indirect data forwarding in E-UTRAN to GERAN/UTRAN inter RAT handover with SGW relocation procedure, or shall be included in the message sent from the source MME to the SGW selected by the source MME for indirect data forwarding if the SGSN F-TEID for DL data forwarding is included in the Forwarding Relocation Response message."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "RNC F-TEID for DL data forwarding", "presence" : "C", "instance" : "3", "comment" : "Target RNC F-TEIDThis IE shall be present in the message sent from the target SGSN to the SGW selected by the target SGSN for indirect data forwarding in E-UTRAN to UTRAN inter RAT handover with SGW relocation procedure, or shall be included in the message sent from the source MME to the SGW selected by the source MME for indirect data forwarding if the RNC F-TEID for DL data forwarding is included in the Forwarding Relocation Response message."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "eNodeB F-TEID for UL data forwarding", "presence" : "O", "instance" : "4", "comment" : "Target eNodeB F-TEID. If available this IE may be present in the message, which is sent during the intra-EUTRAN HO from the target MME to the SGW selected by the target MME for indirect data forwarding, or may be included in the message sent from the source MME to the SGW selected by the source MME for indirect data forwarding if the eNodeB F-TEID for data UL forwarding is included in the Forward Relocation Response message."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "SGW F-TEID for UL data forwarding", "presence" : "O", "instance" : "5", "comment" : "Target SGW F-TEIDIf available this IE may be present in the message, which is sent during the intra-EUTRAN HO from the source MME to the SGW selected by the source MME for indirect data forwarding if SGW F-TEID for UL data forwarding is included in the Forward Relocation Response message. This F-TEID is assigned by the SGW that the target MME selects for indirect data forwarding."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "MME F-TEID for DL data forwarding", "presence" : "CO", "instance" : "6", "comment" : "Target MME S11-U F-TEIDThis IE shall be present in the message sent from the target MME to the SGW selected by the target MME for indirect data forwarding, during a TAU procedure with SGW change and data forwarding, with Control Plane CIoT EPS optimisation, as specified in clause 5.3.3.1A of 3GPP TS 23.401 [3]."})
group_list[("Bearer Context", "Create Indirect Data Forwarding Tunnel Request", "")] = { "index" : "193", "type" : "93", "context" : "Create Indirect Data Forwarding Tunnel Request", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 7.2.19-2: Bearer Context within Create Indirect Data Forwarding Tunnel Response] Index = 80
ies = []
paragraph = "Table 7.2.19-2: Bearer Context within Create Indirect Data Forwarding Tunnel Response"
ies.append({ "ie_type" : "EBI", "ie_value" : "EPS Bearer ID", "presence" : "M", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "Cause", "ie_value" : "Cause", "presence" : "M", "instance" : "0", "comment" : "This IE shall indicate if the tunnel setup was successful, and if not, gives information on the reason."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "S1-U SGW F-TEID for DL data forwarding", "presence" : "C", "instance" : "0", "comment" : "This IE shall be included in the response sent from the SGW selected by the source MME for indirect data forwarding to the source MME. See NOTE 3."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "S12 SGW F-TEID for DL data forwarding", "presence" : "C", "instance" : "1", "comment" : "S12 usage only.This IE shall be included in the response sent from the SGW selected by the source SGSN for indirect data forwarding to the source SGSN. See NOTE 3."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "S4-U SGW F-TEID for DL data forwarding", "presence" : "C", "instance" : "2", "comment" : "S4-U usage only.This IE shall be included in the response sent from the SGW selected by the source SGSN for indirect data forwarding to the source SGSN. See NOTE 3."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "SGW F-TEID for DL data forwarding", "presence" : "C", "instance" : "3", "comment" : "This IE shall be included in the response message sent from the SGW selected by the target MME/SGSN for indirect data forwarding to the target MME/SGSN. See NOTE 3."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "S1-U SGW F-TEID for UL data forwarding", "presence" : "O", "instance" : "4", "comment" : "If available this IE may be included in the response sent during the intra-EUTRAN HO from the SGW  selected by the source MME for indirect data forwarding to the source MME. See NOTE 4."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "SGW F-TEID for UL data forwarding", "presence" : "O", "instance" : "5", "comment" : "If available this IE may be included in the response message sent during the intra-EUTRAN HO from the SGW selected by the target MME for indirect data forwarding to the target MME. See NOTE 4."})
group_list[("Bearer Context", "Create Indirect Data Forwarding Tunnel Response", "")] = { "index" : "193", "type" : "93", "context" : "Create Indirect Data Forwarding Tunnel Response", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 7.2.22-2: Load Control Information within Release Access Bearers Response] Index = 83
ies = []
paragraph = "Table 7.2.22-2: Load Control Information within Release Access Bearers Response"
ies.append({ "ie_type" : "Sequence Number", "ie_value" : "Load Control Sequence Number", "presence" : "M", "instance" : "0", "comment" : "See clause 12.2.5.1.2.1 for the description and use of this parameter."})
ies.append({ "ie_type" : "Metric", "ie_value" : "Load Metric", "presence" : "M", "instance" : "0", "comment" : "See clause 12.2.5.1.2.2 for the description and use of this parameter."})
group_list[("Load Control Information", "Release Access Bearers Response", "")] = { "index" : "281", "type" : "181", "context" : "Release Access Bearers Response", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 7.2.22-3: Overload Control Information within Release Access Bearers Response] Index = 84
ies = []
paragraph = "Table 7.2.22-3: Overload Control Information within Release Access Bearers Response"
ies.append({ "ie_type" : "Sequence Number", "ie_value" : "Overload Control Sequence Number", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.1 for the description and use of this parameter."})
ies.append({ "ie_type" : "Metric", "ie_value" : "Overload Reduction Metric", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.3 for the description and use of this parameter."})
ies.append({ "ie_type" : "EPC Timer", "ie_value" : "Period of Validity", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.2 for the description and use of this parameter.This IE should be set to 0 if the Overload Reduction Metric is null. This IE shall be ignored by the receiver if the Overload Reduction Metric is null."})
group_list[("Overload Control Information", "Release Access Bearers Response", "")] = { "index" : "280", "type" : "180", "context" : "Release Access Bearers Response", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 7.2.24-2: Bearer Context to be modified within Modify Access Bearers Request] Index = 87
ies = []
paragraph = "Table 7.2.24-2: Bearer Context to be modified within Modify Access Bearers Request"
ies.append({ "ie_type" : "EBI", "ie_value" : "EPS Bearer ID", "presence" : "M", "instance" : "0", "comment" : "See NOTE 1."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "S1-U eNodeB F-TEID", "presence" : "C", "instance" : "0", "comment" : "This IE shall be sent if S1-U is being used for:an UE triggered Service Request;S1-based Handover without SGW relocation;X2-based handover without SGW relocation;in S1-U GTP-U tunnel setup procedure during an Inter-MME E-UTRAN Tracking Area Update without SGW Change procedure or Intra-MME E-UTRAN Tracking Area Update without SGW Change procedure with Active Flag   (see 3GPP TS 24.301 [23]);an E-UTRAN Initiated E-RAB modification procedure;an UE initiated Connection Resume;the Establishment of S1-U bearer during Data Transport in Control Plane CIoT EPS optimisation procedure. See NOTE 2.If an MME is aware that the eNodeB supports both IP address types, the MME shall send both IP addresses within an F-TEID IE. If only one IP address is included, then the SGW shall assume that the eNodeB does not support the other IP address type."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "S11-U MME F-TEID", "presence" : "CO", "instance" : "1", "comment" : "This IE shall be sent on the S11 interface if S11-U is being used, i.e. for the following procedures:Mobile Originated Data transport in Control Plane CIoT EPS optimisation with P-GW connectivityMobile Terminated Data Transport in Control Plane CIoT EPS optimisation with P-GW connectivity "})
group_list[("Bearer Context", "Modify Access Bearers Request", "modified")] = { "index" : "193", "type" : "93", "context" : "Modify Access Bearers Request", "action" : "modified", "paragraph" : paragraph, "ies" : ies }
# [Table 7.2.24-3: Bearer Context to be removed within Modify Access Bearers Request] Index = 88
ies = []
paragraph = "Table 7.2.24-3: Bearer Context to be removed within Modify Access Bearers Request"
ies.append({ "ie_type" : "EBI", "ie_value" : "EPS Bearer ID", "presence" : "M", "instance" : "0", "comment" : ""})
group_list[("Bearer Context", "Modify Access Bearers Request", "removed")] = { "index" : "193", "type" : "93", "context" : "Modify Access Bearers Request", "action" : "removed", "paragraph" : paragraph, "ies" : ies }
# [Table 7.2.25-2: Bearer Context modified within Modify Access Bearers Response] Index = 90
ies = []
paragraph = "Table 7.2.25-2: Bearer Context modified within Modify Access Bearers Response"
ies.append({ "ie_type" : "EBI", "ie_value" : "EPS Bearer ID", "presence" : "M", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "Cause", "ie_value" : "Cause", "presence" : "M", "instance" : "0", "comment" : "This IE shall indicate if the bearer handling was successful, and if not, gives information on the reason."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "S1-U SGW F-TEID", "presence" : "C", "instance" : "0", "comment" : "This IE shall be present on the S11 interface if S1-U is being used, i.e. if the S11-U Tunnel flag was not set in the Modify Access Bearers Request. The SGW may change the GTP-U F-TEID value if the Change F-TEID support Indication flag was set to 1 in the Modify Access Bearers Request. Otherwise, the SGW shall return the currently allocated GTP-U F-TEID value.See NOTE 1."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "S11-U SGW F-TEID", "presence" : "C", "instance" : "1", "comment" : "This IE shall be present on the S11 interface if S11-U is being used, i.e. if the S11-U Tunnel flag was set in the Modify Access Bearers Request. If the Change F-TEID support Indication flag was set to 1 in the Modify Bearer Request and the SGW needs to change the F-TEID, the SGW shall include the new GTP-U F-TEID value. Otherwise, the SGW shall return the currently allocated GTP-U F-TEID value. "})
group_list[("Bearer Context", "Modify Access Bearers Response", "modified")] = { "index" : "193", "type" : "93", "context" : "Modify Access Bearers Response", "action" : "modified", "paragraph" : paragraph, "ies" : ies }
# [Table 7.2.25-3: Bearer Context marked for removal within Modify Access Bearers Response] Index = 91
ies = []
paragraph = "Table 7.2.25-3: Bearer Context marked for removal within Modify Access Bearers Response"
ies.append({ "ie_type" : "EBI", "ie_value" : "EPS Bearer ID", "presence" : "M", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "Cause", "ie_value" : "Cause", "presence" : "M", "instance" : "0", "comment" : "This IE shall indicate if the bearer handling was successful, and if not, gives information on the reason."})
group_list[("Bearer Context", "Modify Access Bearers Response", "removal")] = { "index" : "193", "type" : "93", "context" : "Modify Access Bearers Response", "action" : "removal", "paragraph" : paragraph, "ies" : ies }
# [Table 7.2.25-4: Load Control Information within Modify Access Bearers Response] Index = 92
ies = []
paragraph = "Table 7.2.25-4: Load Control Information within Modify Access Bearers Response"
ies.append({ "ie_type" : "Sequence Number", "ie_value" : "Load Control Sequence Number", "presence" : "M", "instance" : "0", "comment" : "See clause 12.2.5.1.2.1 for the description and use of this parameter."})
ies.append({ "ie_type" : "Metric", "ie_value" : "Load Metric", "presence" : "M", "instance" : "0", "comment" : "See clause 12.2.5.1.2.2 for the description and use of this parameter."})
group_list[("Load Control Information", "Modify Access Bearers Response", "")] = { "index" : "281", "type" : "181", "context" : "Modify Access Bearers Response", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 7.2.25-5: Overload Control Information within Modify Access Bearers Response] Index = 93
ies = []
paragraph = "Table 7.2.25-5: Overload Control Information within Modify Access Bearers Response"
ies.append({ "ie_type" : "Sequence Number", "ie_value" : "Overload Control Sequence Number", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.1 for the description and use of this parameter."})
ies.append({ "ie_type" : "Metric", "ie_value" : "Overload Reduction Metric", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.3 for the description and use of this parameter."})
ies.append({ "ie_type" : "EPC Timer", "ie_value" : "Period of Validity", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.2 for the description and use of this parameter.This IE should be set to 0 if the Overload Reduction Metric is null. This IE shall be ignored by the receiver if the Overload Reduction Metric is null."})
group_list[("Overload Control Information", "Modify Access Bearers Response", "")] = { "index" : "280", "type" : "180", "context" : "Modify Access Bearers Response", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 7.2.26-2: Remote UE Context Connected within Remote UE Report Notification] Index = 95
ies = []
paragraph = "Table 7.2.26-2: Remote UE Context Connected within Remote UE Report Notification"
ies.append({ "ie_type" : "Remote User ID", "ie_value" : "Remote User ID", "presence" : "M", "instance" : "0", "comment" : "See clause 8.123 for the description and use of this parameter"})
ies.append({ "ie_type" : "Remote UE IP Information", "ie_value" : "Remote UE IP Information", "presence" : "M", "instance" : "0", "comment" : "See clause 8.124 for the description and use of this parameter"})
group_list[("Remote UE Context", "Remote UE Report Notification", "")] = { "index" : "291", "type" : "191", "context" : "Remote UE Report Notification", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 7.2.26-3: Remote UE Context Disconnected with Remote UE Report Notification ] Index = 96
ies = []
paragraph = "Table 7.2.26-3: Remote UE Context Disconnected with Remote UE Report Notification "
ies.append({ "ie_type" : "Remote User ID", "ie_value" : "Remote User ID", "presence" : "M", "instance" : "0", "comment" : "See clause 8.123 for the description and use of this parameter"})
group_list[("Remote UE Context", "", "")] = { "index" : "291", "type" : "191", "context" : "", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 7.3.1-2: MME/SGSN/AMF UE EPS PDN Connections within Forward Relocation Request] Index = 99
ies = []
paragraph = "Table 7.3.1-2: MME/SGSN/AMF UE EPS PDN Connections within Forward Relocation Request"
ies.append({ "ie_type" : "APN", "ie_value" : "APN", "presence" : "M", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "APN Restriction", "ie_value" : "APN Restriction", "presence" : "C", "instance" : "0", "comment" : "This IE denotes the restriction on the combination of types of APN for the APN associated with this EPS bearer Context. The target MME or SGSN determines the Maximum APN Restriction using the APN Restriction. If available, the source MME/S4SGSN shall include this IE."})
ies.append({ "ie_type" : "Selection Mode", "ie_value" : "Selection Mode", "presence" : "CO", "instance" : "0", "comment" : "When available, this IE shall be included by the source MME/S4-SGSN/AMF."})
ies.append({ "ie_type" : "IP Address", "ie_value" : "IPv4 Address", "presence" : "C", "instance" : "0", "comment" : "This IE shall not be included if no IPv4 Address is assigned. See NOTE 1."})
type_list["IP Address"]["max_instance"] = "1"
ies.append({ "ie_type" : "IP Address", "ie_value" : "IPv6 Address", "presence" : "C", "instance" : "1", "comment" : "This IE shall not be included if no IPv6 Address is assigned."})
ies.append({ "ie_type" : "EBI", "ie_value" : "Linked EPS Bearer ID", "presence" : "M", "instance" : "0", "comment" : "This IE identifies the default bearer of the PDN Connection."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "PGW S5/S8 IP Address for Control Plane or PMIP", "presence" : "M", "instance" : "0", "comment" : "This IE shall include the TEID in the GTP based S5/S8 case and the uplink GRE key in the PMIP based S5/S8 case.See NOTE 4."})
ies.append({ "ie_type" : "FQDN", "ie_value" : "PGW node name", "presence" : "C", "instance" : "0", "comment" : "This IE shall be included if the source MME, SGSN or AMF has the PGW FQDN."})
ies.append({ "ie_type" : "Bearer Context", "ie_value" : "Bearer Contexts ", "presence" : "C", "instance" : "0", "comment" : "Several IEs with this type and instance values may be included as necessary to represent a list of Bearers."})
ies.append({ "ie_type" : "AMBR", "ie_value" : "Aggregate Maximum Bit Rate", "presence" : "M", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "Charging Characteristics", "ie_value" : "Charging characteristics", "presence" : "C", "instance" : "0", "comment" : "This IE shall be present if charging characteristics was supplied by the HSS to the MME/SGSN, or by the UDM to the SMF, as a part of subscription information."})
ies.append({ "ie_type" : "Change Reporting Action", "ie_value" : "Change Reporting Action", "presence" : "C", "instance" : "0", "comment" : "This IE shall be included whenever available at the source MME/SGSN. See NOTE 5."})
ies.append({ "ie_type" : "CSG Information Reporting Action", "ie_value" : "CSG Information Reporting Action", "presence" : "CO", "instance" : "0", "comment" : "This IE shall be included whenever available at the source MME/SGSN."})
ies.append({ "ie_type" : "eNB Information Reporting", "ie_value" : "HNB Information Reporting ", "presence" : "CO", "instance" : "0", "comment" : "This IE shall be included whenever available at the source MME/SGSN."})
ies.append({ "ie_type" : "Indication", "ie_value" : "Indication Flags", "presence" : "CO", "instance" : "0", "comment" : "This IE shall be included if any of the flags are set to 1.Change Reporting support indication flag: This flag shall be set to 1 if the Source S4-SGSN/MME supports Location Change Reporting mechanism and if the S4-SGSN/MME has indicated the support for the Location Change Reporting mechanism to the PGW, during the session establishment and/or modification procedures. See NOTE 2.CSG Change Reporting Support Indication flag: This flag shall be set to 1 if the Source S4-SGSN/MME supports CSG Information Change Reporting mechanism and if the S4-SGSN/MME has indicated the support for the CSG Informatoin Change Reporting to the PGW, during the session establishment and/or modification procedures. See NOTE 2. Delay Tolerant Connection Indication flag: This flag shall be set to 1 on the S3/S10/S16 interface by the source MME/SGSN if the PGW indicated that this PDN Connection is delay tolerant.Extended PCO Support Indication flag: This flag shall be set to 1 on S10/N26 interface by the source MME if the UE and the source MME support Extended PCO. It shall be set to 1 on the N26 interface during a 5GS to EPS handover. NO 5GS N26 mobility Indication flag: This flag shall be set to 1 on S10 interface if the PDN connection cannot be moved to 5GS via N26."})
ies.append({ "ie_type" : "Signalling Priority Indication", "ie_value" : "Signalling Priority Indication  ", "presence" : "CO", "instance" : "0", "comment" : "The source SGSN/MME shall include this IE if the UE indicated low access priority when establishing the PDN connection."})
ies.append({ "ie_type" : "Change to Report Flags", "ie_value" : "Change to Report Flags", "presence" : "CO", "instance" : "0", "comment" : "This IE shall be included by the SGSN if any one of the applicable flags is set to 1. See NOTE3.Applicable flags:Serving Network Change to Report: This flag shall be set to 1 if the source SGSN has detected a Serving Network change during a RAU procedure without SGSN change but has not yet reported this change to the PGW.Time Zone Change to Report: This flag shall be set to 1 if the source SGSN has detected a UE Time Zone change during a RAU procedure without SGSN change but has not yet reported this change to the PGW."})
type_list["FQDN"]["max_instance"] = "1"
ies.append({ "ie_type" : "FQDN", "ie_value" : "Local Home Network ID", "presence" : "CO", "instance" : "1", "comment" : "This IE shall be sent over the S3/S10/S16 interface if SIPTO at the Local Network is active for the PDN connection in the SIPTO at the Local Network architecture with stand-alone GW. "})
ies.append({ "ie_type" : "Presence Reporting Area Action", "ie_value" : "Presence Reporting Area Action", "presence" : "CO", "instance" : "0", "comment" : "This IE shall be included if the PGW requested the source MME/SGSN to report changes of UE presence in a Presence Reporting Area. The source MME/SGSN shall include the Presence Reporting Area Identifier and, if received from the PGW, the list of the Presence Reporting Area elements.Several IEs with the same type and instance value may be included as necessary to represent a list of Presence Reporting Area Actions. One IE shall be included for each Presence Reporting Area."})
ies.append({ "ie_type" : "WLAN Offloadability Indication", "ie_value" : "WLAN Offloadability Indication", "presence" : "CO", "instance" : "0", "comment" : "If the MME/SGSN supports WLAN/3GPP Radio Interworking with RAN rules then this IE shall be included on S3/S10/S16 if the UE has been authorized to perform WLAN offload for at least one RAT."})
ies.append({ "ie_type" : "Remote UE Context", "ie_value" : "Remote UE Context Connected", "presence" : "CO", "instance" : "0", "comment" : "The source MME shall include this IE on the S10 interface during an inter MME mobility procedure if such information is available. Several IEs with the same type and instance value may be included as necessary to represent a list of remote UEs connected. "})
ies.append({ "ie_type" : "PDN Type", "ie_value" : "PDN Type", "presence" : "CO", "instance" : "0", "comment" : "The source MME/SGSN/AMF shall include this IE on the S10/S3/S16/N26 interface, for a Non-IP PDN Connection, during an inter MME/SGSN/AMF mobility procedure, if the target MME/SGSN/AMF supports SGi Non-IP or Ethernet PDN connections."})
ies.append({ "ie_type" : "Header Compression Configuration", "ie_value" : "Header Compression Configuration", "presence" : "CO", "instance" : "0", "comment" : "This IE shall be sent over the S10 interface if the use of IP Header Compression for Control Plane CIoT EPS optimisations has been negotiated with the UE and the target MME is known to support CIoT EPS optimisations."})
group_list[("PDN Connection", "Forward Relocation Request", "")] = { "index" : "209", "type" : "109", "context" : "Forward Relocation Request", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 7.3.1-3: Bearer Context within MME/SGSN/AMF UE EPS PDN Connections within Forward Relocation Request] Index = 100
ies = []
paragraph = "Table 7.3.1-3: Bearer Context within MME/SGSN/AMF UE EPS PDN Connections within Forward Relocation Request"
ies.append({ "ie_type" : "EBI", "ie_value" : "EPS Bearer ID", "presence" : "M", "instance" : "0", "comment" : "See NOTE 3."})
ies.append({ "ie_type" : "Bearer TFT", "ie_value" : "TFT", "presence" : "C", "instance" : "0", "comment" : "This IE shall be present if a TFT is defined for this bearer."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "SGW S1/S4/S12 IP Address and TEID for user plane", "presence" : "M", "instance" : "0", "comment" : "This IE shall contain the SGW S1/S4/S12 IP Address and TEID for user plane. Over the N26 interface, the SMF (on behalf of the source AMF) shall set the IP address and TEID to the following values:-	any reserved TEID (e.g. all 0s, or all 1s);-	IPv4 address set to 0.0.0.0, or IPv6 Prefix Length and IPv6 prefix and Interface Identifier all set to zero.See NOTE2, NOTE 4."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "PGW S5/S8 IP Address and TEID for user plane", "presence" : "C", "instance" : "1", "comment" : "This IE shall be present for GTP based S5/S8."})
ies.append({ "ie_type" : "Bearer QoS", "ie_value" : "Bearer Level QoS", "presence" : "M", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "F-Container", "ie_value" : "BSS Container", "presence" : "CO", "instance" : "0", "comment" : "The MME/S4 SGSN shall include the Packet Flow ID, Radio Priority, SAPI, PS Handover XID parameters in the TAU/RAU/Handover procedure, if available. See Figure 8.48-2. The Container Type shall be set to 2."})
ies.append({ "ie_type" : "TI", "ie_value" : "Transaction Identifier", "presence" : "C", "instance" : "0", "comment" : "This IE shall be sent over S3/S10/S16 if the UE supports A/Gb and/or Iu mode."})
ies.append({ "ie_type" : "Bearer Flags", "ie_value" : "Bearer Flags", "presence" : "CO", "instance" : "0", "comment" : "Applicable flags:vSRVCC indicator: This IE shall be sent by the source MME to the target MME on the S10 interface if vSRVCC indicator is available in the source MME. ASI (Activity Status Indicator): the source S4-SGSN shall set this indicator to 1 on the S16 interface if the bearer context is preserved in the CN without an associated RAB."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "SGW S11 IP Address and TEID for user plane", "presence" : "CO", "instance" : "2", "comment" : "This IE shall be present if available. See NOTE 2."})
group_list[("Bearer Context", "Forward Relocation Request", "")] = { "index" : "193", "type" : "93", "context" : "Forward Relocation Request", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 7.3.1-4: Remote UE Context Connected within MME/SGSN UE EPS PDN Connections within Forward Relocation Request] Index = 101
ies = []
paragraph = "Table 7.3.1-4: Remote UE Context Connected within MME/SGSN UE EPS PDN Connections within Forward Relocation Request"
ies.append({ "ie_type" : "Remote User ID", "ie_value" : "Remote User ID", "presence" : "M", "instance" : "0", "comment" : "See clause 8.123 for the description and use of this parameter"})
ies.append({ "ie_type" : "Remote UE IP Information", "ie_value" : "Remote UE IP Information", "presence" : "M", "instance" : "0", "comment" : "See clause 8.124 for the description and use of this parameter"})
group_list[("Remote UE Context", "Forward Relocation Request", "")] = { "index" : "291", "type" : "191", "context" : "Forward Relocation Request", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 7.3.1-5: MME UE SCEF PDN Connections within Forward Relocation Request] Index = 102
ies = []
paragraph = "Table 7.3.1-5: MME UE SCEF PDN Connections within Forward Relocation Request"
ies.append({ "ie_type" : "APN", "ie_value" : "APN", "presence" : "M", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "EBI", "ie_value" : "Default EPS Bearer ID", "presence" : "M", "instance" : "0", "comment" : "This IE shall identify the default bearer of the SCEF PDN Connection."})
ies.append({ "ie_type" : "Node Identifier", "ie_value" : "SCEF ID", "presence" : "M", "instance" : "0", "comment" : "This IE shall include the SCEF Identifier and the SCEF Realm for the APN."})
group_list[("SCEF PDN Connection", "Forward Relocation Request", "")] = { "index" : "295", "type" : "195", "context" : "Forward Relocation Request", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 7.3.1-6: Subscribed V2X Information within Forward Relocation Request] Index = 103
ies = []
paragraph = "Table 7.3.1-6: Subscribed V2X Information within Forward Relocation Request"
ies.append({ "ie_type" : "Services Authorized", "ie_value" : "LTE V2X Services Authorized", "presence" : "C", "instance" : "0", "comment" : "This IE shall be included to indicate the authorization status of the UE to use the LTE sidelink for V2X services."})
type_list["Services Authorized"]["max_instance"] = "1"
ies.append({ "ie_type" : "Services Authorized", "ie_value" : "NR V2X Services Authorized", "presence" : "C", "instance" : "1", "comment" : "This IE shall be included to indicate the authorization status of the UE to use the NR sidelink for V2X services."})
ies.append({ "ie_type" : "Bit Rate", "ie_value" : "LTE UE Sidelink Aggregate Maximum Bit Rate", "presence" : "C", "instance" : "0", "comment" : "This IE shall be included if the UE is authorized for LTE V2X services."})
type_list["Bit Rate"]["max_instance"] = "1"
ies.append({ "ie_type" : "Bit Rate", "ie_value" : "NR UE Sidelink Aggregate Maximum Bit Rate", "presence" : "C", "instance" : "1", "comment" : "This IE shall be included if the UE is authorized for NR V2X services."})
ies.append({ "ie_type" : "PC5 QoS Parameters", "ie_value" : "PC5 QoS Parameters", "presence" : "C", "instance" : "0", "comment" : "This IE shall be included if the UE is authorized for NR V2X services."})
group_list[("V2X Context", "Forward Relocation Request", "")] = { "index" : "102", "type" : "2", "context" : "Forward Relocation Request", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 7.3.1-7: PC5 QoS Parameters within Forward Relocation Request] Index = 104
ies = []
paragraph = "Table 7.3.1-7: PC5 QoS Parameters within Forward Relocation Request"
ies.append({ "ie_type" : "PC5 QoS Flow", "ie_value" : "PC5 QoS Flows", "presence" : "M", "instance" : "0", "comment" : "Several IEs with this type and same instance value may be included as necessary to represent a list of PC5 QoS Flows."})
ies.append({ "ie_type" : "Bit Rate", "ie_value" : "PC5 Link Aggregated Bit Rates", "presence" : "O", "instance" : "0", "comment" : "This IE may be included for the non-GBR PC5 QoS Flows."})
group_list[("PC5 QoS Parameters", "Forward Relocation Request", "")] = { "index" : "105", "type" : "5", "context" : "Forward Relocation Request", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 7.3.2-2: Bearer Context ] Index = 106
ies = []
paragraph = "Table 7.3.2-2: Bearer Context "
ies.append({ "ie_type" : "EBI", "ie_value" : "EPS Bearer ID", "presence" : "C", "instance" : "0", "comment" : "This IE shall be included if the message is used for S1-Based handover procedure, 5GS to EPS handover or EPS to 5GS handover. This IE shall be included if the message is used for SRNS relocation procedure and Inter RAT handover to/from Iu mode procedures."})
ies.append({ "ie_type" : "Packet Flow ID", "ie_value" : "Packet Flow ID", "presence" : "C", "instance" : "0", "comment" : "This IE shall be included if the message is used for PS handover and Inter RAT handover to/from A/Gb mode procedures."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "eNodeB F-TEID for DL data forwarding", "presence" : "C", "instance" : "0", "comment" : "This IE shall be included for the message sent from the target MME, if the DL Transport Layer Address and DL GTP TEID are included in the SAE Bearers Admitted List of the S1AP: HANDOVER REQUEST ACKNOWLEDGE and direct forwarding or indirect forwarding without SGW change is applied."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "eNodeB F-TEID for UL data forwarding", "presence" : "O", "instance" : "1", "comment" : "This IE may be included for the message sent from the target MME during the intra-EUTRAN HO, if the UL Transport Layer Address and UL GTP TEID are included in the SAE Bearers Admitted List of the S1AP: HANDOVER REQUEST ACKNOWLEDGE and direct forwarding or indirect forwarding without SGW change is applied."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "SGW/UPF F-TEID for DL data forwarding", "presence" : "C", "instance" : "2", "comment" : "This IE shall be included when indirect data forwarding with SGW change is applied. "})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "RNC F-TEID for DL data forwarding", "presence" : "C", "instance" : "3", "comment" : "This RNC F-TEID shall be included in the message sent from SGSN, if the target system decides using RNC F-TEID for data forwarding."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "SGSN F-TEID for DL data forwarding", "presence" : "C", "instance" : "4", "comment" : "This SGSN F-TEID shall be included in the message sent from SGSN, if the target system decides using SGSN F-TEID for data forwarding."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "SGW F-TEID for UL data forwarding", "presence" : "O", "instance" : "5", "comment" : "If available this SGW F-TEID may be included when indirect data forwarding with SGW change is applied, during the intra-EUTRAN HO."})
group_list[("Bearer Context", "", "")] = { "index" : "193", "type" : "93", "context" : "", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 7.3.6-2: MME/SGSN/AMF UE EPS PDN Connections within Context Response] Index = 111
ies = []
paragraph = "Table 7.3.6-2: MME/SGSN/AMF UE EPS PDN Connections within Context Response"
ies.append({ "ie_type" : "APN", "ie_value" : "APN", "presence" : "M", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "APN Restriction", "ie_value" : "APN Restriction", "presence" : "C", "instance" : "0", "comment" : "This IE denotes the restriction on the combination of types of APN for the APN associated with this EPS bearer Context. The target MME or SGSN determines the Maximum APN Restriction using the APN Restriction. If available, the source MME/S4 SGSN shall include this IE. "})
ies.append({ "ie_type" : "Selection Mode", "ie_value" : "Selection Mode", "presence" : "CO", "instance" : "0", "comment" : "When available, this IE shall be included by the source MME/S4-SGSN/AMF."})
ies.append({ "ie_type" : "IP Address", "ie_value" : "IPv4 Address", "presence" : "C", "instance" : "0", "comment" : "This IE shall not be included if no IPv4 Address is assigned. See NOTE 1. See NOTE 5."})
ies.append({ "ie_type" : "IP Address", "ie_value" : "IPv6 Address", "presence" : "C", "instance" : "1", "comment" : "This IE shall not be included if no IPv6 Address is assigned. See NOTE 5."})
ies.append({ "ie_type" : "EBI", "ie_value" : "Linked EPS Bearer ID", "presence" : "M", "instance" : "0", "comment" : "This IE identifies the default bearer of the PDN Connection."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "PGW S5/S8 IP Address for Control Plane or PMIP", "presence" : "M", "instance" : "0", "comment" : "This IE shall include the TEID in the GTP based S5/S8 case and the uplink GRE key in the PMIP based S5/S8 case.See NOTE 3."})
ies.append({ "ie_type" : "FQDN", "ie_value" : "PGW node name", "presence" : "C", "instance" : "0", "comment" : "This IE shall be included if the source MME, SGSN or AMF has the PGW FQDN."})
ies.append({ "ie_type" : "Bearer Context", "ie_value" : "Bearer Contexts ", "presence" : "M", "instance" : "0", "comment" : "Several IEs with this type and instance values may be included as necessary to represent a list of Bearers."})
ies.append({ "ie_type" : "AMBR", "ie_value" : "Aggregate Maximum Bit Rate", "presence" : "M", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "Charging Characteristics", "ie_value" : "Charging characteristics", "presence" : "C", "instance" : "0", "comment" : "This IE shall be present if charging characteristics was supplied by the HSS to the MME/SGSN, or by the UDM to the SMF, as a part of subscription information."})
ies.append({ "ie_type" : "Change Reporting Action", "ie_value" : "Change Reporting Action", "presence" : "C", "instance" : "0", "comment" : "This IE shall be included whenever available at the source MME/SGSN. See NOTE 4."})
ies.append({ "ie_type" : "CSG Information Reporting Action", "ie_value" : "CSG Information Reporting Action", "presence" : "CO", "instance" : "0", "comment" : "This IE shall be included whenever available at the source MME/SGSN."})
ies.append({ "ie_type" : "eNB Information Reporting", "ie_value" : "HNB Information Reporting ", "presence" : "CO", "instance" : "0", "comment" : "This IE shall be included whenever available at the source MME/SGSN."})
ies.append({ "ie_type" : "Indication", "ie_value" : "Indication flags", "presence" : "CO", "instance" : "0", "comment" : "This IE shall be included if any one of the applicable flags is set to 1.Applicable flags:Subscribed QoS Change Indication: This flag shall be set to 1 if the subscribed QoS profile of the related PDN connection has changed in the old MME/SGSN when the UE is in ECM-IDLE state and ISR is activated. Change Reporting support indication flag: This flag shall be set to 1 if the source S4-SGSN/MME supports Location Change Reporting mechanism and if the S4-SGSN/MME has indicated the support for the Location Change Reporting mechanism to the PGW, during the session establishment and/or modification procedures. See NOTE 2.CSG Change Reporting Support Indication flag: This flag shall be set to 1 if the Source S4-SGSN/MME supports CSG Information Change Reporting mechanism and if the S4-SGSN/MME has indicated the support for the CSG Information Change Reporting to the PGW, during the session establishment and/or modification procedures. See NOTE 2. Pending Subscription Change Indication flag: This flag shall be set to 1 if the source MME has received Subscribed QoS profile updates for QCI/ARP/APN-AMBR from the HSS but has deferred the reporting of these updates to the PGW/PCRF because the UE was not reachable. Pending Network Initiated PDN Connection Signalling Indication flag: This flag shall be set to 1 by the source MME/SGSN if there is pending network initiated signalling for the PDN connection.Delay Tolerant Connection Indication flag: This flag shall be set to 1 interface by the source MME/SGSN if the PGW indicated that this PDN Connection is delay tolerant.Extended PCO Support Indication flag: This flag shall be set to 1 on S10/N26 interface by the source MME if the UE and the source MME support Extended PCO. It shall be set to 1 on the N26 interface during a 5GS to EPS Idle mode mobility.Control Plane Only PDN Connection Indication: This flag shall be set to 1 if the PDN Connection is set to Control Plane Only.NO 5GS N26 mobility Indication flag: This flag shall be set to 1 on S10 interface if the PDN connection cannot be moved to 5GS via N26."})
ies.append({ "ie_type" : "Signalling Priority Indication", "ie_value" : "Signalling Priority Indication  ", "presence" : "CO", "instance" : "0", "comment" : "The source SGSN/MME shall include this IE if the UE indicated low access priority when establishing the PDN connection."})
ies.append({ "ie_type" : "Change to Report Flags", "ie_value" : "Change to Report Flags", "presence" : "CO", "instance" : "0", "comment" : "This IE shall be included by the MME/SGSN if any one of the applicable flags is set to 1.Applicable flags:Serving Network Change to Report: This flag shall be set to 1 if the source MME/SGSN has detected a Serving Network change during a TAU/RAU procedure without MME/SGSN change but has not yet reported this change to the PGW.Time Zone Change to Report: This flag shall be set to 1 if the source MME/SGSN has detected a UE Time Zone change during a TAU/RAU procedure without MME/SGSN change but has not yet reported this change to the PGW."})
ies.append({ "ie_type" : "FQDN", "ie_value" : "Local Home Network ID", "presence" : "CO", "instance" : "1", "comment" : "This IE shall be sent over the S3/S10/S16 interface if SIPTO at the Local Network is active for the PDN connection in the SIPTO at the Local Network architecture with stand-alone GW. "})
ies.append({ "ie_type" : "Presence Reporting Area Action", "ie_value" : "Presence Reporting Area Action", "presence" : "CO", "instance" : "0", "comment" : "This IE shall be included if the PGW requested the source MME/SGSN to report changes of UE presence in a Presence Reporting Area. The source MME/SGSN shall include the Presence Reporting Area Identifier and, if received from the PGW, the list of the Presence Reporting Area elements.Several IEs with the same type and instance value may be included as necessary to represent a list of Presence Reporting Area Actions. One IE shall be included for each Presence Reporting Area."})
ies.append({ "ie_type" : "WLAN Offloadability Indication", "ie_value" : "WLAN Offloadability Indication", "presence" : "CO", "instance" : "0", "comment" : "If the MME/SGSN supports WLAN/3GPP Radio Interworking with RAN rules then this IE shall be included on S3/S10/S16 if the UE has been authorized to perform WLAN offload for at least one RAT. "})
ies.append({ "ie_type" : "Remote UE Context", "ie_value" : "Remote UE Context Connected", "presence" : "CO", "instance" : "0", "comment" : "The source MME shall include this IE on the S10 interface during an inter MME mobility procedure if such information is available. Several IEs with the same type and instance value may be included as necessary to represent a list of remote UEs connected. "})
ies.append({ "ie_type" : "PDN Type", "ie_value" : "PDN Type", "presence" : "CO", "instance" : "0", "comment" : "The source MME/SGSN/AMF shall include this IE on the S10/S3/S16/N26 interface, for a Non-IP or Ethernet PDN Connection, during an inter MME/SGSN/AMF mobility procedure if the new MME/SGSN/AMF supports non-IP or Ethernet PDN connection using SGi as indicated in the Context Request message."})
ies.append({ "ie_type" : "Header Compression Configuration", "ie_value" : "Header Compression Configuration", "presence" : "CO", "instance" : "0", "comment" : "This IE shall be sent over the S10 interface if the use of IP Header Compression for Control Plane CIoT EPS optimisations has been negotiated with the UE and the target MME has set the IHCSI bit of the CIoT Optimizations Support Indication IE to 1 in the Context Request as specified in clause 8.125."})
group_list[("PDN Connection", "Context Response", "")] = { "index" : "209", "type" : "109", "context" : "Context Response", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 7.3.6-3: Bearer Context within MME/SGSN/AMF UE EPS PDN Connections within Context Response] Index = 112
ies = []
paragraph = "Table 7.3.6-3: Bearer Context within MME/SGSN/AMF UE EPS PDN Connections within Context Response"
ies.append({ "ie_type" : "EBI", "ie_value" : "EPS Bearer ID", "presence" : "M", "instance" : "0", "comment" : "See NOTE 4."})
ies.append({ "ie_type" : "Bearer TFT", "ie_value" : "TFT", "presence" : "C", "instance" : "0", "comment" : "This IE shall be present if a TFT is defined for this bearer."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "SGW S1/S4/S12/S11 IP Address and TEID for user plane", "presence" : "C", "instance" : "0", "comment" : "The IE shall be present except if:the source and target MME/S4-SGSN support the MME/S4-SGSN triggered SGW restoration procedure, and the source MME/S4-SGSN has not performed the SGW relocation procedure after the SGW has failed as specified in 3GPP TS 23.007 [17].Over the N26 interface, the SMF (on behalf of the source AMF) shall set the IP address and TEID to the following values:-	any reserved TEID (e.g. all 0s, or all 1s);-	IPv4 address set to 0.0.0.0, or IPv6 Prefix Length and IPv6 prefix and Interface Identifier all set to zero.See NOTE 2, NOTE3 and NOTE 5."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "PGW S5/S8 IP Address and TEID for user plane", "presence" : "C", "instance" : "1", "comment" : "This IE shall be included for GTP based S5/S8."})
ies.append({ "ie_type" : "Bearer QoS", "ie_value" : "Bearer Level QoS", "presence" : "M", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "F-Container", "ie_value" : "BSS Container", "presence" : "CO", "instance" : "0", "comment" : "The MME/S4 SGSN shall include the Packet Flow ID, Radio Priority, SAPI, PS Handover XID parameters in the TAU/RAU/Handover procedure, if available. The Container Type shall be set to 2."})
ies.append({ "ie_type" : "TI", "ie_value" : "Transaction Identifier", "presence" : "C", "instance" : "0", "comment" : "This IE shall be sent over S3/S10/S16 if the UE supports A/Gb and/or Iu mode."})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "SGW S11 IP Address and TEID for user plane", "presence" : "CO", "instance" : "2", "comment" : "This IE shall be present if available. See NOTE 3."})
group_list[("Bearer Context", "Context Response", "")] = { "index" : "193", "type" : "93", "context" : "Context Response", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 7.3.6-4: Remote UE Context Connected within MME/SGSN UE EPS PDN Connections within Context Response] Index = 113
ies = []
paragraph = "Table 7.3.6-4: Remote UE Context Connected within MME/SGSN UE EPS PDN Connections within Context Response"
ies.append({ "ie_type" : "Remote User ID", "ie_value" : "Remote User ID", "presence" : "M", "instance" : "0", "comment" : "See clause 8.123 for the description and use of this parameter"})
ies.append({ "ie_type" : "Remote UE IP Information", "ie_value" : "Remote UE IP Information", "presence" : "M", "instance" : "0", "comment" : "See clause 8.124 for the description and use of this parameter"})
group_list[("Remote UE Context", "Context Response", "")] = { "index" : "291", "type" : "191", "context" : "Context Response", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 7.3.7-2: Bearer Context within Context Acknowledge] Index = 116
ies = []
paragraph = "Table 7.3.7-2: Bearer Context within Context Acknowledge"
ies.append({ "ie_type" : "EBI", "ie_value" : "EPS Bearer ID", "presence" : "M", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "F-TEID", "ie_value" : "Forwarding F-TEID", "presence" : "M", "instance" : "0", "comment" : "The interface type of the Forwarding F-TEID should be set to either: 23 (SGW/UPF GTP-U interface for DL data forwarding) for indirect forwarding, 0 ( S1-U eNodeB GTP-U interface) or 3 (S12 RNC GTP-U interface), if the eNB or RNC supports such forwarding, or15 (S4 SGSN GTP-U interface)."})
group_list[("Bearer Context", "Context Acknowledge", "")] = { "index" : "193", "type" : "93", "context" : "Context Acknowledge", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 8.28-1: Bearer Context Grouped Type] Index = 198
added_ies = group_list[("Bearer Context", "", "")]["ies"]
# [Table 8.39-1: PDN Connection Grouped Type] Index = 221
ies = []
paragraph = "Table 8.39-1: PDN Connection Grouped Type"
group_list[("PDN Connection", "", "")] = { "index" : "209", "type" : "109", "context" : "", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 8.111-1: Overload Control Information Grouped Type] Index = 326
ies = []
paragraph = "Table 8.111-1: Overload Control Information Grouped Type"
group_list[("Overload Control Information", "", "")] = { "index" : "280", "type" : "180", "context" : "", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 8.112-1: Load Control Information Grouped Type] Index = 327
ies = []
paragraph = "Table 8.112-1: Load Control Information Grouped Type"
group_list[("Load Control Information", "", "")] = { "index" : "281", "type" : "181", "context" : "", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 8.122-1: Remote UE Context Grouped Type] Index = 337
added_ies = group_list[("Bearer Context", "", "")]["ies"]
# [Table 8.126-1: PDN Connection Grouped Type] Index = 341
added_ies = group_list[("PDN Connection", "", "")]["ies"]
# [Table 8.138-1: V2X Context Grouped Type] Index = 356
ies = []
paragraph = "Table 8.138-1: V2X Context Grouped Type"
group_list[("V2X Context", "", "")] = { "index" : "102", "type" : "2", "context" : "", "action" : "", "paragraph" : paragraph, "ies" : ies }
# [Table 8.140-1: PC5 QoS Parameters Grouped Type] Index = 357
ies = []
paragraph = "Table 8.140-1: PC5 QoS Parameters Grouped Type"
group_list[("PC5 QoS Parameters", "", "")] = { "index" : "105", "type" : "5", "context" : "", "action" : "", "paragraph" : paragraph, "ies" : ies }
