/****************
 Contributed by: Konstantin Chekushin <koch@lmt.lv> and Thomas Klausner <tk@giga.or.at>
 License: same as freeDiameter
****************/


/* 
 * Dictionary definitions of objects specified in DCCA (rfc4006).
 */
#include <freeDiameter/extension.h>


/* The content of this file follows the same structure as dict_base_proto.c */

#define CHECK_dict_new( _type, _data, _parent, _ref )			\
    CHECK_FCT(  fd_dict_new( fd_g_config->cnf_dict, (_type), (_data), (_parent), (_ref))  );

#define CHECK_dict_search( _type, _criteria, _what, _result )		\
    CHECK_FCT(  fd_dict_search( fd_g_config->cnf_dict, (_type), (_criteria), (_what), (_result), ENOENT) );

struct local_rules_definition {
    char 			*avp_name;
    enum rule_position	position;
    int 			min;
    int			max;
};

#define RULE_ORDER( _position ) ((((_position) == RULE_FIXED_HEAD) || ((_position) == RULE_FIXED_TAIL)) ? 1 : 0 )

#define PARSE_loc_rules( _rulearray, _parent) {				\
	int __ar;							\
	for (__ar=0; __ar < sizeof(_rulearray) / sizeof((_rulearray)[0]); __ar++) { \
	    struct dict_rule_data __data = { NULL,			\
					     (_rulearray)[__ar].position, \
					     0,				\
					     (_rulearray)[__ar].min,	\
					     (_rulearray)[__ar].max};	\
	    __data.rule_order = RULE_ORDER(__data.rule_position);	\
	    CHECK_FCT(  fd_dict_search(					\
			    fd_g_config->cnf_dict,			\
			    DICT_AVP,					\
			    AVP_BY_NAME,				\
			    (_rulearray)[__ar].avp_name,		\
			    &__data.rule_avp, 0 ) );			\
	    if ( !__data.rule_avp ) {					\
		TRACE_DEBUG(INFO, "AVP Not found: '%s'", (_rulearray)[__ar].avp_name );	\
		return ENOENT;						\
	    }								\
	    CHECK_FCT_DO( fd_dict_new( fd_g_config->cnf_dict, DICT_RULE, &__data, _parent, NULL), \
			  {						\
			      TRACE_DEBUG(INFO, "Error on rule with AVP '%s'", \
					  (_rulearray)[__ar].avp_name ); \
			      return EINVAL;				\
			  } );						\
	}								\
    }

#define enumval_def_u32( _val_, _str_ )		\
    { _str_, 		{ .u32 = _val_ }}

#define enumval_def_os( _len_, _val_, _str_ )				\
    { _str_, 		{ .os = { .data = (unsigned char *)_val_, .len = _len_ }}}


static int dict_dcca_entry(char * conffile)
{
    struct dict_object * dcca;
    TRACE_ENTRY("%p", conffile);		
	
    /* Applications section */
    {
	/* DCCA */
	{
	    struct dict_application_data data = {        4, "Diameter Credit Control Application" 			};
	    CHECK_dict_new( DICT_APPLICATION, &data, NULL, &dcca);
	}                                
    }
	
    /* Result codes */
    {
        struct dict_object *ResultCodeType;
        CHECK_dict_search(DICT_TYPE, TYPE_BY_NAME, "Enumerated*(Result-Code)", &ResultCodeType);

        {
            struct dict_enumval_data error_code = {"END_USER_SERVICE_DENIED",
						   { .u32 = 4010}};
            CHECK_dict_new(DICT_ENUMVAL, &error_code, ResultCodeType, NULL);
        }
        {
            struct dict_enumval_data error_code = {"CREDIT_CONTROL_NOT_APPLICABLE",
						   { .u32 = 4011}};
            CHECK_dict_new(DICT_ENUMVAL, &error_code, ResultCodeType, NULL);
        }
        {
            struct dict_enumval_data error_code = {"CREDIT_LIMIT_REACHED",
						   { .u32 = 4012}};
            CHECK_dict_new(DICT_ENUMVAL, &error_code, ResultCodeType, NULL);
        }
        {
            struct dict_enumval_data error_code = {"USER_UNKNOWN",
						   { .u32 = 5030}};
            CHECK_dict_new(DICT_ENUMVAL, &error_code, ResultCodeType, NULL);
        }
        {
            struct dict_enumval_data error_code = {"RATING_FAILED",
						   { .u32 = 5031}};
            CHECK_dict_new(DICT_ENUMVAL, &error_code, ResultCodeType, NULL);
        }

    }


	
    /* AVP section */
    {
	struct dict_object * Address_type;
	struct dict_object * UTF8String_type;
	struct dict_object * DiameterIdentity_type;
	struct dict_object * DiameterURI_type;
	struct dict_object * Time_type;
	struct dict_object * IPFilterRule_type;

	CHECK_dict_search( DICT_TYPE, TYPE_BY_NAME, "Address", &Address_type);
	CHECK_dict_search( DICT_TYPE, TYPE_BY_NAME, "UTF8String", &UTF8String_type);
	CHECK_dict_search( DICT_TYPE, TYPE_BY_NAME, "DiameterIdentity", &DiameterIdentity_type);
	CHECK_dict_search( DICT_TYPE, TYPE_BY_NAME, "DiameterURI", &DiameterURI_type);
	CHECK_dict_search( DICT_TYPE, TYPE_BY_NAME, "Time", &Time_type);
	CHECK_dict_search( DICT_TYPE, TYPE_BY_NAME, "IPFilterRule", &IPFilterRule_type);
                
                
	/* CC-Correlation-Id */
	{
	    /* 
	       OctetString. 
	    */
	    struct dict_avp_data    data = { 
		411,                                    /* Code */
		0,                                      /* Vendor */
		"CC-Correlation-Id",                    /* Name */
		AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
		AVP_FLAG_MANDATORY,                     /* Fixed flag values */
		AVP_TYPE_OCTETSTRING                    /* base type of data */
	    };

	    CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
	}
                
	/* CC-Input-Octets */
	{
	    /* 
	       Unsigned64. 
	    */
	    struct dict_avp_data data = { 
		412,                                    /* Code */
		0,                                      /* Vendor */
		"CC-Input-Octets",                      /* Name */
		AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
		AVP_FLAG_MANDATORY,                     /* Fixed flag values */
		AVP_TYPE_UNSIGNED64                     /* base type of data */
	    };
	    CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
	}
                
	/* CC-Output-Octets */
	{
	    /* 
	       Unsigned64. 
	    */
	    struct dict_avp_data data = { 
		414,                                    /* Code */
		0,                                      /* Vendor */
		"CC-Output-Octets",                     /* Name */
		AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
		AVP_FLAG_MANDATORY,                     /* Fixed flag values */
		AVP_TYPE_UNSIGNED64                     /* base type of data */
	    };
	    CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
	}
                
	/* CC-Request-Number */
	{
	    /* 
	       Unsigned32. 
	    */
	    struct dict_avp_data data = { 
		415,                                    /* Code */
		0,                                      /* Vendor */
		"CC-Request-Number",                    /* Name */
		AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
		AVP_FLAG_MANDATORY,                     /* Fixed flag values */
		AVP_TYPE_UNSIGNED32                     /* base type of data */
	    };
	    CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
	}

	/* CC-Request-Type */
	{
	    /* 
	       Enumerated. 
	    */

	    struct dict_object              *type;
	    struct dict_type_data           tdata = { AVP_TYPE_INTEGER32, "Enumerated(CC-Request-Type)", NULL, NULL, NULL };
	    struct dict_enumval_data        t_1 = { "INITIAL_REQUEST", { .i32 = 1 }};
	    struct dict_enumval_data        t_2 = { "UPDATE_REQUEST", { .i32 = 2 }};
	    struct dict_enumval_data        t_3 = { "TERMINATION_REQUEST", { .i32 = 3 }};
	    struct dict_enumval_data        t_4 = { "EVENT_REQUEST", { .i32 = 4 }};
                  

	    struct dict_avp_data    data = { 
		416,                                    /* Code */
		0,                                      /* Vendor */
		"CC-Request-Type",                      /* Name */
		AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
		AVP_FLAG_MANDATORY,                     /* Fixed flag values */
		AVP_TYPE_INTEGER32                      /* base type of data */
	    };
	    /* Create the Enumerated type, and then the AVP */
	    CHECK_dict_new( DICT_TYPE, &tdata , NULL, &type);
	    CHECK_dict_new( DICT_ENUMVAL, &t_1 , type, NULL);
	    CHECK_dict_new( DICT_ENUMVAL, &t_2 , type, NULL);
	    CHECK_dict_new( DICT_ENUMVAL, &t_3 , type, NULL);
	    CHECK_dict_new( DICT_ENUMVAL, &t_4 , type, NULL);
	    CHECK_dict_new( DICT_AVP, &data , type, NULL);
	}
                

	/* CC-Service-Specific-Units */
	{
	    /* 
	       Unsigned64. 
	    */
	    struct dict_avp_data data = { 
		417,                                    /* Code */
		0,                                      /* Vendor */
		"CC-Service-Specific-Units",            /* Name */
		AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
		AVP_FLAG_MANDATORY,                     /* Fixed flag values */
		AVP_TYPE_UNSIGNED64                     /* base type of data */
	    };
	    CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
	}
   
	/* CC-Session-Failover */
	{
	    /* 
	       Enumerated. 
	    */

	    struct dict_object              *type;
	    struct dict_type_data           tdata = { AVP_TYPE_INTEGER32, "Enumerated(CC-Session-Failover)" , NULL, NULL, NULL };
	    struct dict_enumval_data        t_1 = { "FAILOVER_NOT_SUPPORTED", { .i32 = 0 }};
	    struct dict_enumval_data        t_2 = { "FAILOVER_SUPPORTED", { .i32 = 1 }};
                  

	    struct dict_avp_data    data = { 
		418,                                    /* Code */
		0,                                      /* Vendor */
		"CC-Session-Failover",                  /* Name */
		AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
		AVP_FLAG_MANDATORY,                     /* Fixed flag values */
		AVP_TYPE_INTEGER32                      /* base type of data */
	    };
	    /* Create the Enumerated type, and then the AVP */
	    CHECK_dict_new( DICT_TYPE, &tdata , NULL, &type);
	    CHECK_dict_new( DICT_ENUMVAL, &t_1 , type, NULL);                  
	    CHECK_dict_new( DICT_ENUMVAL, &t_2 , type, NULL);
	    CHECK_dict_new( DICT_AVP, &data , type, NULL);
	}
                
	/* CC-Sub-Session-Id */
	{
	    /* 
	       Unsigned64. 
	    */
	    struct dict_avp_data data = { 
		419,                                    /* Code */
		0,                                      /* Vendor */
		"CC-Sub-Session-Id",                    /* Name */
		AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
		AVP_FLAG_MANDATORY,                     /* Fixed flag values */
		AVP_TYPE_UNSIGNED64                     /* base type of data */
	    };
	    CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
	}
                
	/* CC-Time */
	{
	    /* 
	       Unsigned32. 
	    */
	    struct dict_avp_data data = { 
		420,                                    /* Code */
		0,                                      /* Vendor */
		"CC-Time",                              /* Name */
		AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
		AVP_FLAG_MANDATORY,                     /* Fixed flag values */
		AVP_TYPE_UNSIGNED32                     /* base type of data */
	    };
	    CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
	}
                                     
	/* CC-Total-Octets */
	{
	    /* 
	       Unsigned64. 
	    */
	    struct dict_avp_data data = { 
		421,                                    /* Code */
		0,                                      /* Vendor */
		"CC-Total-Octets",                      /* Name */
		AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
		AVP_FLAG_MANDATORY,                     /* Fixed flag values */
		AVP_TYPE_UNSIGNED64                     /* base type of data */
	    };
	    CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
	}
                
	/* CC-Unit-Type */
	{
	    /* 
	       Enumerated. 
	    */

	    struct dict_object              *type;
	    struct dict_type_data           tdata = { AVP_TYPE_INTEGER32, "Enumerated(CC-Unit-Type)" , NULL, NULL, NULL };
	    struct dict_enumval_data        t_1 = { "TIME", { .i32 = 0 }};
	    struct dict_enumval_data        t_2 = { "MONEY", { .i32 = 1 }};
	    struct dict_enumval_data        t_3 = { "TOTAL-OCTETS", { .i32 = 2 }};
	    struct dict_enumval_data        t_4 = { "INPUT-OCTETS", { .i32 = 3 }};
	    struct dict_enumval_data        t_5 = { "OUTPUT-OCTETS", { .i32 = 4 }};
	    struct dict_enumval_data        t_6 = { "SERVICE-SPECIFIC-UNITS", { .i32 = 5 }};
                  

	    struct dict_avp_data    data = { 
		454,                                    /* Code */
		0,                                      /* Vendor */
		"CC-Unit-Type",                         /* Name */
		AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
		AVP_FLAG_MANDATORY,                     /* Fixed flag values */
		AVP_TYPE_INTEGER32                      /* base type of data */
	    };
	    /* Create the Enumerated type, and then the AVP */
	    CHECK_dict_new( DICT_TYPE, &tdata , NULL, &type);
	    CHECK_dict_new( DICT_ENUMVAL, &t_1 , type, NULL);   
	    CHECK_dict_new( DICT_ENUMVAL, &t_2 , type, NULL); 
	    CHECK_dict_new( DICT_ENUMVAL, &t_3 , type, NULL); 
	    CHECK_dict_new( DICT_ENUMVAL, &t_4 , type, NULL); 
	    CHECK_dict_new( DICT_ENUMVAL, &t_5 , type, NULL); 
	    CHECK_dict_new( DICT_ENUMVAL, &t_6 , type, NULL); 
	    CHECK_dict_new( DICT_AVP, &data , type, NULL);
	}

	/* Check-Balance-Result */
	{
	    /* 
	       Enumerated. 
	    */

	    struct dict_object              *type;
	    struct dict_type_data           tdata = { AVP_TYPE_INTEGER32, "Enumerated(Check-Balance-Result)" , NULL, NULL, NULL };
	    struct dict_enumval_data        t_1 = { "ENOUGH_CREDIT", { .i32 = 0 }};
	    struct dict_enumval_data        t_2 = { "NO_CREDIT", { .i32 = 1 }};
                  

	    struct dict_avp_data    data = { 
		422,                                    /* Code */
		0,                                      /* Vendor */
		"Check-Balance-Result",                 /* Name */
		AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
		AVP_FLAG_MANDATORY,                     /* Fixed flag values */
		AVP_TYPE_INTEGER32                      /* base type of data */
	    };
	    /* Create the Enumerated type, and then the AVP */
	    CHECK_dict_new( DICT_TYPE, &tdata , NULL, &type);
	    CHECK_dict_new( DICT_ENUMVAL, &t_1 , type, NULL);  
	    CHECK_dict_new( DICT_ENUMVAL, &t_2 , type, NULL); 
	    CHECK_dict_new( DICT_AVP, &data , type, NULL);
	}
                
	/* Cost-Unit */
	{
	    /* 
	       UTF8String. 
	    */
	    struct dict_avp_data    data = { 
		424,                                    /* Code */
		0,                                      /* Vendor */
		"Cost-Unit",                            /* Name */
		AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
		AVP_FLAG_MANDATORY,                     /* Fixed flag values */
		AVP_TYPE_OCTETSTRING                    /* base type of data */
	    };
	    CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
	} 
                
	/* Credit-Control */
	{
	    /* 
	       Enumerated. 
	    */

	    struct dict_object              *type;
	    struct dict_type_data           tdata = { AVP_TYPE_INTEGER32, "Enumerated(Credit-Control)" , NULL, NULL, NULL };
	    struct dict_enumval_data        t_1 = { "CREDIT_AUTHORIZATION", { .i32 = 0 }};
	    struct dict_enumval_data        t_2 = { "RE_AUTHORIZATION", { .i32 = 1 }};
                  
	    struct dict_avp_data    data = { 
		426,                                    /* Code */
		0,                                      /* Vendor */
		"Credit-Control",                       /* Name */
		AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
		AVP_FLAG_MANDATORY,                     /* Fixed flag values */
		AVP_TYPE_INTEGER32                      /* base type of data */
	    };
	    /* Create the Enumerated type, and then the AVP */
	    CHECK_dict_new( DICT_TYPE, &tdata , NULL, &type);
	    CHECK_dict_new( DICT_ENUMVAL, &t_1, type, NULL); 
	    CHECK_dict_new( DICT_ENUMVAL, &t_2, type, NULL);
	    CHECK_dict_new( DICT_AVP, &data , type, NULL);
	}
   
	/* Credit-Control-Failure-Handling */
	{
	    /* 
	       Enumerated. 
	    */

	    struct dict_object              *type;
	    struct dict_type_data           tdata = { AVP_TYPE_INTEGER32, "Enumerated(Credit-Control-Failure-Handling)" , NULL, NULL, NULL };
	    struct dict_enumval_data        t_1 = { "TERMINATE", { .i32 = 0 }};
	    struct dict_enumval_data        t_2 = { "CONTINUE", { .i32 = 1 }};
	    struct dict_enumval_data        t_3 = { "RETRY_AND_TERMINATE", { .i32 = 2 }};
                  
	    struct dict_avp_data    data = { 
		427,                                    /* Code */
		0,                                      /* Vendor */
		"Credit-Control-Failure-Handling",      /* Name */
		AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
		AVP_FLAG_MANDATORY,                     /* Fixed flag values */
		AVP_TYPE_INTEGER32                      /* base type of data */
	    };
	    /* Create the Enumerated type, and then the AVP */
	    CHECK_dict_new( DICT_TYPE, &tdata , NULL, &type);
	    CHECK_dict_new( DICT_ENUMVAL, &t_1 , type, NULL);   
	    CHECK_dict_new( DICT_ENUMVAL, &t_2 , type, NULL);
	    CHECK_dict_new( DICT_ENUMVAL, &t_3 , type, NULL);
	    CHECK_dict_new( DICT_AVP, &data , type, NULL);
	}

	/* Currency-Code */
	{
	    /* 
	       Unsigned32. 
	    */
	    struct dict_avp_data data = { 
		425,                                    /* Code */
		0,                                      /* Vendor */
		"Currency-Code",                        /* Name */
		AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
		AVP_FLAG_MANDATORY,                     /* Fixed flag values */
		AVP_TYPE_UNSIGNED32                     /* base type of data */
	    };
	    CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
	}

	/* Direct-Debiting-Failure-Handling */
	{
	    /* 
	       Enumerated. 
	    */

	    struct dict_object              *type;
	    struct dict_type_data           tdata = { AVP_TYPE_INTEGER32, "Enumerated(Direct-Debiting-Failure-Handling)" , NULL, NULL, NULL };
	    struct dict_enumval_data        t_1 = { "TERMINATE_OR_BUFFER", { .i32 = 0 }};
	    struct dict_enumval_data        t_2 = { "CONTINUE", { .i32 = 1 }};
                  
	    struct dict_avp_data    data = { 
		428,                                    /* Code */
		0,                                      /* Vendor */
		"Direct-Debiting-Failure-Handling",     /* Name */
		AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
		AVP_FLAG_MANDATORY,                     /* Fixed flag values */
		AVP_TYPE_INTEGER32                      /* base type of data */
	    };
	    /* Create the Enumerated type, and then the AVP */
	    CHECK_dict_new( DICT_TYPE, &tdata , NULL, &type);
	    CHECK_dict_new( DICT_ENUMVAL, &t_1 , type, NULL); 
	    CHECK_dict_new( DICT_ENUMVAL, &t_2 , type, NULL);
	    CHECK_dict_new( DICT_AVP, &data , type, NULL);
	}

	/* Exponent */
	{
	    /* 
	       Integer32. 
	    */
	    struct dict_avp_data data = { 
		429,                                    /* Code */
		0,                                      /* Vendor */
		"Exponent",                             /* Name */
		AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
		AVP_FLAG_MANDATORY,                     /* Fixed flag values */
		AVP_TYPE_INTEGER32                      /* base type of data */
	    };
	    CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
	}
                
	/* Final-Unit-Action */
	{
	    /* 
	       Enumerated. 
	    */

	    struct dict_object              *type;
	    struct dict_type_data           tdata = { AVP_TYPE_INTEGER32, "Enumerated(Final-Unit-Action)" , NULL, NULL, NULL };
	    struct dict_enumval_data        t_1 = { "TERMINATE", { .i32 = 0 }};
	    struct dict_enumval_data        t_2 = { "REDIRECT", { .i32 = 1 }};
	    struct dict_enumval_data        t_3 = { "RESTRICT_ACCESS", { .i32 = 2 }};
                  
	    struct dict_avp_data    data = { 
		449,                                    /* Code */
		0,                                      /* Vendor */
		"Final-Unit-Action",                    /* Name */
		AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
		AVP_FLAG_MANDATORY,                     /* Fixed flag values */
		AVP_TYPE_INTEGER32                      /* base type of data */
	    };
	    /* Create the Enumerated type, and then the AVP */
	    CHECK_dict_new( DICT_TYPE, &tdata , NULL, &type);
	    CHECK_dict_new( DICT_ENUMVAL, &t_1 , type, NULL);  
	    CHECK_dict_new( DICT_ENUMVAL, &t_2 , type, NULL); 
	    CHECK_dict_new( DICT_ENUMVAL, &t_3 , type, NULL); 
	    CHECK_dict_new( DICT_AVP, &data , type, NULL);
	}

	/* G-S-U-Pool-Identifier */
	{
	    /* 
	       Unsigned32. 
	    */
	    struct dict_avp_data data = { 
		453,                                    /* Code */
		0,                                      /* Vendor */
		"G-S-U-Pool-Identifier",                /* Name */
		AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
		AVP_FLAG_MANDATORY,                     /* Fixed flag values */
		AVP_TYPE_UNSIGNED32                     /* base type of data */
	    };
	    CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
	}
                
	/* Multiple-Services-Indicator */
	{
	    /* 
	       Enumerated. 
	    */

	    struct dict_object              *type;
	    struct dict_type_data           tdata = { AVP_TYPE_INTEGER32, "Enumerated(Multiple-Services-Indicator)" , NULL, NULL, NULL };
	    struct dict_enumval_data        t_1 = { "MULTIPLE_SERVICES_NOT_SUPPORTED", { .i32 = 0 }};
	    struct dict_enumval_data        t_2 = { "MULTIPLE_SERVICES_SUPPORTED", { .i32 = 1 }};
                  
	    struct dict_avp_data    data = { 
		455,                                    /* Code */
		0,                                      /* Vendor */
		"Multiple-Services-Indicator",          /* Name */
		AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
		AVP_FLAG_MANDATORY,                     /* Fixed flag values */
		AVP_TYPE_INTEGER32                      /* base type of data */
	    };
	    /* Create the Enumerated type, and then the AVP */
	    CHECK_dict_new( DICT_TYPE, &tdata , NULL, &type);
	    CHECK_dict_new( DICT_ENUMVAL, &t_1 , type, NULL); 
	    CHECK_dict_new( DICT_ENUMVAL, &t_2 , type, NULL);
	    CHECK_dict_new( DICT_AVP, &data , type, NULL);
	}
                
	/* Rating-Group */
	{
	    /* 
	       Unsigned32. 
	    */
	    struct dict_avp_data data = { 
		432,                                    /* Code */
		0,                                      /* Vendor */
		"Rating-Group",                         /* Name */
		AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
		AVP_FLAG_MANDATORY,                     /* Fixed flag values */
		AVP_TYPE_UNSIGNED32                     /* base type of data */
	    };
	    CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
	}
                                      
	/* Redirect-Address-Type */
	{
	    /* 
	       Enumerated. 
	    */

	    struct dict_object              *type;
	    struct dict_type_data           tdata = { AVP_TYPE_INTEGER32, "Enumerated(Redirect-Address-Type)" , NULL, NULL, NULL };
	    struct dict_enumval_data        t_1 = { "IPV4_ADDRESS", { .i32 = 0 }};
	    struct dict_enumval_data        t_2 = { "IPV6_ADDRESS", { .i32 = 1 }};
	    struct dict_enumval_data        t_3 = { "URL", { .i32 = 2 }};
	    struct dict_enumval_data        t_4 = { "SIP_URI", { .i32 = 3 }};
                  
	    struct dict_avp_data    data = { 
		433,                                    /* Code */
		0,                                      /* Vendor */
		"Redirect-Address-Type",                /* Name */
		AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
		AVP_FLAG_MANDATORY,                     /* Fixed flag values */
		AVP_TYPE_INTEGER32                      /* base type of data */
	    };
	    /* Create the Enumerated type, and then the AVP */
	    CHECK_dict_new( DICT_TYPE, &tdata , NULL, &type);
	    CHECK_dict_new( DICT_ENUMVAL, &t_1 , type, NULL);  
	    CHECK_dict_new( DICT_ENUMVAL, &t_2 , type, NULL); 
	    CHECK_dict_new( DICT_ENUMVAL, &t_3 , type, NULL); 
	    CHECK_dict_new( DICT_ENUMVAL, &t_4 , type, NULL); 
	    CHECK_dict_new( DICT_AVP, &data , type, NULL);
	}           

	/* Redirect-Server-Address */
	{
	    /* 
	       UTF8String. 
	    */
	    struct dict_avp_data    data = { 
		435,                                    /* Code */
		0,                                      /* Vendor */
		"Redirect-Server-Address",              /* Name */
		AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
		AVP_FLAG_MANDATORY,                     /* Fixed flag values */
		AVP_TYPE_OCTETSTRING                    /* base type of data */
	    };
	    CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
	}
                
	/* Requested-Action */
	{
	    /* 
	       Enumerated. 
	    */

	    struct dict_object              *type;
	    struct dict_type_data           tdata = { AVP_TYPE_INTEGER32, "Enumerated(Requested-Action)" , NULL, NULL, NULL };
	    struct dict_enumval_data        t_1 = { "DIRECT_DEBITING", { .i32 = 0 }};
	    struct dict_enumval_data        t_2 = { "REFUND_ACCOUNT", { .i32 = 1 }};
	    struct dict_enumval_data        t_3 = { "CHECK_BALANCE", { .i32 = 2 }};
	    struct dict_enumval_data        t_4 = { "PRICE_ENQUIRY", { .i32 = 3 }};
                  
	    struct dict_avp_data    data = { 
		436,                                    /* Code */
		0,                                      /* Vendor */
		"Requested-Action",                     /* Name */
		AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
		AVP_FLAG_MANDATORY,                     /* Fixed flag values */
		AVP_TYPE_INTEGER32                      /* base type of data */
	    };
	    /* Create the Enumerated type, and then the AVP */
	    CHECK_dict_new( DICT_TYPE, &tdata , NULL, &type);
	    CHECK_dict_new( DICT_ENUMVAL, &t_1 , type, NULL); 
	    CHECK_dict_new( DICT_ENUMVAL, &t_2 , type, NULL);
	    CHECK_dict_new( DICT_ENUMVAL, &t_3 , type, NULL);
	    CHECK_dict_new( DICT_ENUMVAL, &t_4 , type, NULL);
	    CHECK_dict_new( DICT_AVP, &data , type, NULL);
	}
                                    
	/* Restriction-Filter-Rule */
	{
	    /* 
	       IPFiltrRule. 
	    */
	    struct dict_avp_data    data = { 
		438,                                    /* Code */
		0,                                      /* Vendor */
		"Restriction-Filter-Rule",              /* Name */
		AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
		AVP_FLAG_MANDATORY,                     /* Fixed flag values */
		AVP_TYPE_OCTETSTRING                    /* base type of data */
	    };
	    CHECK_dict_new( DICT_AVP, &data , IPFilterRule_type, NULL);
	}
	/*Service-Context-Id  */
	{
	    /* 
	       UTF8String. 
	    */
	    struct dict_avp_data    data = { 
		461,                                    /* Code */
		0,                                      /* Vendor */
		"Service-Context-Id",                   /* Name */
		AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
		AVP_FLAG_MANDATORY,                     /* Fixed flag values */
		AVP_TYPE_OCTETSTRING                    /* base type of data */
	    };
	    CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
	}

	/* Service-Identifier */
	{
	    /* 
	       Unsigned32. 
	    */
	    struct dict_avp_data data = { 
		439,                                    /* Code */
		0,                                      /* Vendor */
		"Service-Identifier",                   /* Name */
		AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
		AVP_FLAG_MANDATORY,                     /* Fixed flag values */
		AVP_TYPE_UNSIGNED32                     /* base type of data */
	    };
	    CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
	}                       
                               
	/* Service-Parameter-Type */
	{
	    /* 
	       Unsigned32. 
	    */
	    struct dict_avp_data data = { 
		441,                                    /* Code */
		0,                                      /* Vendor */
		"Service-Parameter-Type",               /* Name */
		AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
		AVP_FLAG_MANDATORY,                     /* Fixed flag values */
		AVP_TYPE_UNSIGNED32                     /* base type of data */
	    };
	    CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
	}
                
	/* Service-Parameter-Value */
	{
	    /* 
	       OctetString. 
	    */
	    struct dict_avp_data data = { 
		442,                                    /* Code */
		0,                                      /* Vendor */
		"Service-Parameter-Value",              /* Name */
		AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
		AVP_FLAG_MANDATORY,                     /* Fixed flag values */
		AVP_TYPE_OCTETSTRING                    /* base type of data */
	    };
	    CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
	}

	/* Subscription-Id-Data */
	{
	    /* 
	       UTF8String. 
	    */
	    struct dict_avp_data    data = { 
		444,                                    /* Code */
		0,                                      /* Vendor */
		"Subscription-Id-Data",                 /* Name */
		AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
		AVP_FLAG_MANDATORY,                     /* Fixed flag values */
		AVP_TYPE_OCTETSTRING                    /* base type of data */
	    };
	    CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
	}            
    
	/* Subscription-Id-Type */
	{
	    /* 
	       Enumerated. 
	    */

	    struct dict_object              *type;
	    struct dict_type_data           tdata = { AVP_TYPE_INTEGER32, "Enumerated(Subscription-Id-Type)" , NULL, NULL, NULL };
	    struct dict_enumval_data        t_1 = { "END_USER_E164", { .i32 = 0 }};
	    struct dict_enumval_data        t_2 = { "END_USER_IMSI", { .i32 = 1 }};
	    struct dict_enumval_data        t_3 = { "END_USER_SIP_URI", { .i32 = 2 }};
	    struct dict_enumval_data        t_4 = { "END_USER_NAI", { .i32 = 3 }};
                  
	    struct dict_avp_data    data = { 
		450,                                    /* Code */
		0,                                      /* Vendor */
		"Subscription-Id-Type",                 /* Name */
		AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
		AVP_FLAG_MANDATORY,                     /* Fixed flag values */
		AVP_TYPE_INTEGER32                      /* base type of data */
	    };
	    /* Create the Enumerated type, and then the AVP */
	    CHECK_dict_new( DICT_TYPE, &tdata , NULL, &type);
	    CHECK_dict_new( DICT_ENUMVAL, &t_1 , type, NULL); 
	    CHECK_dict_new( DICT_ENUMVAL, &t_2 , type, NULL);
	    CHECK_dict_new( DICT_ENUMVAL, &t_3 , type, NULL);
	    CHECK_dict_new( DICT_ENUMVAL, &t_4 , type, NULL);
	    CHECK_dict_new( DICT_AVP, &data , type, NULL);
	}
                
	/* Tariff-Change-Usage */
	{
	    /* 
	       Enumerated. 
	    */

	    struct dict_object              *type;
	    struct dict_type_data           tdata = { AVP_TYPE_INTEGER32, "Enumerated(Tariff-Change-Usage)" , NULL, NULL, NULL };
	    struct dict_enumval_data        t_1 = { "UNIT_BEFORE_TARIFF_CHANGE", { .i32 = 0 }};
	    struct dict_enumval_data        t_2 = { "UNIT_AFTER_TARIFF_CHANGE", { .i32 = 1 }};
	    struct dict_enumval_data        t_3 = { "UNIT_INDETERMINATE", { .i32 = 2 }};
                  
	    struct dict_avp_data    data = { 
		452,                                    /* Code */
		0,                                      /* Vendor */
		"Tariff-Change-Usage",                  /* Name */
		AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
		AVP_FLAG_MANDATORY,                     /* Fixed flag values */
		AVP_TYPE_INTEGER32                      /* base type of data */
	    };
	    /* Create the Enumerated type, and then the AVP */
	    CHECK_dict_new( DICT_TYPE, &tdata , NULL, &type);
	    CHECK_dict_new( DICT_ENUMVAL, &t_1 , type, NULL); 
	    CHECK_dict_new( DICT_ENUMVAL, &t_2 , type, NULL);
	    CHECK_dict_new( DICT_ENUMVAL, &t_3 , type, NULL);
	    CHECK_dict_new( DICT_AVP, &data , type, NULL);
	}
                
	/* Tariff-Time-Change */
	{
	    /*
	      Time. 
	    */
	    struct dict_avp_data data = { 
		451,                                    /* Code */
		0,                                      /* Vendor */
		"Tariff-Time-Change",                   /* Name */
		AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
		AVP_FLAG_MANDATORY,                     /* Fixed flag values */
		AVP_TYPE_OCTETSTRING                    /* base type of data */
	    };
	    CHECK_dict_new( DICT_AVP, &data , Time_type, NULL);
	}

	/* User-Equipment-Info-Type */
	{
	    /* 
	       Enumerated. 
	    */

	    struct dict_object              *type;
	    struct dict_type_data           tdata = { AVP_TYPE_INTEGER32, "Enumerated(User-Equipment-Info-Type)" , NULL, NULL, NULL };
	    struct dict_enumval_data        t_1 = { "IMEISV", { .i32 = 0 }};
	    struct dict_enumval_data        t_2 = { "MAC", { .i32 = 1 }};
	    struct dict_enumval_data        t_3 = { "EUI64", { .i32 = 2 }};
                  
	    struct dict_avp_data    data = { 
		459,                                    /* Code */
		0,                                      /* Vendor */
		"User-Equipment-Info-Type",             /* Name */
		AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
		AVP_FLAG_MANDATORY,                     /* Fixed flag values */
		AVP_TYPE_INTEGER32                      /* base type of data */
	    };
	    /* Create the Enumerated type, and then the AVP */
	    CHECK_dict_new( DICT_TYPE, &tdata , NULL, &type);
	    CHECK_dict_new( DICT_ENUMVAL, &t_1 , type, NULL);
	    CHECK_dict_new( DICT_ENUMVAL, &t_2 , type, NULL);
	    CHECK_dict_new( DICT_ENUMVAL, &t_3 , type, NULL);
	    CHECK_dict_new( DICT_AVP, &data , type, NULL);
	}                           
   
	/* User-Equipment-Info-Value */
	{
	    /* 
	       OctetString. 
	    */
	    struct dict_avp_data data = { 
		460,                                    /* Code */
		0,                                      /* Vendor */
		"User-Equipment-Info-Value",            /* Name */
		AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
		AVP_FLAG_MANDATORY,                     /* Fixed flag values */
		AVP_TYPE_OCTETSTRING                    /* base type of data */
	    };
	    CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
	}

	/* Value-Digits */
	{
	    /* 
	       Integer64. 
	    */
	    struct dict_avp_data data = { 
		447,                                    /* Code */
		0,                                      /* Vendor */
		"Value-Digits",                         /* Name */
		AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
		AVP_FLAG_MANDATORY,                     /* Fixed flag values */
		AVP_TYPE_INTEGER64                      /* base type of data */
	    };
	    CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
	}                         
                 
	/* Validity-Time */
	{
	    /* 
	       Unsigned32. 
	    */
	    struct dict_avp_data data = { 
		448,                                    /* Code */
		0,                                      /* Vendor */
		"Validity-Time",                        /* Name */
		AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
		AVP_FLAG_MANDATORY,                     /* Fixed flag values */
		AVP_TYPE_UNSIGNED32                     /* base type of data */
	    };
	    CHECK_dict_new( DICT_AVP, &data , NULL, NULL);
	}


	/* Grouped AVPs below since they have dependencies on types above */

	/* Redirect-Server */
	{
	    /*
	      Grouped
	    */
	    struct dict_object * avp;
	    struct dict_avp_data data = {
		434,                                    /* Code */
		0,                                      /* Vendor */
		"Redirect-Server",                      /* Name */
		AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
		AVP_FLAG_MANDATORY,                     /* Fixed flag values */
		AVP_TYPE_GROUPED                        /* base type of data */
	    };
	    struct local_rules_definition rules[] = {
		{  "Redirect-Address-Type",	RULE_REQUIRED,	-1, 1 },
		{  "Redirect-Server-Address",	RULE_REQUIRED,	-1, 1 }
	    };
	    CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
	    PARSE_loc_rules( rules, avp );
	}

	/* Service-Parameter-Info */
	{
	    /*
	      Grouped
	    */
	    struct dict_object * avp;
	    struct dict_avp_data data = {
		440,                                    /* Code */
		0,                                      /* Vendor */
		"Service-Parameter-Info",               /* Name */
		AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
		AVP_FLAG_MANDATORY,                     /* Fixed flag values */
		AVP_TYPE_GROUPED                        /* base type of data */
	    };
	    struct local_rules_definition rules[] = {
		{  "Service-Parameter-Type",	RULE_REQUIRED,	-1, 1 },
		{  "Service-Parameter-Value",	RULE_REQUIRED,	-1, 1 }
	    };
	    CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
	    PARSE_loc_rules( rules, avp );
	}                        

	/* Subscription-Id */
	{
	    /*
	      Grouped
	    */
	    struct dict_object * avp;
	    struct dict_avp_data data = {
		443,                                    /* Code */
		0,                                      /* Vendor */
		"Subscription-Id",                      /* Name */
		AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
		AVP_FLAG_MANDATORY,                     /* Fixed flag values */
		AVP_TYPE_GROUPED                        /* base type of data */
	    };
	    struct local_rules_definition rules[] = {
		{  "Subscription-Id-Type",	RULE_REQUIRED,	-1, 1 },
		{  "Subscription-Id-Data",	RULE_REQUIRED,	-1, 1 }
	    };
	    CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
	    PARSE_loc_rules( rules, avp );
	}
                                     
	/* Unit-Value */
	{
	    /*
	      Grouped
	    */
	    struct dict_object * avp;
	    struct dict_avp_data data = {
		445,                                    /* Code */
		0,                                      /* Vendor */
		"Unit-Value",                           /* Name */
		AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
		AVP_FLAG_MANDATORY,                     /* Fixed flag values */
		AVP_TYPE_GROUPED                        /* base type of data */
	    };
	    struct local_rules_definition rules[] = {
		{  "Value-Digits",	RULE_REQUIRED,	-1, 1 },
		{  "Exponent",		RULE_OPTIONAL,	-1, 1 }
	    };
	    CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
	    PARSE_loc_rules( rules, avp );
	}

	/* User-Equipment-Info */
	{
	    /*
	      Grouped
	    */
	    struct dict_object * avp;
	    struct dict_avp_data data = {
		458,                                    /* Code */
		0,                                      /* Vendor */
		"User-Equipment-Info",                  /* Name */
		AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
		AVP_FLAG_MANDATORY,                     /* Fixed flag values */
		AVP_TYPE_GROUPED                        /* base type of data */
	    };
	    struct local_rules_definition rules[] = {
		{  "User-Equipment-Info-Type",	RULE_REQUIRED,	-1, 1 },
		{  "User-Equipment-Info-Value",	RULE_REQUIRED,	-1, 1 }
	    };
	    CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
	    PARSE_loc_rules( rules, avp );
	}

	/* grouped AVPs using grouped AVPs */ 

	/* CC-Money */
	{
	    /*
	      Grouped
	    */
	    struct dict_object * avp;
	    struct dict_avp_data data = {
		413,                                    /* Code */
		0,                                      /* Vendor */
		"CC-Money",                             /* Name */
		AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
		AVP_FLAG_MANDATORY,                     /* Fixed flag values */
		AVP_TYPE_GROUPED                        /* base type of data */
	    };
	    struct local_rules_definition rules[] = {
		{  "Unit-Value",	RULE_REQUIRED,	-1, 1 },
		{  "Currency-Code",	RULE_OPTIONAL,	-1, 1 }
	    };
	    CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
	    PARSE_loc_rules( rules, avp );
	}

	/* Cost-Information */
	{
	    /*
	      Grouped
	    */
	    struct dict_object * avp;
	    struct dict_avp_data data = {
		423,                                    /* Code */
		0,                                      /* Vendor */
		"Cost-Information",                     /* Name */
		AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
		AVP_FLAG_MANDATORY,                     /* Fixed flag values */
		AVP_TYPE_GROUPED                        /* base type of data */
	    };
	    struct local_rules_definition rules[] = {
		{  "Unit-Value",	RULE_REQUIRED,	-1, 1 },
		{  "Currency-Code",	RULE_REQUIRED,	-1, 1 },
		{  "Cost-Unit",		RULE_OPTIONAL,	-1, 1 }
	    };
	    CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
	    PARSE_loc_rules( rules, avp );
	}

	/* Final-Unit-Indication */
	{
	    /*
	      Grouped
	    */
	    struct dict_object * avp;
	    struct dict_avp_data data = {
		430,                                    /* Code */
		0,                                      /* Vendor */
		"Final-Unit-Indication",                /* Name */
		AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
		AVP_FLAG_MANDATORY,                     /* Fixed flag values */
		AVP_TYPE_GROUPED                        /* base type of data */
	    };
	    struct local_rules_definition rules[] = {
		{  "Final-Unit-Action",		RULE_REQUIRED,	-1, 1 },
		{  "Restriction-Filter-Rule",	RULE_OPTIONAL,	-1, -1 },
		{  "Filter-Id",			RULE_OPTIONAL,	-1, -1 },
		{  "Redirect-Server",		RULE_OPTIONAL,	-1, 1 },
	    };
	    CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
	    PARSE_loc_rules( rules, avp );
	}
                
	/* Granted-Service-Unit */
	{
	    /*
	      Grouped
	    */
	    struct dict_object * avp;
	    struct dict_avp_data data = {
		431,                                    /* Code */
		0,                                      /* Vendor */
		"Granted-Service-Unit",                 /* Name */
		AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
		AVP_FLAG_MANDATORY,                     /* Fixed flag values */
		AVP_TYPE_GROUPED                        /* base type of data */
	    };
	    struct local_rules_definition rules[] = {
		{  "Tariff-Time-Change",	RULE_OPTIONAL,	-1, 1 },
		{  "CC-Time",	       		RULE_OPTIONAL,	-1, 1 },
		{  "CC-Money",			RULE_OPTIONAL,	-1, 1 },
		{  "CC-Total-Octets",		RULE_OPTIONAL,	-1, 1 },
		{  "CC-Input-Octets",		RULE_OPTIONAL,	-1, 1 },
		{  "CC-Output-Octets",		RULE_OPTIONAL,	-1, 1 },
		{  "CC-Service-Specific-Units",	RULE_OPTIONAL,	-1, 1 }
		/* plus any additional AVPs { "AVP", RULE_OPTIONAL, -1, -1 } */
	    };
	    CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
	    PARSE_loc_rules( rules, avp );
	}

	/* G-S-U-Pool-Reference */
	{
	    /*
	      Grouped
	    */
	    struct dict_object * avp;
	    struct dict_avp_data data = {
		457,                                    /* Code */
		0,                                      /* Vendor */
		"G-S-U-Pool-Reference",                 /* Name */
		AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
		AVP_FLAG_MANDATORY,                     /* Fixed flag values */
		AVP_TYPE_GROUPED                        /* base type of data */
	    };
	    struct local_rules_definition rules[] = {
		{  "G-S-U-Pool-Identifier",	RULE_REQUIRED,	-1, 1 },
		{  "CC-Unit-Type",		RULE_REQUIRED,	-1, 1 },
		{  "Unit-Value",		RULE_REQUIRED,	-1, 1 }
	    };
	    CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
	    PARSE_loc_rules( rules, avp );
	}                     

	/* Requested-Service-Unit */
	{
	    /*
	      Grouped
	    */
	    struct dict_object * avp;
	    struct dict_avp_data data = {
		437,                                    /* Code */
		0,                                      /* Vendor */
		"Requested-Service-Unit",               /* Name */
		AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
		AVP_FLAG_MANDATORY,                     /* Fixed flag values */
		AVP_TYPE_GROUPED                        /* base type of data */
	    };
	    struct local_rules_definition rules[] = {
		{  "CC-Time",	       		RULE_OPTIONAL,	-1, 1 },
		{  "CC-Money",			RULE_OPTIONAL,	-1, 1 },
		{  "CC-Total-Octets",		RULE_OPTIONAL,	-1, 1 },
		{  "CC-Input-Octets",		RULE_OPTIONAL,	-1, 1 },
		{  "CC-Output-Octets",		RULE_OPTIONAL,	-1, 1 },
		{  "CC-Service-Specific-Units",	RULE_OPTIONAL,	-1, 1 }
		/* plus any additional AVPs { "AVP", RULE_OPTIONAL, -1, -1 } */
	    };
	    CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
	    PARSE_loc_rules( rules, avp );
	}

	/* Used-Service-Unit */
	{
	    /*
	      Grouped
	    */
	    struct dict_object * avp;
	    struct dict_avp_data data = {
		446,                                    /* Code */
		0,                                      /* Vendor */
		"Used-Service-Unit",                    /* Name */
		AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
		AVP_FLAG_MANDATORY,                     /* Fixed flag values */
		AVP_TYPE_GROUPED                        /* base type of data */
	    };
	    struct local_rules_definition rules[] = {
		{  "Tariff-Change-Usage",	RULE_OPTIONAL,	-1, 1 },
		{  "CC-Time",	       		RULE_OPTIONAL,	-1, 1 },
		{  "CC-Money",			RULE_OPTIONAL,	-1, 1 },
		{  "CC-Total-Octets",		RULE_OPTIONAL,	-1, 1 },
		{  "CC-Input-Octets",		RULE_OPTIONAL,	-1, 1 },
		{  "CC-Output-Octets",		RULE_OPTIONAL,	-1, 1 },
		{  "CC-Service-Specific-Units",	RULE_OPTIONAL,	-1, 1 }
		/* plus any additional AVPs { "AVP", RULE_OPTIONAL, -1, -1 } */
	    };
	    CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
	    PARSE_loc_rules( rules, avp );
	}

	/* at least three levels of grouping */
	/* Multiple-Services-Credit-Control */
	{
	    /*
	      Grouped
	    */
	    struct dict_object * avp;
	    struct dict_avp_data data = {
		456,                                    /* Code */
		0,                                      /* Vendor */
		"Multiple-Services-Credit-Control",     /* Name */
		AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,   /* Fixed flags */
		AVP_FLAG_MANDATORY,                     /* Fixed flag values */
		AVP_TYPE_GROUPED                        /* base type of data */
	    };
	    struct local_rules_definition rules[] = {
		{  "Granted-Service-Unit",	RULE_OPTIONAL,	-1, 1 },
		{  "Requested-Service-Unit",	RULE_OPTIONAL,	-1, 1 },
		{  "Used-Service-Unit",		RULE_OPTIONAL,	-1, -1 },
		{  "Tariff-Change-Usage",	RULE_OPTIONAL,	-1, 1 },
		{  "Service-Identifier",	RULE_OPTIONAL,	-1, -1 },
		{  "Rating-Group",		RULE_OPTIONAL,	-1, 1 },
		{  "G-S-U-Pool-Reference",	RULE_OPTIONAL,	-1, -1 },
		{  "Validity-Time",		RULE_OPTIONAL,	-1, 1 },
		{  "Result-Code",		RULE_OPTIONAL,	-1, 1 },
		{  "Final-Unit-Indication",	RULE_OPTIONAL,	-1, 1 }
		/* plus any additional AVPs { "AVP", RULE_OPTIONAL, -1, -1 } */
	    };
	    CHECK_dict_new( DICT_AVP, &data , NULL, &avp);
	    PARSE_loc_rules( rules, avp );
	}
                

    }


    /* Commands section */
    {
	/* Credit-Control-Request (CCR) Command */
	{
	    /*
	      From RFC 4006:

	      3.1.  Credit-Control-Request (CCR) Command
		  
	      The Credit-Control-Request message (CCR) is indicated by the
	      command-code field being set to 272 and the 'R' bit being set in the
	      Command Flags field.  It is used between the Diameter credit-control
	      client and the credit-control server to request credit authorization
	      for a given service.
		  
	      The Auth-Application-Id MUST be set to the value 4, indicating the
	      Diameter credit-control application.
		  
	      Message Format
		  
	      <Credit-Control-Request> ::= < Diameter Header: 272, REQ, PXY >
			 	          < Session-Id >
			 	          { Origin-Host }
			 	          { Origin-Realm }
			 	          { Destination-Realm }
			 	          { Auth-Application-Id }
			 	          { Service-Context-Id }
			 	          { CC-Request-Type }
			 	          { CC-Request-Number }
			 	          [ Destination-Host ]
			 	          [ User-Name ]
			 	          [ CC-Sub-Session-Id ]
			 	          [ Acct-Multi-Session-Id ]
			 	          [ Origin-State-Id ]
			 	          [ Event-Timestamp ]
			 	         *[ Subscription-Id ]
			 	          [ Service-Identifier ]
			 	          [ Termination-Cause ]
			 	          [ Requested-Service-Unit ]
			 	          [ Requested-Action ]
			 	         *[ Used-Service-Unit ]
			 	          [ Multiple-Services-Indicator ]
			 	         *[ Multiple-Services-Credit-Control ]
			 	         *[ Service-Parameter-Info ]
			 	          [ CC-Correlation-Id ]
			 	          [ User-Equipment-Info ]
			 	         *[ Proxy-Info ]
			 	         *[ Route-Record ]
			 	         *[ AVP ]
			 	          		  
	      10.1.  Credit-Control AVP Table
		  
	      The table in this section is used to represent which credit-control
	      applications specific AVPs defined in this document are to be present
	      in the credit-control messages.
		  
	      +-----------+
	      |  Command  |
	      |   Code    |
	      |-----+-----+
	      Attribute Name                | CCR | CCA |
	      ------------------------------|-----+-----+
	      Acct-Multi-Session-Id         | 0-1 | 0-1 |
	      Auth-Application-Id           | 1   | 1   |
	      CC-Correlation-Id             | 0-1 | 0   |
	      CC-Session-Failover           | 0   | 0-1 |
	      CC-Request-Number             | 1   | 1   |
	      CC-Request-Type               | 1   | 1   |
	      CC-Sub-Session-Id             | 0-1 | 0-1 |
	      Check-Balance-Result          | 0   | 0-1 |
	      Cost-Information              | 0   | 0-1 |
	      Credit-Control-Failure-       | 0   | 0-1 |
	      Handling                      |     |     |
	      Destination-Host              | 0-1 | 0   |
	      Destination-Realm             | 1   | 0   |
	      Direct-Debiting-Failure-      | 0   | 0-1 |
	      Handling                      |     |     |
	      Event-Timestamp               | 0-1 | 0-1 |
	      Failed-AVP                    | 0   | 0+  |
	      Final-Unit-Indication         | 0   | 0-1 |
	      Granted-Service-Unit          | 0   | 0-1 |
	      Multiple-Services-Credit-     | 0+  | 0+  |
	      Control                       |     |     |
	      Multiple-Services-Indicator   | 0-1 | 0   |
	      Origin-Host                   | 1   | 1   |
	      Origin-Realm                  | 1   | 1   |
	      Origin-State-Id               | 0-1 | 0-1 |
	      Proxy-Info                    | 0+  | 0+  |
	      Redirect-Host                 | 0   | 0+  |
	      Redirect-Host-Usage           | 0   | 0-1 |
	      Redirect-Max-Cache-Time       | 0   | 0-1 |
	      Requested-Action              | 0-1 | 0   |
	      Requested-Service-Unit        | 0-1 | 0   |
	      Route-Record                  | 0+  | 0+  |
	      Result-Code                   | 0   | 1   |
	      Service-Context-Id            | 1   | 0   |
	      Service-Identifier            | 0-1 | 0   |
	      Service-Parameter-Info        | 0+  | 0   |
	      Session-Id                    | 1   | 1   |
	      Subscription-Id               | 0+  | 0   |
	      Termination-Cause             | 0-1 | 0   |
	      User-Equipment-Info           | 0-1 | 0   |
	      Used-Service-Unit             | 0+  | 0   |
	      User-Name                     | 0-1 | 0-1 |
	      Validity-Time                 | 0   | 0-1 |
	      ------------------------------|-----+-----+
		  

	    */
	    struct dict_object * cmd;
	    struct dict_cmd_data data = { 
		272, 					/* Code */
		"Credit-Control-Request", 		/* Name */
		CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE, 	/* Fixed flags */
		CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE 			/* Fixed flag values */
	    };
	    struct local_rules_definition rules[] = 
		{ 	 
		    { "Session-Id", RULE_FIXED_HEAD, -1, 1 },
		    { "Origin-Host", RULE_REQUIRED, -1, 1 },
		    { "Origin-Realm", RULE_REQUIRED, -1, 1 },
		    { "Destination-Realm", RULE_REQUIRED, -1, 1 },
		    { "Auth-Application-Id", RULE_REQUIRED, -1, 1 },
		    { "Service-Context-Id", RULE_REQUIRED, -1, 1 },
		    { "CC-Request-Type", RULE_REQUIRED, -1, 1 },
		    { "CC-Request-Number", RULE_REQUIRED, -1, 1 },
		    { "Destination-Host", RULE_OPTIONAL, -1, 1 },
		    { "User-Name", RULE_OPTIONAL, -1, 1 },
		    { "CC-Sub-Session-Id", RULE_OPTIONAL, -1, 1 },
		    { "Acct-Multi-Session-Id", RULE_OPTIONAL, -1, 1 },
		    { "Origin-State-Id", RULE_OPTIONAL, -1, 1 },
		    { "Event-Timestamp", RULE_OPTIONAL, -1, 1 },
		    { "Subscription-Id", RULE_OPTIONAL, -1, -1 },
		    { "Service-Identifier", RULE_OPTIONAL, -1, 1 },
		    { "Termination-Cause", RULE_OPTIONAL, -1, 1 },
		    { "Requested-Service-Unit", RULE_OPTIONAL, -1, 1 },
		    { "Requested-Action", RULE_OPTIONAL, -1, 1 },
		    { "Used-Service-Unit", RULE_OPTIONAL, -1, -1 },
		    { "Multiple-Services-Indicator", RULE_OPTIONAL, -1, 1 },
		    { "Multiple-Services-Credit-Control", RULE_OPTIONAL, -1, -1 },
		    { "Service-Parameter-Info", RULE_OPTIONAL, -1, -1 },
		    { "CC-Correlation-Id", RULE_OPTIONAL, -1, 1 },
		    { "User-Equipment-Info", RULE_OPTIONAL, -1, 1 },
		    { "Proxy-Info", RULE_OPTIONAL, -1, -1 },
		    { "Route-Record", RULE_OPTIONAL, -1, -1 }
		    /* plus any additional AVPs { "AVP", RULE_OPTIONAL, -1, -1 } */
		};

	    CHECK_dict_new( DICT_COMMAND, &data, dcca, &cmd);
	    PARSE_loc_rules( rules, cmd );
	}

	/* Credit-Control-Answer (CCA) Command */
	{
	    /*
	      From RFC 4006:
	      3.2.  Credit-Control-Answer (CCA) Command
		  
	      The Credit-Control-Answer message (CCA) is indicated by the command-
	      code field being set to 272 and the 'R' bit being cleared in the
	      Command Flags field.  It is used between the credit-control server
	      and the Diameter credit-control client to acknowledge a Credit-
	      Control-Request command.
		  
	      Message Format
		  
	      <Credit-Control-Answer> ::= < Diameter Header: 272, PXY >
			 	          < Session-Id >
			 	          { Result-Code }
			 	          { Origin-Host }
			 	          { Origin-Realm }
			 	          { Auth-Application-Id }
			 	          { CC-Request-Type }
			 	          { CC-Request-Number }
			 	          [ User-Name ]
			 	          [ CC-Session-Failover ]
			 	          [ CC-Sub-Session-Id ]
			 	          [ Acct-Multi-Session-Id ]
			 	          [ Origin-State-Id ]
			 	          [ Event-Timestamp ]
			 	          [ Granted-Service-Unit ]
			 	         *[ Multiple-Services-Credit-Control ]
			 	          [ Cost-Information]
			 	          [ Final-Unit-Indication ]
			 	          [ Check-Balance-Result ]
			 	          [ Credit-Control-Failure-Handling ]
			 	          [ Direct-Debiting-Failure-Handling ]
			 	          [ Validity-Time]
			 	         *[ Redirect-Host]
			 	          [ Redirect-Host-Usage ]
			 	          [ Redirect-Max-Cache-Time ]
			 	         *[ Proxy-Info ]
			 	         *[ Route-Record ]
			 	         *[ Failed-AVP ]
			 	         *[ AVP ]
			 	          		  
	      */
	    struct dict_object * cmd;
	    struct dict_cmd_data data = { 
		272, 					/* Code */
		"Credit-Control-Answer", 		/* Name */
		CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR,	/* Fixed flags */
		CMD_FLAG_PROXIABLE 			/* Fixed flag values */
	    };
	    struct local_rules_definition rules[] = 
		{ 	 
		    { "Session-Id", RULE_FIXED_HEAD, -1, 1 },
		    { "Result-Code", RULE_REQUIRED, -1, 1 },
		    { "Origin-Host", RULE_REQUIRED, -1, 1 },
		    { "Origin-Realm", RULE_REQUIRED, -1, 1 },
		    { "Auth-Application-Id", RULE_REQUIRED, -1, 1 },
		    { "CC-Request-Type", RULE_REQUIRED, -1, 1 },
		    { "CC-Request-Number", RULE_REQUIRED, -1, 1 },
		    { "User-Name", RULE_OPTIONAL, -1, 1 },
		    { "CC-Session-Failover", RULE_OPTIONAL, -1, 1 },
		    { "CC-Sub-Session-Id", RULE_OPTIONAL, -1, 1 },
		    { "Acct-Multi-Session-Id", RULE_OPTIONAL, -1, 1 },
		    { "Origin-State-Id", RULE_OPTIONAL, -1, 1 },
		    { "Event-Timestamp", RULE_OPTIONAL, -1, 1 },
		    { "Granted-Service-Unit", RULE_OPTIONAL, -1, 1 },
		    { "Multiple-Services-Credit-Control", RULE_OPTIONAL, -1, -1 },
		    { "Cost-Information", RULE_OPTIONAL, -1, 1 },
		    { "Final-Unit-Indication", RULE_OPTIONAL, -1, 1 },
		    { "Check-Balance-Result", RULE_OPTIONAL, -1, 1 },
		    { "Credit-Control-Failure-Handling", RULE_OPTIONAL, -1, 1 },
		    { "Direct-Debiting-Failure-Handling", RULE_OPTIONAL, -1, 1 },
		    { "Validity-Time", RULE_OPTIONAL, -1, 1 },
		    { "Redirect-Host", RULE_OPTIONAL, -1, -1 },
		    { "Redirect-Host-Usage", RULE_OPTIONAL, -1, 1 },
		    { "Redirect-Max-Cache-Time", RULE_OPTIONAL, -1, 1 },
		    { "Proxy-Info", RULE_OPTIONAL, -1, -1 },
		    { "Route-Record", RULE_OPTIONAL, -1, -1 },
		    { "Failed-AVP", RULE_OPTIONAL, -1, -1 }
		    /* plus any additional AVPs { "AVP", RULE_OPTIONAL, -1, -1 } */
		};

	    CHECK_dict_new( DICT_COMMAND, &data, dcca, &cmd);
	    PARSE_loc_rules( rules, cmd );
	}
    }
    LOG_D( "Extension 'Dictionary definitions for DCCA (rfc4006)' initialized");
    return 0;
}

/* needs dict_nasreq for Filter-Id */
EXTENSION_ENTRY("dict_dcca", dict_dcca_entry, "dict_nasreq");
