<?php

# This file is an example wrapper around display_results.php.

# It relies on clients authentication based on certificate usage 
# (it must be the same certificate as used during AAA access, so that the User-Name matches).
# See your web server documentation for details.
# Example for apache2:
#  (+ detail in http://httpd.apache.org/docs/2.0/ssl/ssl_howto.html#allclients )
# - in vhost definition file, refence the CA chain of your users certificates:
#  SSLCACertificateFile /var/www/conf/ssl.crt/ca.crt
# - in vhost file or .htaccess file (adjust Depth to your setup):
#  <IfModule mod_ssl.c>
#  SSLVerifyClient require
#  SSLVerifyDepth 2
#  </IfModule>

/* Check the client is correctly SSL authenticated with his server */
if (!isset($_SERVER["SSL_CLIENT_VERIFY"]) || $_SERVER["SSL_CLIENT_VERIFY"] != "SUCCESS")
	die("SSL authentication failed, the webserver is probably not configured correctly.\n");
	
/* Force some parameters to integer values */
if ($_GET["t_limit"])
	$_GET["t_limit"] = (int) $_GET["t_limit"];
if ($_GET["t_offset"])
	$_GET["t_offset"] = (int) $_GET["t_offset"];
	
/* Default form values */
if (!isset($_GET["Submit"])) {
	$_GET["t_limit"] = 50;
	$_GET["c_limit"] = 1;
	$_GET["t_offset"] = 0;
}

/* Output the form */
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
<html lang="en-US">
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8">

    <title>Accounting Data</title>
    <meta name="description" content="This page gives you access to your personal accounting data.">
    <meta name="keywords" content="accounting">

    <style type="text/css">
        body { text-align:center; }
	table { border-collapse:collapse; margin-left:auto; margin-right:auto; }
	table, td, th { border:1px solid green; padding-left:.5em; padding-right:.5em;}
	th { background-color:green; color:white; }
    </style>
</head>

<body >
 <h1>Accounting data</h1>
  <p>Note well: this page displays only data about <em>terminated</em> sessions.</p>
  <form method="GET">
  <table>
    <tr>
      <th colspan="3">
        Filtering parameters
      </th>
    </tr>
    <tr>
      <td><input type="checkbox" name="c_starttime"<?php if (isset($_GET["c_starttime"])) echo " checked"; ?>></td>
      <td>Show only sessions starting from (<a href="http://www.postgresql.org/docs/8.4/static/datatype-datetime.html">YYYY-MM-DD HH:MM:SS</a>):</td>
      <td><input type="text" name="t_starttime"<?php if (isset($_GET["t_starttime"])) echo 'value="'.$_GET["t_starttime"].'"'; ?>></td>
    </tr>
    <tr>
      <td><input type="checkbox" name="c_endtime"<?php if (isset($_GET["c_endtime"])) echo " checked"; ?>></td>
      <td>Show only sessions starting until (<a href="http://www.postgresql.org/docs/8.4/static/datatype-datetime.html">YYYY-MM-DD HH:MM:SS</a>):</td>
      <td><input type="text" name="t_endtime"<?php if (isset($_GET["t_endtime"])) echo 'value="'.$_GET["t_endtime"].'"'; ?>></td>
    </tr>
    <tr>
      <td><input type="checkbox" name="c_limit"<?php if (isset($_GET["c_limit"])) echo " checked"; ?>></td>
      <td>Show only this number of records:</td>
      <td><input type="text" name="t_limit"<?php if (isset($_GET["t_limit"])) echo 'value="'.$_GET["t_limit"].'"'; ?>></td>
    </tr>
    <tr>
      <td><input type="checkbox" name="c_offset"<?php if (isset($_GET["c_offset"])) echo " checked"; ?>></td>
      <td>Starting from record:</td>
      <td><input type="text" name="t_offset"<?php if (isset($_GET["t_offset"])) echo 'value="'.$_GET["t_offset"].'"'; ?>></td>
    </tr>
    <tr>
      <th colspan="3">
        Apply this filter: <input type="submit" name="Submit">
      </th>
    </tr>
  </table>
  </form>
  
<p>
   Currently displaying user <em><?php echo htmlentities($_SERVER["SSL_CLIENT_S_DN_CN"]); ?></em><?php

/* Search user by CN or Email since some OS use the later during EAP-TLS authentication */
$USERS = array($_SERVER["SSL_CLIENT_S_DN_CN"], $_SERVER["SSL_CLIENT_S_DN_Email"]);

/* If the start time boundary was specified... */
if ($_GET["c_starttime"] && $_GET["t_starttime"]) {
	$START_TIME=$_GET["t_starttime"];
}
if ($_GET["c_endtime"] && $_GET["t_endtime"]) {
	$END_TIME=$_GET["t_endtime"];
}

/* idem with end time */
if ($START_TIME && $END_TIME) {
	echo ", sessions starting between $START_TIME and $END_TIME";
} elseif ($START_TIME) {
	echo ", sessions starting after $START_TIME";
} elseif ($END_TIME) {
	echo ", sessions starting before $END_TIME";
}

/* Pagination */
if ($_GET["c_limit"] && $_GET["t_limit"]) {
	$LIMIT=$_GET["t_limit"];
}
if ($_GET["c_offset"] && $_GET["t_offset"]) {
	$LIMIT_OFFSET=$_GET["t_offset"];
}
if ($LIMIT) {
	echo ", limited to ".$LIMIT." records";
	if ($LIMIT_OFFSET)
		echo " starting at ".$LIMIT_OFFSET;
} else if ($LIMIT_OFFSET) {
	echo " starting at record ".$LIMIT_OFFSET;
}
echo ".\n";
?>
</p>

<?php
/* This file will generate the array of data matching the selection */
require("display_results.php");

?>
</body>
</html>
