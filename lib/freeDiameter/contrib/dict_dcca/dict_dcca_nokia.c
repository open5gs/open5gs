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


static int dict_dcca_nokia_entry(char * conffile)
{
	TRACE_ENTRY("%p", conffile);		
	
	/* Applications section */               
	{		
                /* Create the vendors */                                
                {
                        struct dict_vendor_data vendor_data = { 94, "Nokia" };
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

                /* NSN-Token-Value */
                {
                  /* 
                     OctetString. A token that was received, for 
                     example, from the subscription portal [14]. The 
                     maximum length of the token is 64 octets.
                     When this AVP is included in the MSCC, the USU 
                     and Reporting-Reason AVPs will not be included 
                     (because the token is not a trigger to report quota 
                     usage). Additionally, the Rating-Group and 
                     Service-Id AVPs will have the values received 
                     from the subscription portal.
                   */
                  struct dict_avp_data data = { 
                    5113,                                    /* Code */
                    94,                                      /* Vendor */
                    "NSN-Token-Value",                      /* Name */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,                     /* Fixed flag values */
                    AVP_TYPE_OCTETSTRING                     /* base type of data */
                  };
                  CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
                }
                
                /* Quota-Consumption-Time */
                {
                  /*
                    Unsigned32. Contains the time quota envelope 
                    expiration time in seconds. This is the time 
                    elapsed from the previous service request, to 
                    the moment the Flexi ISN considers that the 
                    time envelope finishes. The value of this AVP 
                    will be in effect for the remainder of the session 
                    or until a new value is received. If this AVP is 
                    not present, the default value configured for the 
                    rating group will be used. The OCS may 
                    disable the time quota envelope expiration by 
                    setting the value of this AVP to 0. The value of 
                    QCT sent from the OCS for a certain MSCC 
                    instance is stored in the Flexi ISN and it is used 
                    if the OCS does not sent a new value in the 
                    CCAs that follow
                  */
                  struct dict_avp_data data = { 
                    5109,                                    /* Code */
                    94,                                      /* Vendor */
                    "Quota-Consumption-Time",                      /* Name */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,                     /* Fixed flag values */
                    AVP_TYPE_UNSIGNED32                     /* base type of data */
                  };
                  CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
                }

                /* Quota-Holding-Time */
                {
                  /* 
                     Unsigned32. Indicates how long the Flexi ISN 
                     will keep the granted quota after a service 
                     request has been received. The unit is in 
                     seconds. When this time elapses, the Flexi ISN 
                     ends the MSCC instance and reports the used 
                     quota without requesting more. The value of 
                     this AVP will be in effect for the remainder of 
                     the session or until a new value is received. If 
                     this AVP is not present, the default value con-
                     figured for the rating group will be used. The 
                     OCS may disable the Quota Holding Timer by 
                     setting the value of this AVP to 0. The value of 
                     the QHT sent from the OCS for a certain 
                     MSCC instance is stored in the Flexi ISN, and 
                     it is used if the OCS does not sent a new value 
                     in the CCAs that follow.
                  */
                  struct dict_avp_data data = { 
                    5110,                                    /* Code */
                    94,                                      /* Vendor */
                    "Quota-Holding-Time",                      /* Name */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,                     /* Fixed flag values */
                    AVP_TYPE_UNSIGNED32                     /* base type of data */
                  };
                  CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
                }

                /* Default-Quota */
                {
                  /*
                    Grouped. Indicates the absolute number of 
                    units that are available for consumption during 
                    each upcoming OCS interrogation (e.g. after a 
                    CCR has been sent but before the CCA is 
                    received). If this AVP is not present, the Flexi 
                    ISN will use the configured or earlier received 
                    default quota. If this AVP is present but does 
                    not include the default quota for a certain unit 
                    type, that unit type is considered to have no 
                    default quota. 
                    The usage of default quota can be disabled in 
                    certain situations by modifying the Flexi ISN 
                    settings
                  */
                  struct dict_avp_data data = {
                    5111,                                    /* Code */
                    94,                                      /* Vendor */
                    "Default-Quota",                  /* Name */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,                     /* Fixed flag values */
                    AVP_TYPE_GROUPED                        /* base type of data */
                  };
                  CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
                }                

                /* Session-Start-Indicator */
                {
                  /*
                    OctetString. (3GPP TS 29.061 Rel4). The address 
                    of the charging gateway that has been marked as 
                    the default charging gateway for the PDP context. 
                    The address is expressed as a four-byte integer. 
                    Present in the initial CCR only.
                  */
                  struct dict_avp_data data = { 
                    5105,                                    /* Code */
                    94,                                      /* Vendor */
                    "Session-Start-Indicator",                      /* Name */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
                    AVP_FLAG_VENDOR,                     /* Fixed flag values */
                    AVP_TYPE_OCTETSTRING                     /* base type of data */
                  };
                  CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
                }                

                /* Rulebase-id */
                {
                  /*
                  */
                  struct dict_avp_data data = { 
                    5106,                                    /* Code */
                    94,                                      /* Vendor */
                    "Rulebase-id",                    /* Name */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
                    AVP_FLAG_VENDOR,                     /* Fixed flag values */
                    AVP_TYPE_OCTETSTRING                      /* base type of data */
                  };
                  CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
                }                

                /* Time-Of-First-Usage */
                {
                  /*
                    Time. Contains a time-stamp identifying the date 
                    and time of the first increment of the used units 
                    counter since the previous report. If no used units 
                    were gathered during the previous reporting inter-
                    val, this AVP will not be present.
                  */
                  struct dict_avp_data data = { 
                    5103,                                     /* Code */
                    94,                                      /* Vendor */
                    "Time-Of-First-Usage",                      /* Name */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
                    AVP_FLAG_VENDOR,                     /* Fixed flag values */
                    AVP_TYPE_OCTETSTRING                    /* base type of data */
                  };
                  CHECK_dict_new( DICT_AVP, &data , Time_type, NULL);
                }

                /* Time-Of-Last-Usage */
                {
                  /*
                    Time. Contains a time-stamp identifying the date 
                    and time of the first increment of the used units 
                    counter since the previous report. If no used units 
                    were gathered during the previous reporting inter-
                    val, this AVP will not be present.
                  */
                  struct dict_avp_data data = { 
                    5104,                                     /* Code */
                    94,                                      /* Vendor */
                    "Time-Of-Last-Usage",                      /* Name */
                    AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
                    AVP_FLAG_VENDOR,                     /* Fixed flag values */
                    AVP_TYPE_OCTETSTRING                    /* base type of data */
                  };
                  CHECK_dict_new( DICT_AVP, &data , Time_type, NULL);
                }
        }
	
	TRACE_DEBUG(INFO, "Extension 'Dictionary definitions for DCCA Nokia' initialized");
	return 0;
}

EXTENSION_ENTRY("dict_dcca_nokia", dict_dcca_nokia_entry, "dict_dcca");
