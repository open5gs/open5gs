#########################################################################
#	diameap.sql							#
#			DiamEAP extension				#
#  		 Users Information Database schema           		#
#  									#
#  									#
#									#
# MySQL command to load the script:					#
#    mysql> mysql -u username -p password diameap_ui <	diameap.sql	#
#									#
#########################################################################

-- --------------------------------------------------------

--
-- Table structure for table `authe`
--

CREATE TABLE IF NOT EXISTS `authe` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `grp` int(11) NOT NULL,
  `attribute` set('Authorization-Lifetime','Auth-Grace-Period','Auth-Session-State','Re-Auth-Request-Type','Session-Timeout','Multi-Round-Time-Out','Acct-Interim-Interval') NOT NULL DEFAULT 'Authorization-Lifetime',
  `value` char(255) NOT NULL,
  PRIMARY KEY (`id`)
);

-- --------------------------------------------------------

--
-- Table structure for table `authz`
--

CREATE TABLE IF NOT EXISTS `authz` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `grp` int(11) NOT NULL,
  `attribute` set('NAS-IPv6-Address','NAS-Identifier','NAS-IP-Address','NAS-Port','NAS-Port-Id','NAS-Port-Type','Called-Station-Id','Calling-Station-Id','Connect-Info','Originating-Line-Info','Service-Type','Callback-Number','Callback-Id','Idle-Timeout','Port-Limit','NAS-Filter-Rule','Filter-Id','Configuration-Token','QoS-Filter-Rule','Framed-Protocol','Framed-Routing','Framed-MTU','Framed-Compression','Framed-IP-Address','Framed-IP-Netmask','Framed-Route','Framed-Pool','Framed-Interface-Id','Framed-IPv6-Prefix','Framed-IPv6-Pool','Framed-IPv6-Route','Framed-IPX-Network','Framed-Appletalk-Link','Framed-Appletalk-Network','Framed-Appletalk-Zone') NOT NULL DEFAULT 'Service-Type',
  `op` set('==','>','>=','<','<=','!=','~=','=+','+==','+>','+>=','+<','+<=','+!=','+~=','==+','>+','>=+','<+','<=+','!=+') NOT NULL DEFAULT '==',
  `value` char(255) NOT NULL,
  PRIMARY KEY (`id`)
);

-- --------------------------------------------------------

--
-- Table structure for table `grp`
--

CREATE TABLE IF NOT EXISTS `grp` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `grp_name` char(255) NOT NULL,
  `active` set('Y','N') NOT NULL DEFAULT 'Y',
  PRIMARY KEY (`id`)
);

-- --------------------------------------------------------

--
-- Table structure for table `users`
--

CREATE TABLE IF NOT EXISTS `users` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `username` varchar(255) CHARACTER SET latin1 COLLATE latin1_general_ci NOT NULL,
  `eapmethod` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `vendor` int(11) NOT NULL DEFAULT '0',
  `password` varchar(255) CHARACTER SET latin1 COLLATE latin1_general_ci NOT NULL,
  `active` enum('Y','N') CHARACTER SET latin1 COLLATE latin1_general_ci NOT NULL DEFAULT 'Y',
  PRIMARY KEY (`id`)
);

-- --------------------------------------------------------

--
-- Table structure for table `user_grp`
--

CREATE TABLE IF NOT EXISTS `user_grp` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `user` int(11) NOT NULL,
  `grp` int(11) NOT NULL,
  PRIMARY KEY (`id`)
);
