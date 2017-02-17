<?php

# The copyright of this file is the same as the freeDiameter project. Licence is BSD.

#------------------------------------------------------------------------------------------
# DATABASE:

/* The Connection String used to access that database:
  Example: "host=localhost dbname=app_acct user=freediameter password=foo" */
$CONNSTR=""; 

/* The name of the table containing the processed data (from process_records.php script) */
$PROCESSED="processed";

#------------------------------------------------------------------------------------------

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
	table.colored { border-collapse:collapse; margin-left:auto; margin-right:auto; }
	table.colored td, table.colored th { border:1px solid green; padding-left:.5em; padding-right:.5em;}
	.colored th { background-color:green; color:white; }
	hr { width:15px; margin-top:2em;margin-bottom:2em; }
    </style>
</head>

<body >
 <h1>Statistical accounting data</h1>
  <form method="GET">
  <table class="colored">
    <tr>
      <th colspan="3">
        Filtering options
      </th>
    </tr>
    <tr>
      <td><input type="checkbox" name="c_starttime"<?php if (isset($_GET["c_starttime"])) echo " checked"; ?>></td>
      <td>Limit to sessions starting from (<a href="http://www.postgresql.org/docs/8.4/static/datatype-datetime.html">hint</a>):</td>
      <td><input type="text" title="YYYY-MM-DD HH:MM:SS" name="t_starttime"<?php if (isset($_GET["t_starttime"])) echo 'value="'.$_GET["t_starttime"].'"'; ?>></td>
    </tr>
    <tr>
      <td><input type="checkbox" name="c_endtime"<?php if (isset($_GET["c_endtime"])) echo " checked"; ?>></td>
      <td>Limit to sessions starting until (<a href="http://www.postgresql.org/docs/8.4/static/datatype-datetime.html">hint</a>):</td>
      <td><input type="text" title="YYYY-MM-DD HH:MM:SS" name="t_endtime"<?php if (isset($_GET["t_endtime"])) echo 'value="'.$_GET["t_endtime"].'"'; ?>></td>
    </tr>
    <tr>
      <th colspan="3">
        Apply this filter: <input type="submit" name="Submit">
      </th>
    </tr>
  </table>
  </form>
  <p>Note well: this page displays only data about <em>terminated</em> sessions.</p>
<?php

/* Connect to the database */
$dbconn = pg_connect($CONNSTR)
    or die('Could not connect: ' . pg_last_error() . "\n");
    
if ($_GET["c_starttime"] && $_GET["t_starttime"]) {
	$START_TIME=$_GET["t_starttime"];
}
if ($_GET["c_endtime"] && $_GET["t_endtime"]) {
	$END_TIME=$_GET["t_endtime"];
}
$sql_cond="";
if ($START_TIME) {
	$sql_cond = "sess_start >= '".pg_escape_string($START_TIME)."'";
}
if ($END_TIME) {
	if ($sql_cond)
		$sql_cond .= " AND ";
	$sql_cond .= "sess_start =< '".pg_escape_string($END_TIME)."'";
}


/* Function to format download size (from php.net) */
function human_readable( $size )
{
   $count = 0;
   $format = array("B","KB","MB","GB","TB","PB","EB","ZB","YB");
   while(($size/1024)>1 && $count<8)
   {
       $size=$size/1024;
       $count++;
   }
   if( $size >= 100 ) $decimals = 0;
   elseif ($size >= 10 ) $decimals = 1;
   else  $decimals = 2;
   $return = number_format($size,$decimals,'.',' ')." ".$format[$count];
   return $return;
}

?>
  
  <hr />
  
<?php
    
/* First query: global data */
$sql = 'SELECT SUM(downl_bytes) as total_down, SUM(upl_bytes) as total_up,';
$sql .= ' to_char(min(sess_start), \'YYYY-MM-DD&nbsp;HH24:MI:SS&nbsp;TZ\') as first_sess,';
$sql .= ' to_char(max(sess_start), \'YYYY-MM-DD&nbsp;HH24:MI:SS&nbsp;TZ\') as last_sess,';
$sql .= ' count(distinct user_name) as nb_users,';
$sql .= ' count(distinct nas_info) as nb_ap';
$sql .= ' FROM "'.$PROCESSED.'"';
if ($sql_cond)
	$sql .= ' WHERE '.$sql_cond;

/* Execute the query */
$result = pg_query($dbconn, $sql) or die('Query failed: ' . pg_last_error() . "\n");
if (pg_num_rows($result) == 0) {
	echo "<p><em>No data is available in the selected period.</em></p>\n";
	die("</body></html>\n");
} 

$data = pg_fetch_array($result, null, PGSQL_ASSOC);
?>
  <table class="colored">
    <tr>
      <th colspan="2">Total accounted data</th>
    </tr>
<?php
echo "    <tr><td>First session started:</td><td>".$data["first_sess"]."</td></tr>\n";
echo "    <tr><td>Last session started:</td><td>".$data["last_sess"]."</td></tr>\n";
echo "    <tr><td>Total data downloaded:</td><td><strong>".human_readable($data["total_down"])."</strong></td></tr>\n";
echo "    <tr><td>Total data uploaded:</td><td><strong>".human_readable($data["total_up"])."</strong></td></tr>\n";
echo "    <tr><td>Number of users who connected at least once:</td><td>".$data["nb_users"]."</td></tr>\n";
echo "    <tr><td>Number of access points involved:</td><td>".$data["nb_ap"]."</td></tr>\n";
echo "  </tr>\n";
echo " </table>\n";

pg_free_result($result);

?>
  
  <hr />
  
  <table class="colored">
    <tr>
      <th colspan="2">Top-5 <strong>downloading</strong> users</th>
    </tr>
<?php
    
/* Now, get the top 5 downloaders */
$sql = 'SELECT SUM(downl_bytes) as total_down, user_name';
$sql .= ' FROM "'.$PROCESSED.'"';
if ($sql_cond)
	$sql .= ' WHERE '.$sql_cond;
$sql .= ' GROUP BY user_name';
$sql .= ' ORDER BY total_down desc, user_name';
$sql .= ' LIMIT 5';

/* Execute the query */
$result = pg_query($dbconn, $sql) or die('Query failed: ' . pg_last_error() . "\n");
while ($data = pg_fetch_array($result, null, PGSQL_ASSOC)) {
	echo "    <tr>\n";
	echo "      <td>".htmlentities(pg_unescape_bytea($data["user_name"]))."</td>\n";
	echo "      <td>".human_readable($data["total_down"])."</td>\n";
	echo "    </tr>\n";
}
pg_free_result($result);
?>
  </table>
  
  <hr />
  
  <table class="colored">
    <tr>
      <th colspan="2">Top-5 <strong>uploading</strong> users</th>
    </tr>
<?php
    
/* Now, get the top 5 downloaders */
$sql = 'SELECT SUM(upl_bytes) as total_up, user_name';
$sql .= ' FROM "'.$PROCESSED.'"';
if ($sql_cond)
	$sql .= ' WHERE '.$sql_cond;
$sql .= ' GROUP BY user_name';
$sql .= ' ORDER BY total_up desc, user_name';
$sql .= ' LIMIT 5';

/* Execute the query */
$result = pg_query($dbconn, $sql) or die('Query failed: ' . pg_last_error() . "\n");
while ($data = pg_fetch_array($result, null, PGSQL_ASSOC)) {
	echo "    <tr>\n";
	echo "      <td>".htmlentities(pg_unescape_bytea($data["user_name"]))."</td>\n";
	echo "      <td>".human_readable($data["total_up"])."</td>\n";
	echo "    </tr>\n";
}
pg_free_result($result);
?>
  </table>

  <hr />
  
  <table class="colored">
    <tr>
      <th colspan="2">Top-8 access devices (by number of users)</th>
    </tr>
<?php
    
/* Now, get the top 5 downloaders */
$sql = 'SELECT count(distinct user_name) as unc, nas_info';
$sql .= ' FROM "'.$PROCESSED.'"';
if ($sql_cond)
	$sql .= ' WHERE '.$sql_cond;
$sql .= ' GROUP BY nas_info';
$sql .= ' ORDER BY unc desc, nas_info';
$sql .= ' LIMIT 8';

/* Execute the query */
$result = pg_query($dbconn, $sql) or die('Query failed: ' . pg_last_error() . "\n");
while ($data = pg_fetch_array($result, null, PGSQL_ASSOC)) {
	echo "    <tr>\n";
	echo "      <td>".htmlentities(pg_unescape_bytea($data["nas_info"]))."</td>\n";
	echo "      <td>".$data["unc"]."</td>\n";
	echo "    </tr>\n";
}
pg_free_result($result);
?>
  </table>

  <hr />
  
  <table class="colored">
    <tr>
      <th colspan="2">Top-8 access devices (by traffic: up+down)</th>
    </tr>
<?php
    
/* Now, get the top 5 downloaders */
$sql = 'SELECT SUM(upl_bytes) + SUM(downl_bytes) as traffic, nas_info';
$sql .= ' FROM "'.$PROCESSED.'"';
if ($sql_cond)
	$sql .= ' WHERE '.$sql_cond;
$sql .= ' GROUP BY nas_info';
$sql .= ' ORDER BY traffic desc, nas_info';
$sql .= ' LIMIT 8';

/* Execute the query */
$result = pg_query($dbconn, $sql) or die('Query failed: ' . pg_last_error() . "\n");
while ($data = pg_fetch_array($result, null, PGSQL_ASSOC)) {
	echo "    <tr>\n";
	echo "      <td>".htmlentities(pg_unescape_bytea($data["nas_info"]))."</td>\n";
	echo "      <td>".human_readable($data["traffic"])."</td>\n";
	echo "    </tr>\n";
}
pg_free_result($result);
?>
  </table>

  <hr />
<?php
/* Closing connection */
pg_close($dbconn);
?>
<p><small>Data generated by scripts from the <a href="http://www.freediameter.net">freeDiameter</a> project.</small></p>
</body>
</html>
