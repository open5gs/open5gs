

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";

--
-- SUBSCRIBER LOCATOR DATABASE
--

-- --------------------------------------------------------

--
-- Table structure for table `ds_sip_aor_map`
--

CREATE TABLE IF NOT EXISTS `ds_sip_aor_map` (
  `id_map` int(11) NOT NULL AUTO_INCREMENT,
  `sip_aor` varchar(255) NOT NULL,
  `diameter_uri` varchar(255) NOT NULL,
  PRIMARY KEY (`id_map`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=2 ;

