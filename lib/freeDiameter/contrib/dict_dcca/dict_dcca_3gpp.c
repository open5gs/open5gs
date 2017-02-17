/* 
 * Dictionary definitions of objects specified in DCCA (Nokia, 3GPP).
 */
#include <freeDiameter/extension.h>


/* The content of this file follows the same structure as dict_base_proto.c */

#define CHECK_dict_new( _type, _data, _parent, _ref )	\
	CHECK_FCT(  fd_dict_new( fd_g_config->cnf_dict, (_type), (_data), (_parent), (_ref))  );

#define CHECK_dict_search( _type, _criteria, _what, _result )	\
	CHECK_FCT(  fd_dict_search( fd_g_config->cnf_dict, (_type), (_criteria), (_what), (_result), ENOENT) );

struct local_rules_definition {
	char 			*avp_name;
	enum rule_position	position;
	int 			min;
	int			max;
};

#define RULE_ORDER( _position ) ((((_position) == RULE_FIXED_HEAD) || ((_position) == RULE_FIXED_TAIL)) ? 1 : 0 )

#define PARSE_loc_rules( _rulearray, _parent) {								\
	int __ar;											\
	for (__ar=0; __ar < sizeof(_rulearray) / sizeof((_rulearray)[0]); __ar++) {			\
		struct dict_rule_data __data = { NULL, 							\
			(_rulearray)[__ar].position,							\
			0, 										\
			(_rulearray)[__ar].min,								\
			(_rulearray)[__ar].max};							\
		__data.rule_order = RULE_ORDER(__data.rule_position);					\
		CHECK_FCT(  fd_dict_search( 								\
			fd_g_config->cnf_dict,								\
			DICT_AVP, 									\
			AVP_BY_NAME, 									\
			(_rulearray)[__ar].avp_name, 							\
			&__data.rule_avp, 0 ) );							\
		if ( !__data.rule_avp ) {								\
			TRACE_DEBUG(INFO, "AVP Not found: '%s'", (_rulearray)[__ar].avp_name );		\
			return ENOENT;									\
		}											\
		CHECK_FCT_DO( fd_dict_new( fd_g_config->cnf_dict, DICT_RULE, &__data, _parent, NULL),	\
			{							        		\
				TRACE_DEBUG(INFO, "Error on rule with AVP '%s'",      			\
					 (_rulearray)[__ar].avp_name );		      			\
				return EINVAL;					      			\
			} );							      			\
	}									      			\
}

#define enumval_def_u32( _val_, _str_ ) \
		{ _str_, 		{ .u32 = _val_ }}

#define enumval_def_os( _len_, _val_, _str_ ) \
		{ _str_, 		{ .os = { .data = (unsigned char *)_val_, .len = _len_ }}}


static int dict_dcca_3gpp_entry(char * conffile)
{
	TRACE_ENTRY("%p", conffile);		
	
	/* Applications section */
	{		
                /* Create the vendors */
                {
                        struct dict_vendor_data vendor_data = { 10415, "3GPP" };
                        CHECK_FCT(fd_dict_new(fd_g_config->cnf_dict, DICT_VENDOR, &vendor_data, NULL, NULL));
                }                                
  
	}
	
	
	/* AVP section */
        {
                struct dict_object * Address_type;
                struct dict_object * UTF8String_type;
                struct dict_object * DiameterIdentity_type;
                struct dict_object * DiameterURI_type;
                struct dict_object * Time_type;

                CHECK_dict_search( DICT_TYPE, TYPE_BY_NAME, "Address", &Address_type);
                CHECK_dict_search( DICT_TYPE, TYPE_BY_NAME, "UTF8String", &UTF8String_type);
                CHECK_dict_search( DICT_TYPE, TYPE_BY_NAME, "DiameterIdentity", &DiameterIdentity_type);
                CHECK_dict_search( DICT_TYPE, TYPE_BY_NAME, "DiameterURI", &DiameterURI_type);
                CHECK_dict_search( DICT_TYPE, TYPE_BY_NAME, "Time", &Time_type);                                

                /* Reporting-Reason */
                {
                  /* 
                     Enumerated. Will be present if quota was 
                     exhausted for one or more metering types. Only 
                     one Reporting-Reason AVP can be present in the 
                     Used-Service-Unit AVP. (Note that the Reporting-
                     Reason AVP may be present also at the MSCC 
                     level; see below.)
                     The following values are supported:

                     QUOTA_EXHAUSTED(3); Quota for the 
                     associated metering type has been 
                     exhausted. With this reporting reason there is 
                     only one metering type in this Used-Service-
                     Unit AVP.

                     OTHER_QUOTA_TYPE(5); Quota for one or 
                     more of the other metering types has been 
                     exhausted. With this reporting reason there 
                     may be multiple metering types in Used-
                     Service-Unit AVP.

                     POOL_EXHAUSTED(8); Quota from the 
                     credit pool has been exhausted. This reporting 
                     reason is used if the quota for the associated 
                     metering type was granted from a credit pool 
                     by using the GSU-Pool-Reference AVP in the 
                     CCA. With this reporting reason there is only 
                     one metering type in the Used-Service-Unit 
                     AVP.
                  */

                  struct dict_object              *type;
                  struct dict_type_data           tdata = { AVP_TYPE_INTEGER32,   "Enumerated(Reporting-Reason)" , NULL, NULL, NULL };
				  struct dict_enumval_data        t_1 = { "QHT",                      { .i32 = 1 }};
				  struct dict_enumval_data        t_2 = { "FINAL",                      { .i32 = 2 }};
                  struct dict_enumval_data        t_3 = { "QUOTA_EXHAUSTED",                      { .i32 = 3 }};
				  struct dict_enumval_data        t_4 = { "VALIDITY_TIME",                      { .i32 = 4 }};
                  struct dict_enumval_data        t_5 = { "OTHER_QUOTA_TYPE",        { .i32 = 5 }};
				  struct dict_enumval_data        t_6 = { "RATING_CONDITION_CHANGE",        { .i32 = 6 }};
				  struct dict_enumval_data        t_7 = { "FORCED_REAUTHORIZATION",        { .i32 = 7 }};
                  struct dict_enumval_data        t_8 = { "POOL_EXHAUSTED",                  { .i32 = 8 }};

                  struct dict_avp_data    data = { 
                    872,                                    /* Code */
                    10415,                                      /* Vendor */
                    "Reporting-Reason",                    /* Name */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,                     /* Fixed flag values */
                    AVP_TYPE_INTEGER32                      /* base type of data */
                  };
                  /* Create the Enumerated type, and then the AVP */
                  CHECK_dict_new( DICT_TYPE, &tdata , NULL, &type);
                  CHECK_dict_new( DICT_ENUMVAL, &t_3 , type, NULL);
                  CHECK_dict_new( DICT_ENUMVAL, &t_5 , type, NULL);
                  CHECK_dict_new( DICT_ENUMVAL, &t_8 , type, NULL);
                  CHECK_dict_new( DICT_AVP, &data , type, NULL);
                }               

                /* Trigger */
                {
                  /*
                    Grouped. The presence of the Trigger AVP in the 
                    CCR identifies the event(s) triggering the CCR.
                   */

                  struct dict_avp_data data = {
                    1264,                                    /* Code */
                    10415,                                      /* Vendor */
                    "Trigger",                  /* Name */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,                     /* Fixed flag values */
                    AVP_TYPE_GROUPED                        /* base type of data */
                  };
                  CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
                }

                /* Trigger-Type */
                {
                  /* 
                     Enumerated. One or more of these AVPs may be 
                     present. Indicates the event that triggered the 
                     MSCC. Supported values are:

                     CHANGE_IN_SGSN_IP_ADDRESS (1), a  change in the SGSN address.

                     CHANGEINQOS_ANY (2), a change in the QoS profile

                     CHANGEINRAT (4), a change in radio access technology

                   */

                  struct dict_object              *type;
                  struct dict_type_data           tdata = { AVP_TYPE_INTEGER32,   "Enumerated(Trigger-Type)" , NULL, NULL, NULL };
                  struct dict_enumval_data        t_1 = { "CHANGE_IN_SGSN_IP_ADDRESS",                      { .i32 = 1 }};
                  struct dict_enumval_data        t_2 = { "CHANGEINQOS_ANY",        { .i32 = 2 }};
                  struct dict_enumval_data        t_4 = { "CHANGEINRAT",                  { .i32 = 4 }};

                  struct dict_avp_data    data = { 
                    870,                                    /* Code */
                    10415,                                      /* Vendor */
                    "Trigger-Type",                    /* Name */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,                     /* Fixed flag values */
                    AVP_TYPE_INTEGER32                      /* base type of data */
                  };
                  /* Create the Enumerated type, and then the AVP */
                  CHECK_dict_new( DICT_TYPE, &tdata , NULL, &type);
                  CHECK_dict_new( DICT_ENUMVAL, &t_1 , type, NULL);
                  CHECK_dict_new( DICT_ENUMVAL, &t_2 , type, NULL);
                  CHECK_dict_new( DICT_ENUMVAL, &t_4 , type, NULL);
                  CHECK_dict_new( DICT_AVP, &data , type, NULL);
                }
                
                /* Service-Information */
                {
                  /* 
                     Grouped. The Service-Information purpose is to 
                     allow the transmission of additional 3GPP service 
                     specific information elements (3GPP 32.299 
                     Rel7).
                  */

                  struct dict_avp_data data = {
                    873,                                    /* Code */
                    10415,                                      /* Vendor */
                    "Service-Information",                  /* Name */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,                     /* Fixed flag values */
                    AVP_TYPE_GROUPED                        /* base type of data */
                  };
                  CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
                }

                /* PS-Information */
                {
                  /* 
                     Grouped. Its purpose is to allow the transmission 
                     of additional PS service specific information 
                     elements (3GPP 32.299 Rel7).
                  */

                  struct dict_avp_data data = {
                    874,                                    /* Code */
                    10415,                                      /* Vendor */
                    "PS-Information",                  /* Name */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,                     /* Fixed flag values */
                    AVP_TYPE_GROUPED                        /* base type of data */
                  };
                  CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
                }

                /* PDP-Address */
                {
                  /*
                    Address. Present only in the initial CCR. Contains 
                    the user equipment IP address. This AVP is 
                    defined in 3GPP 32.299 Rel7. 
                  */

                  struct dict_avp_data data = { 
                    1227,                                    /* Code */
                    10415,                                      /* Vendor */
                    "PDP-Address",                      /* Name */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,                     /* Fixed flag values */
                    AVP_TYPE_OCTETSTRING                    /* base type of data */
                  };
                  CHECK_dict_new( DICT_AVP, &data , Address_type, NULL);
                }

                /* 3GPP-IMSI */
                {
                  /*
		    IMSI encoded in UTF-8 per 3GPP TS 23.003. No
		    padding. Maximum length of data: 15.
                  */
                  struct dict_avp_data data = { 
                    1,                                    /* Code */
                    10415,                                      /* Vendor */
                    "3GPP-IMSI",                            /* Name */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,                     /* Fixed flag values */
                    AVP_TYPE_OCTETSTRING                      /* base type of data */
                  };
                  CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
                }

                /* 3GPP-Charging-Id */
                {
                  /*
                    Unsigned32. (3GPP TS 29.061 Rel7) . The 
                    charging identifier for the PDP context. The Flexi 
                    ISN generates the 3GPP charging ID for both 
                    virtual and normal PDP contexts with one excep-
                    tion. If the Flexi ISN acts as a NAS server and the 
                    charging ID selection is set to NAS Client, the 
                    charging ID will be the NAand not the 3GPP charging ID of Flexi ISN. 
                    Present in the initial CCR only.S client
                  */
                  struct dict_avp_data data = { 
                    2,                                    /* Code */
                    10415,                                      /* Vendor */
                    "3GPP-Charging-Id",                      /* Name */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,                     /* Fixed flag values */
                    AVP_TYPE_UNSIGNED32                     /* base type of data */
                  };
                  CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
                }

                /* 3GPP-PDP-Type */
                {
                  /*
                    Enumerated. (3GPP TS 29.061 Rel7 ). Type of 
                    PDP context, for example, IP or PPP. Present in 
                    the initial CCR only.
		    0 = IPv4
		    1 = PPP
		    2 = IPv6
		    3 = IPv4v6
                  */
                  struct dict_avp_data    data = { 
                    3,                                    /* Code */
                    10415,                                      /* Vendor */
                    "3GPP-PDP-Type",                    /* Name */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,                     /* Fixed flag values */
                    AVP_TYPE_UNSIGNED32                      /* base type of data */
                  };
                  CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
                }

                /* 3GPP-CG-Address */
                {
                  /*
                    OctetString. (3GPP TS 29.061 Rel4). The address 
                    of the charging gateway that has been marked as 
                    the default charging gateway for the PDP context. 
                    The address is expressed as a four-byte integer. 
                    Present in the initial CCR only.
                  */
                  struct dict_avp_data data = { 
                    4,                                    /* Code */
                    10415,                                      /* Vendor */
                    "3GPP-CG-Address",                      /* Name */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
                    AVP_FLAG_VENDOR,                     /* Fixed flag values */
                    AVP_TYPE_OCTETSTRING                     /* base type of data */
                  };
                  CHECK_dict_new( DICT_AVP, &data , Address_type, NULL);
                }

                /* 3GPP-GPRS-Negotiated-QoS-Profile */
                {
                  /* 
                     UTF8String. (3GPP TS 29.061 Rel7 ). The QoS 
                     profile applied by the Flexi ISN. In update and ter-
                     mination requests, this AVP is present only when 
                     the CCR has been triggered by a PDP context 
                     update affecting the negotiated QoS
		     Each octet is described by two UTF-8-encoded
                     characters denoting the hexadecimal
                     representation.
                  */
                  struct dict_avp_data data = { 
                    5,                                    /* Code */
                    10415,                                      /* Vendor */
                    "3GPP-GPRS-Negotiated-QoS-Profile",                    /* Name */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,                     /* Fixed flag values */
                    AVP_TYPE_OCTETSTRING                      /* base type of data */
                  };
                  CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
                }

                /* 3GPP-SGSN-Address */
                {
                  /*
                    OctetString. (3GPP TS 29.061 Rel4). The address 
                    of the charging gateway that has been marked as 
                    the default charging gateway for the PDP context. 
                    The address is expressed as a four-byte integer. 
                    Present in the initial CCR only.
                  */
                  struct dict_avp_data data = { 
                    6,                                    /* Code */
                    10415,                                      /* Vendor */
                    "3GPP-SGSN-Address",                      /* Name */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
                    AVP_FLAG_VENDOR,                     /* Fixed flag values */
                    AVP_TYPE_OCTETSTRING                     /* base type of data */
                  };
                  CHECK_dict_new( DICT_AVP, &data , Address_type, NULL);
                }               

                /* 3GPP-GGSN-Address */
                {
                  /*
                    OctetString. (3GPP TS 29.061 Rel4). Usually the 
                    IP address of Flexi ISN. The only exception is 
                    when the Flexi ISN acts as a NAS server and the 
                    charging ID selection is set to NAS Client; then the 
                    GGSN IP address will be the NAIP address. Present in the initial CCR only.S client
                  */
                  struct dict_avp_data data = { 
                    7,                                    /* Code */
                    10415,                                      /* Vendor */
                    "3GPP-GGSN-Address",                      /* Name */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
                    AVP_FLAG_VENDOR,                     /* Fixed flag values */
                    AVP_TYPE_OCTETSTRING                     /* base type of data */
                  };
                  CHECK_dict_new( DICT_AVP, &data , Address_type, NULL);
                }

                /* 3GPP-IMSI-MCC-MNC */
                {
                  /*
                    UTF8String. MCC and MNC extracted from the 
                    user's IMSI (first 5 or 6 digits, as applicable from 
                    the presented IMSI). The MCC-MNCs are 
                    extracted from the tables configured in FlexiISN 
                    configuration under the General and Roaming 
                    configurations.
                  */
                  struct dict_avp_data data = { 
                    8,                                    /* Code */
                    10415,                                      /* Vendor */
                    "3GPP-IMSI-MCC-MNC",                    /* Name */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,                     /* Fixed flag values */
                    AVP_TYPE_OCTETSTRING                      /* base type of data */
                  };
                  CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
                }

                /* 3GPP-GGSN-MCC-MNC */
                {
                  /* 
                     UTF8String. (3GPP TS 29.061 Rel7 ). Contains 
                     the mobile country and network code of the PLMN 
                     that the Flexi ISN belongs to. Present in the initial 
                     CCR only. The first entry in the list of local PLMNs 
                     in the Flexi ISN configuration determines the value 
                     of this AVP.
                  */
                  struct dict_avp_data data = { 
                    9,                                    /* Code */
                    10415,                                      /* Vendor */
                    "3GPP-GGSN-MCC-MNC",                    /* Name */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,                     /* Fixed flag values */
                    AVP_TYPE_OCTETSTRING                      /* base type of data */
                  };
                  CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
                }

                /* 3GPP-NSAPI */
                {
                  /* 
                     UTF8String. (3GPP TS 29.061 Rel7 ). Indicates 
                     the NSAPI of the PDP context. Contains one octet 
                     consisting of a single digit. Present in the initial 
                     CCR only.
                  */
                  struct dict_avp_data data = { 
                    10,                                    /* Code */
                    10415,                                      /* Vendor */
                    "3GPP-NSAPI",                    /* Name */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,                     /* Fixed flag values */
                    AVP_TYPE_OCTETSTRING                      /* base type of data */
                  };
                  CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
                }

                /* 3GPP-Session-Stop-Indicator */
                {
                  /*
                    OctetString. (3GPP TS 29.061 Rel4). The 
                    presence of this AVP indicates that the last 
                    context of the PDP session has been deleted. May 
                    be present in the termination CCR only. Contains 
                    one octet that has a value of 0xff.
                  */
                  struct dict_avp_data data = { 
                    11,                                    /* Code */
                    10415,                                      /* Vendor */
                    "3GPP-Session-Stop-Indicator",                      /* Name */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
                    AVP_FLAG_VENDOR,                     /* Fixed flag values */
                    AVP_TYPE_OCTETSTRING                     /* base type of data */
                  };
                  CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
                }

                /* 3GPP-Selection-Mode */
                {
                  /*
                    UTF8String. (3GPP TS 29.061 Rel7 ). Consists of 
                    one octet containing the selection mode as 
                    received from SGSN in the Create PDP Context 
                    Request. Present in the initial CCR only.
                  */
                  struct dict_avp_data data = { 
                    12,                                    /* Code */
                    10415,                                      /* Vendor */
                    "3GPP-Selection-Mode",                    /* Name */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,                     /* Fixed flag values */
                    AVP_TYPE_OCTETSTRING                      /* base type of data */
                  };
                  CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
                }

                /* 3GPP-Charging-Characteristics */
                {
                  /*
                    UTF8String. (3GPP TS 29.061 Rel7 ). The 
                    charging characteristics for the PDP context. 
                    Present in the initial CCR only. Consists of four 
                    octets. Each octet contains a single UTF-8 
                    encoded digit. The content of the charging charac-
                    teristics is described in 3GPP TS 32.215.
                  */
                  struct dict_avp_data data = { 
                    13,                                    /* Code */
                    10415,                                      /* Vendor */
                    "3GPP-Charging-Characteristics",                    /* Name */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,                     /* Fixed flag values */
                    AVP_TYPE_OCTETSTRING                      /* base type of data */
                  };
                  CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
                }

                /* 3GPP-CG-IPv6-Address */
                {
                  /*
                    UTF8String.. (3GPP TS 29.061 Rel? (<=10) ). The 
		    IPv6 address of the charging gateway.
                  */
                  struct dict_avp_data data = { 
                    14,                                    /* Code */
                    10415,                                      /* Vendor */
                    "3GPP-CG-IPv6-Address",                    /* Name */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,                     /* Fixed flag values */
                    AVP_TYPE_OCTETSTRING                      /* base type of data */
                  };
                  CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
                }

                /* 3GPP-SGSN-IPv6-Address */
                {
                  /*
                    UTF8String.. (3GPP TS 29.061 Rel? (<=10) ). The 
		    IPv6 address of the SGSN.
                  */
                  struct dict_avp_data data = { 
                    15,                                    /* Code */
                    10415,                                      /* Vendor */
                    "3GPP-SGSN-IPv6-Address",                    /* Name */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,                     /* Fixed flag values */
                    AVP_TYPE_OCTETSTRING                      /* base type of data */
                  };
                  CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
                }

                /* 3GPP-GGSN-IPv6-Address */
                {
                  /*
                    UTF8String.. (3GPP TS 29.061 Rel? (<=10) ). The 
		    IPv6 address of the GGSN.
                  */
                  struct dict_avp_data data = { 
                    16,                                    /* Code */
                    10415,                                      /* Vendor */
                    "3GPP-GGSN-IPv6-Address",                    /* Name */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,                     /* Fixed flag values */
                    AVP_TYPE_OCTETSTRING                      /* base type of data */
                  };
                  CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
                }

                /* 3GPP-GGSN-IPv6-DNS-Servers */
                {
                  /*
                    UTF8String.. (3GPP TS 29.061 Rel? (<=10) ). List
		    of IPv6 addresses of DNS servers for an APN in
		    order of preference (max. 15 servers, 16 bytes
		    each).
                  */
                  struct dict_avp_data data = { 
                    17,                                    /* Code */
                    10415,                                      /* Vendor */
                    "3GPP-GGSN-IPv6-DNS-Servers",                    /* Name */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,                     /* Fixed flag values */
                    AVP_TYPE_OCTETSTRING                      /* base type of data */
                  };
                  CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
                }

                /* 3GPP-SGSN-MCC-MNC */
                {
                  /* 
                     UTF8String. (3GPP TS 29.061 Rel7 ). MCC and 
                     MNC extracted from the RAI within Create or 
                     Update PDP Context Request. In update and ter-
                     mination requests, this AVP is present only when 
                     the CCR has been triggered by a routing area 
                     update.
                  */
                  struct dict_avp_data data = { 
                    18,                                    /* Code */
                    10415,                                      /* Vendor */
                    "3GPP-SGSN-MCC-MNC",                    /* Name */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,                     /* Fixed flag values */
                    AVP_TYPE_OCTETSTRING                      /* base type of data */
                  };
                  CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
                }

                /* Missing: 3GPP-Teardown-Indicator (19) */

                /* 3GPP-IMEISV */
                {
                  /*
		    IMEI(SV) encoded as sequence of UTF8 characters.
                  */
                  struct dict_avp_data data = { 
                    20,                                    /* Code */
                    10415,                                      /* Vendor */
                    "3GPP-IMEISV",                      /* Name */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,                     /* Fixed flag values */
                    AVP_TYPE_OCTETSTRING                     /* base type of data */
                  };
                  CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
                }

                /* 3GPP-RAT-Type */
                {
                  /* 
                     OctetString. (3GPP TS 29.061 Rel7 ) Defines the 
                     method used to access the network. Consists of a 
                     single octet. The following values may be sent:
                     UTRAN (1), GERAN (2), WLAN (3), NAS (254), 
                     Unspecified (255). 
                     This AVP is included in the initial CCR and in 
                     update or termination CCRs if the value changes.
                  */
                  struct dict_avp_data data = { 
                    21,                                    /* Code */
                    10415,                                      /* Vendor */
                    "3GPP-RAT-Type",                      /* Name */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,                     /* Fixed flag values */
                    AVP_TYPE_OCTETSTRING                     /* base type of data */
                  };
                  CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
                }

                /* 3GPP-User-Location-Info */
                {
                  /*
                    OctetString. (3GPP TS 29.061 Rel7 ) Contains 
                    information about the user's current geographical 
                    location as received from the SGSN. Present 
                    always in initial CCR if the value is known and in 
                    update and termination CCRs if the value 
                    changes.
                  */
                  struct dict_avp_data data = { 
                    22,                                    /* Code */
                    10415,                                      /* Vendor */
                    "3GPP-User-Location-Info",                      /* Name */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,                     /* Fixed flag values */
                    AVP_TYPE_OCTETSTRING                     /* base type of data */
                  };
                  CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
                }

                /* 3GPP-MS-TimeZone */
                {
                  /*
                    OctetString. (3GPP TS 29.061 Rel7 ) 3GPP-MS-
                    TimeZone is AVP is present always in the initial CCR if the 
                    value is known and in the update and termination 
                    CCRs if the value changes. This IE is encoded 
                    according to 3GPP TS 29.061 Rel7
                  */
                  struct dict_avp_data data = { 
                    23,                                    /* Code */
                    10415,                                      /* Vendor */
                    "3GPP-MS-TimeZone",                      /* Name */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,                     /* Fixed flag values */
                    AVP_TYPE_OCTETSTRING                     /* base type of data */
                  };
                  CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
                }

		/* Missing: 3GPP-CAMEL-Charging-Info (24) */
		/* Missing: 3GPP-Packet-Filter (25) */
		/* Missing: 3GPP-Negotiated-DSCP (26) */
		/* Missing: 3GPP-Allocate-IP-Type (27) */

                /* PDP-Context-Type */
                {
                  /* 
                     Enumerated. Indicates the type of a PDP context 
                     and is only included in CCR initial.

                     values are 0 for primary and 1 for secondary 
                     (3GPP 32.299 Rel7)
                  */
                  struct dict_object              *type;
                  struct dict_type_data           tdata = { AVP_TYPE_INTEGER32,   "Enumerated(PDP-Context-Type)" , NULL, NULL, NULL };
                  struct dict_enumval_data        t_0 = { "PRIMARY",                      { .i32 = 0 }};
                  struct dict_enumval_data        t_1 = { "SECONDARY",        { .i32 = 1 }};

                  struct dict_avp_data    data = { 
                    1247,                                    /* Code */
                    10415,                                      /* Vendor */
                    "PDP-Context-Type",                    /* Name */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,                     /* Fixed flag values */
                    AVP_TYPE_INTEGER32                      /* base type of data */
                  };
                  /* Create the Enumerated type, and then the AVP */
                  CHECK_dict_new( DICT_TYPE, &tdata , NULL, &type);
                  CHECK_dict_new( DICT_ENUMVAL, &t_0 , type, NULL);
                  CHECK_dict_new( DICT_ENUMVAL, &t_1 , type, NULL);
                  CHECK_dict_new( DICT_AVP, &data , type, NULL);
                }

                /* IMS-Information */
                {
                  /*
                    Grouped. Its purpose is to allow the transmission 
                    of additional IMS service specific information ele-
                    ments. (3GPP 32.299 Rel7)
                  */
                  struct dict_avp_data data = {
                    876,                                    /* Code */
                    10415,                                      /* Vendor */
                    "IMS-Information",                  /* Name */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,                     /* Fixed flag values */
                    AVP_TYPE_GROUPED                        /* base type of data */
                  };
                  CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
                }

                /* IMS-Charging-Identifier */
                {
                  /*
                    UTF8String. Contains the IMS charging identifier 
                    as given to the Flexi ISN by the IMS. This AVP is 
                    defined in 3GPP TS 32.225. This AVP is present 
                    in IMS sessions only. Present in the initial CCR 
                    only.
                  */
                  struct dict_avp_data data = { 
                    841,                                    /* Code */
                    10415,                                      /* Vendor */
                    "IMS-Charging-Identifier",                    /* Name */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,                     /* Fixed flag values */
                    AVP_TYPE_OCTETSTRING                      /* base type of data */
                  };
                  CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
                }
                

                

                /* Max-Requested-Bandwidth-UL */
                {
                  /*
                    Unsigned32. The Max-Requested-Bandwidth-
                    UL/DL AVP indicates the maximum allowed bit 
                    rate (in bits per second) for the uplink direction. 
                    (Re-used from 3GPP 29.214)
                  */
                  struct dict_avp_data data = { 
                    515,                                    /* Code */
                    10415,                                      /* Vendor */
                    "Max-Requested-Bandwidth-UL",                      /* Name */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,                     /* Fixed flag values */
                    AVP_TYPE_UNSIGNED32                     /* base type of data */
                  };
                  CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
                }

                /* Max-Requested-Bandwidth-DL */
                {
                  /*
                    Unsigned32. The Max-Requested-Bandwidth-
                    UL/DL AVP indicates the maximum allowed bit 
                    rate (in bits per second) for the uplink direction.
                  */
                  struct dict_avp_data data = { 
                    516,                                    /* Code */
                    10415,                                      /* Vendor */
                    "Max-Requested-Bandwidth-DL",                      /* Name */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,                     /* Fixed flag values */
                    AVP_TYPE_UNSIGNED32                     /* base type of data */
                  };
                  CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
                }

                /* QoS-Information */
                {
                  /*
                    Grouped. This is the QoS that the DCCA 
                    server requests from Flexi ISN to be applied for 
                    this PDP context. This AVP and all AVPs con-
                    tained within are described in 3GPP TS 29.212 
                    Release 7. The ARP AVP is introduced with 
                    Release 8. The relevant types are defined on 
                    table 5.3.1 of TS 29.212.
                  */

                  struct dict_avp_data data = {
                    1016,                                    /* Code */
                    10415,                                      /* Vendor */
                    "QoS-Information",                  /* Name */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
                    AVP_FLAG_VENDOR,                     /* Fixed flag values */
                    AVP_TYPE_GROUPED                        /* base type of data */
                  };
                  CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
                }                
        }


	
	TRACE_DEBUG(INFO, "Extension 'Dictionary definitions for DCCA 3GPP' initialized");
	return 0;
}

EXTENSION_ENTRY("dict_dcca_3gpp", dict_dcca_3gpp_entry, "dict_dcca");
