--Can be used in phpMyAdmin

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";

--
-- DIAMETER SERVER DATABASE
--

-- --------------------------------------------------------

--
-- Table structure for table `ds_data_types`
--

CREATE TABLE IF NOT EXISTS `ds_data_types` (
  `id_data_type` int(11) NOT NULL AUTO_INCREMENT,
  `label_type` varchar(255) NOT NULL,
  PRIMARY KEY (`id_data_type`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=2 ;

-- --------------------------------------------------------

--
-- Table structure for table `ds_networks`
--

CREATE TABLE IF NOT EXISTS `ds_networks` (
  `id_network` int(11) NOT NULL AUTO_INCREMENT,
  `label_network` varchar(255) NOT NULL,
  PRIMARY KEY (`id_network`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=3 ;

-- --------------------------------------------------------

--
-- Table structure for table `ds_sip_aor`
--

CREATE TABLE IF NOT EXISTS `ds_sip_aor` (
  `id_sip_aor` int(11) NOT NULL AUTO_INCREMENT,
  `id_user` int(11) NOT NULL,
  `sip_aor` varchar(255) NOT NULL,
  `sip_server_uri` varchar(255) DEFAULT NULL,
  `registered` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id_sip_aor`,`id_user`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=4 ;

-- --------------------------------------------------------

--
-- Table structure for table `ds_sip_services`
--

CREATE TABLE IF NOT EXISTS `ds_sip_services` (
  `id_service` int(11) NOT NULL AUTO_INCREMENT,
  `label_service` varchar(255) NOT NULL,
  PRIMARY KEY (`id_service`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=2 ;

-- --------------------------------------------------------

--
-- Table structure for table `ds_users`
--

CREATE TABLE IF NOT EXISTS `ds_users` (
  `id_user` int(11) NOT NULL AUTO_INCREMENT,
  `username` varchar(50) NOT NULL,
  `password` varchar(50) NOT NULL,
  `SIP_Server_URI` varchar(255) DEFAULT NULL,
  `temp_SIP_Server_URI` varchar(255) DEFAULT NULL,
  `authentication_pending` tinyint(1) NOT NULL DEFAULT '0',
  `registrated` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id_user`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=2 ;

-- --------------------------------------------------------

--
-- Table structure for table `ds_user_data`
--

CREATE TABLE IF NOT EXISTS `ds_user_data` (
  `id_user_data` int(11) NOT NULL AUTO_INCREMENT,
  `id_sip_aor` int(11) NOT NULL,
  `id_data_type` int(11) NOT NULL,
  `data` longblob NOT NULL,
  PRIMARY KEY (`id_user_data`,`id_sip_aor`,`id_data_type`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=3 ;

-- --------------------------------------------------------

--
-- Table structure for table `ds_user_networks`
--

CREATE TABLE IF NOT EXISTS `ds_user_networks` (
  `id_user_network` int(11) NOT NULL AUTO_INCREMENT,
  `id_user` int(11) NOT NULL,
  `id_network` int(11) NOT NULL,
  PRIMARY KEY (`id_user_network`,`id_user`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=3 ;

-- --------------------------------------------------------

--
-- Table structure for table `ds_user_services`
--

CREATE TABLE IF NOT EXISTS `ds_user_services` (
  `id_user_service` int(11) NOT NULL AUTO_INCREMENT,
  `id_user` int(11) NOT NULL,
  `id_service` int(11) NOT NULL,
  `compulsory` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id_user_service`,`id_user`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=6 ;

