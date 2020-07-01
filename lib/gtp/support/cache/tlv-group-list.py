ies = []
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
group_list["Bearer Context"] = { "index" : "193", "type" : "93", "ies" : ies }
added_ies = group_list["Bearer Context"]["ies"]
ies = []
ies.append({ "ie_type" : "Sequence Number", "ie_value" : "Overload Control Sequence Number", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.1 for the description and use of this parameter."})
ies.append({ "ie_type" : "Metric", "ie_value" : "Overload Reduction Metric", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.3 for the description and use of this parameter."})
ies.append({ "ie_type" : "EPC Timer", "ie_value" : "Period of Validity", "presence" : "M", "instance" : "0", "comment" : "See clause 12.3.5.1.2.2 for the description and use of this parameter.This IE should be set to 0 if the Overload Reduction Metric is null. This IE shall be ignored by the receiver if the Overload Reduction Metric is null."})
group_list["Overload Control Information"] = { "index" : "280", "type" : "180", "ies" : ies }
ies = []
ies.append({ "ie_type" : "Remote User ID", "ie_value" : "Remote User ID", "presence" : "M", "instance" : "0", "comment" : "See clause 8.123 for the description and use of this parameter"})
ies.append({ "ie_type" : "Remote UE IP Information", "ie_value" : "Remote UE IP Information", "presence" : "M", "instance" : "0", "comment" : "See clause 8.124 for the description and use of this parameter"})
group_list["Remote UE Context"] = { "index" : "291", "type" : "191", "ies" : ies }
added_ies = group_list["Bearer Context"]["ies"]
added_ies.append({ "ie_type" : "Cause", "ie_value" : "Cause", "presence" : "M", "instance" : "0", "comment" : "This IE shall indicate if the bearer handling was successful, and if not, it gives information on the reason. (NOTE 1, NOTE 2, NOTE 3)"})
added_ies.append({ "ie_type" : "Charging ID", "ie_value" : "Charging Id", "presence" : "C", "instance" : "0", "comment" : "This IE shall be included on the S5/S8 interface for an E-UTRAN initial attach, a Handover from Trusted or Untrusted Non-3GPP IP Access to E-UTRAN, a PDP Context Activation, a Handover from Trusted or Untrusted Non-3GPP IP Access to UTRAN/GERAN and a UE requested PDN connectivity."})
added_ies.append({ "ie_type" : "Bearer Flags", "ie_value" : "Bearer Flags", "presence" : "O", "instance" : "0", "comment" : "Applicable flags are:PPC (Prohibit Payload Compression) : this flag may be set on the S5/S8 and S4 interfaces."})
group_list["Bearer Context"] = { "index" : "193", "type" : "93", "ies" : added_ies }
added_ies = group_list["Bearer Context"]["ies"]
ies = []
ies.append({ "ie_type" : "Sequence Number", "ie_value" : "Load Control Sequence Number", "presence" : "M", "instance" : "0", "comment" : "See clause 12.2.5.1.2.1 for the description and use of this parameter."})
ies.append({ "ie_type" : "Metric", "ie_value" : "Load Metric", "presence" : "M", "instance" : "0", "comment" : "See clauses 12.2.5.1.2.2 and 12.2.5.1.2.3 for the description and use of this parameter."})
ies.append({ "ie_type" : "APN and Relative Capacity", "ie_value" : "List of APN and Relative Capacity", "presence" : "CO", "instance" : "0", "comment" : "The IE shall (only) be present in the PGWs APN level Load Control Information IE.For indicating the APN level load, the PGW shall include one or more instances of this IE, up to maximum of 10, with the same type and instance value, representing a list of APN(s) & its respective Relative Capacity (sharing the same Load Metric).See clause 12.2.5.1.2.3 for the description and use of this parameter.See NOTE 1."})
group_list["Load Control Information"] = { "index" : "281", "type" : "181", "ies" : ies }
added_ies = group_list["Overload Control Information"]["ies"]
added_ies.append({ "ie_type" : "APN", "ie_value" : "List of Access Point Name", "presence" : "CO", "instance" : "0", "comment" : "The IE may (only) be present in the PGWs Overload Control Information IE.For indicating the APN level overload, the PGW shall include one or more instances of this IE, up to maximum of 10, with the same type and instance value, representing a list of APN(s) (sharing the same Overload Reduction Metric and Period of Validity). See NOTE 1."})
group_list["Overload Control Information"] = { "index" : "280", "type" : "180", "ies" : added_ies }
added_ies = group_list["Bearer Context"]["ies"]
added_ies.append({ "ie_type" : "PCO", "ie_value" : "Protocol Configuration Options", "presence" : "O", "instance" : "0", "comment" : "This IE may be sent on the S5/S8 and S4/S11 interfaces if ePCO is not supported by the UE or the network. This bearer level IE takes precedence over the PCO IE in the message body if they both exist."})
added_ies.append({ "ie_type" : "ePCO", "ie_value" : "Extended Protocol Configuration Options", "presence" : "O", "instance" : "0", "comment" : "This IE may be sent on the S5/S8 and S11 interfaces if the UE and the network support ePCO. "})
added_ies.append({ "ie_type" : "Maximum Packet Loss Rate", "ie_value" : "Maximum Packet Loss Rate", "presence" : "O", "instance" : "0", "comment" : "This IE may be included on the S5/S8 interfaces if the PGW needs to send Maximum Packet Loss Rate as specified in clause 5.4.1 of 3GPP TS 23.401 [3]. This IE is only applicable for QCI 1. "})
group_list["Bearer Context"] = { "index" : "193", "type" : "93", "ies" : added_ies }
added_ies = group_list["Load Control Information"]["ies"]
added_ies = group_list["Overload Control Information"]["ies"]
added_ies = group_list["Bearer Context"]["ies"]
type_list["F-TEID"]["max_instance"] = "8"
added_ies.append({ "ie_type" : "F-TEID", "ie_value" : "S2b-U ePDG F-TEID", "presence" : "C", "instance" : "8", "comment" : "This IE shall be sent on the S2b interface."})
type_list["F-TEID"]["max_instance"] = "9"
added_ies.append({ "ie_type" : "F-TEID", "ie_value" : "S2b-U PGW F-TEID", "presence" : "C", "instance" : "9", "comment" : "This IE shall be sent on the S2b interface. It shall be used to correlate the bearers with those in the Create Bearer Request."})
type_list["F-TEID"]["max_instance"] = "10"
added_ies.append({ "ie_type" : "F-TEID", "ie_value" : "S2a-U TWAN F-TEID", "presence" : "C", "instance" : "10", "comment" : "This IE shall be sent on the S2a interface."})
type_list["F-TEID"]["max_instance"] = "11"
added_ies.append({ "ie_type" : "F-TEID", "ie_value" : "S2a-U PGW F-TEID", "presence" : "C", "instance" : "11", "comment" : "This IE shall be sent on the S2a interface. It shall be used to correlate the bearers with those in the Create Bearer Request."})
added_ies.append({ "ie_type" : "RAN/NAS Cause", "ie_value" : "RAN/NAS Cause", "presence" : "CO", "instance" : "0", "comment" : "If the bearer creation failed, the MME shall include this IE on the S11 interface to indicate the RAN cause and/or the NAS cause of the bearer creation failure, if available and if this information is permitted to be sent to the PGW operator according to MME operators policy. If both a RAN cause and a NAS cause are generated, then several IEs with the same type and instance value shall be included to represent a list of causes.The SGW shall include this IE on the S5/S8 interface if it receives it from the MME."})
group_list["Bearer Context"] = { "index" : "193", "type" : "93", "ies" : added_ies }
added_ies = group_list["Overload Control Information"]["ies"]
added_ies = group_list["Overload Control Information"]["ies"]
added_ies = group_list["Overload Control Information"]["ies"]
added_ies = group_list["Bearer Context"]["ies"]
added_ies = group_list["Bearer Context"]["ies"]
added_ies = group_list["Overload Control Information"]["ies"]
added_ies = group_list["Bearer Context"]["ies"]
added_ies = group_list["Bearer Context"]["ies"]
added_ies = group_list["Load Control Information"]["ies"]
added_ies = group_list["Overload Control Information"]["ies"]
added_ies = group_list["Overload Control Information"]["ies"]
added_ies = group_list["Bearer Context"]["ies"]
added_ies = group_list["Load Control Information"]["ies"]
added_ies = group_list["Overload Control Information"]["ies"]
added_ies = group_list["Load Control Information"]["ies"]
added_ies = group_list["Overload Control Information"]["ies"]
added_ies = group_list["Bearer Context"]["ies"]
added_ies = group_list["Overload Control Information"]["ies"]
added_ies = group_list["Load Control Information"]["ies"]
added_ies = group_list["Overload Control Information"]["ies"]
added_ies = group_list["Bearer Context"]["ies"]
added_ies = group_list["Overload Control Information"]["ies"]
added_ies = group_list["Overload Control Information"]["ies"]
added_ies = group_list["Bearer Context"]["ies"]
added_ies.append({ "ie_type" : "APCO", "ie_value" : "Additional Protocol Configuration Options", "presence" : "CO", "instance" : "0", "comment" : "The PGW shall include the Additional Prococol Configuration Options (APCO) IE on the S2b interface, including the list of available P-CSCF addresses, as part of the P-CSCF restoration extension procedure for the untrusted WLAN access, as specified in 3GPP TS 23.380 [61]."})
group_list["Bearer Context"] = { "index" : "193", "type" : "93", "ies" : added_ies }
added_ies = group_list["Load Control Information"]["ies"]
added_ies = group_list["Overload Control Information"]["ies"]
added_ies = group_list["Bearer Context"]["ies"]
added_ies = group_list["Overload Control Information"]["ies"]
added_ies = group_list["Bearer Context"]["ies"]
added_ies = group_list["Overload Control Information"]["ies"]
added_ies = group_list["Bearer Context"]["ies"]
added_ies = group_list["Overload Control Information"]["ies"]
added_ies = group_list["Bearer Context"]["ies"]
added_ies = group_list["Bearer Context"]["ies"]
added_ies = group_list["Load Control Information"]["ies"]
added_ies = group_list["Overload Control Information"]["ies"]
added_ies = group_list["Bearer Context"]["ies"]
added_ies = group_list["Bearer Context"]["ies"]
added_ies = group_list["Bearer Context"]["ies"]
added_ies = group_list["Bearer Context"]["ies"]
added_ies = group_list["Load Control Information"]["ies"]
added_ies = group_list["Overload Control Information"]["ies"]
added_ies = group_list["Remote UE Context"]["ies"]
added_ies = group_list["Remote UE Context"]["ies"]
ies = []
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
group_list["PDN Connection"] = { "index" : "209", "type" : "109", "ies" : ies }
added_ies = group_list["Bearer Context"]["ies"]
added_ies.append({ "ie_type" : "F-Container", "ie_value" : "BSS Container", "presence" : "CO", "instance" : "0", "comment" : "The MME/S4 SGSN shall include the Packet Flow ID, Radio Priority, SAPI, PS Handover XID parameters in the TAU/RAU/Handover procedure, if available. See Figure 8.48-2. The Container Type shall be set to 2."})
added_ies.append({ "ie_type" : "TI", "ie_value" : "Transaction Identifier", "presence" : "C", "instance" : "0", "comment" : "This IE shall be sent over S3/S10/S16 if the UE supports A/Gb and/or Iu mode."})
group_list["Bearer Context"] = { "index" : "193", "type" : "93", "ies" : added_ies }
added_ies = group_list["Remote UE Context"]["ies"]
ies = []
ies.append({ "ie_type" : "APN", "ie_value" : "APN", "presence" : "M", "instance" : "0", "comment" : ""})
ies.append({ "ie_type" : "EBI", "ie_value" : "Default EPS Bearer ID", "presence" : "M", "instance" : "0", "comment" : "This IE shall identify the default bearer of the SCEF PDN Connection."})
ies.append({ "ie_type" : "Node Identifier", "ie_value" : "SCEF ID", "presence" : "M", "instance" : "0", "comment" : "This IE shall include the SCEF Identifier and the SCEF Realm for the APN."})
group_list["SCEF PDN Connection"] = { "index" : "295", "type" : "195", "ies" : ies }
ies = []
ies.append({ "ie_type" : "Services Authorized", "ie_value" : "LTE V2X Services Authorized", "presence" : "C", "instance" : "0", "comment" : "This IE shall be included to indicate the authorization status of the UE to use the LTE sidelink for V2X services."})
type_list["Services Authorized"]["max_instance"] = "1"
ies.append({ "ie_type" : "Services Authorized", "ie_value" : "NR V2X Services Authorized", "presence" : "C", "instance" : "1", "comment" : "This IE shall be included to indicate the authorization status of the UE to use the NR sidelink for V2X services."})
ies.append({ "ie_type" : "Bit Rate", "ie_value" : "LTE UE Sidelink Aggregate Maximum Bit Rate", "presence" : "C", "instance" : "0", "comment" : "This IE shall be included if the UE is authorized for LTE V2X services."})
type_list["Bit Rate"]["max_instance"] = "1"
ies.append({ "ie_type" : "Bit Rate", "ie_value" : "NR UE Sidelink Aggregate Maximum Bit Rate", "presence" : "C", "instance" : "1", "comment" : "This IE shall be included if the UE is authorized for NR V2X services."})
ies.append({ "ie_type" : "PC5 QoS Parameters", "ie_value" : "PC5 QoS Parameters", "presence" : "C", "instance" : "0", "comment" : "This IE shall be included if the UE is authorized for NR V2X services."})
group_list["V2X Context"] = { "index" : "102", "type" : "2", "ies" : ies }
ies = []
ies.append({ "ie_type" : "PC5 QoS Flow", "ie_value" : "PC5 QoS Flows", "presence" : "M", "instance" : "0", "comment" : "Several IEs with this type and same instance value may be included as necessary to represent a list of PC5 QoS Flows."})
ies.append({ "ie_type" : "Bit Rate", "ie_value" : "PC5 Link Aggregated Bit Rates", "presence" : "O", "instance" : "0", "comment" : "This IE may be included for the non-GBR PC5 QoS Flows."})
group_list["PC5 QoS Parameters"] = { "index" : "105", "type" : "5", "ies" : ies }
added_ies = group_list["Bearer Context"]["ies"]
added_ies.append({ "ie_type" : "Packet Flow ID", "ie_value" : "Packet Flow ID", "presence" : "C", "instance" : "0", "comment" : "This IE shall be included if the message is used for PS handover and Inter RAT handover to/from A/Gb mode procedures."})
group_list["Bearer Context"] = { "index" : "193", "type" : "93", "ies" : added_ies }
added_ies = group_list["PDN Connection"]["ies"]
added_ies = group_list["Bearer Context"]["ies"]
added_ies = group_list["Remote UE Context"]["ies"]
added_ies = group_list["Bearer Context"]["ies"]
added_ies = group_list["Bearer Context"]["ies"]
added_ies = group_list["PDN Connection"]["ies"]
added_ies = group_list["Overload Control Information"]["ies"]
added_ies = group_list["Load Control Information"]["ies"]
added_ies = group_list["Bearer Context"]["ies"]
added_ies = group_list["PDN Connection"]["ies"]
added_ies = group_list["V2X Context"]["ies"]
added_ies = group_list["PC5 QoS Parameters"]["ies"]
