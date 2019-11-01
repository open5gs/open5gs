<?php

# The copyright of this file is the same as the freeDiameter project. Licence is BSD.

# This file should no be called directly; 
# instead it should be included from another script that sets its parameters as described below.

## $USERS  
# An array of the user names to display; the empty array will display all users.
# This parameter MUST be set.
if (!isset($USERS))
	die('Do not call this file directly');

## $START_TIME:
# If set, this restricts the displayed data to sessions starting after $START

## $END_TIME:
# If set, this restricts the displayed data to sessions starting before $END

## $LIMIT:
## $LIMIT_OFFSET:
# If set, these limit the number of accounting records displayed (for pagination purpose)

#------------------------------------------------------------------------------------------
# DATABASE:

/* The Connection String used to access that database:
  Example: "host=localhost dbname=app_acct user=freediameter password=foo" */
$CONNSTR=""; 

/* The name of the table containing the processed data (from process_records.php script) */
$PROCESSED="processed";

#------------------------------------------------------------------------------------------


/* Connect to the database */
$dbconn = pg_connect($CONNSTR)
    or die('Could not connect: ' . pg_last_error() . "\n");
    
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
    
/* Build the SQL query */
$sql = 'SELECT *, to_char(sess_start, \'YYYY-MM-DD&nbsp;HH24:MI:SS&nbsp;(TZ)\') as fmt_sess_start FROM "'.$PROCESSED.'"';
$where=0;
if ($USERS) {
	$USERS = array_map(pg_escape_bytea, $USERS);
	$sql .= " WHERE user_name IN ('". join("', '", array_values($USERS))."') ";
	$where = 1;
}

if ($START_TIME) {
	$START_TIME = pg_escape_string($START_TIME);
	if ($where++)
		$sql .= " AND ";
	else
		$sql .= " WHERE ";
	$sql .= "sess_start >= '".$START_TIME."'";
}
if ($END_TIME) {
	$END_TIME = pg_escape_string($END_TIME);
	if ($where++)
		$sql .= " AND ";
	else
		$sql .= " WHERE ";
	$sql .= "sess_start <= '".$END_TIME."'";
}

$sql .= " ORDER BY sess_start, sess_duration";

if ($LIMIT)
	$sql .= " LIMIT $LIMIT";
if ($LIMIT_OFFSET)
	$sql .= " OFFSET $LIMIT_OFFSET";

/* Execute the query */
$result = pg_query($dbconn, $sql) or die('Query failed: ' . pg_last_error() . "\n");
$recs = pg_num_rows($result);
if ($recs == 0) {
	echo "<p><em>Sorry, no data is available in this selection.</em></p>\n";
} else {
	echo "<p><strong>$recs</strong> records found.</p>\n";
?>
  <table>
    <tr>
      <th>Device identifier</th>
      <th>Access Device information</th>
      <th>Session started on</th>
      <th>Duration</th>
      <th>Downloaded</th>
      <th>Uploaded</th>
    </tr>
<?php
	while ($record = pg_fetch_array($result, null, PGSQL_ASSOC)) {
		echo "    <tr title='".htmlentities(pg_unescape_bytea($record["user_name"]))."'>\n";
		echo "      <td>";
		echo htmlentities(pg_unescape_bytea($record["user_device"]));
		echo "</td>\n";
		echo "      <td>";
		echo htmlentities(pg_unescape_bytea($record["nas_info"]));
		echo "</td>\n";
		echo "      <td>";
		echo $record["fmt_sess_start"];
		echo "</td>\n";
		echo "      <td>";
		echo htmlentities($record["sess_duration"]);
		echo "</td>\n";
		echo "      <td>";
		echo human_readable( $record["downl_bytes"] )." (".$record["downl_packets"]."pckts)";
		echo "</td>\n";
		echo "      <td>";
		echo human_readable( $record["upl_bytes"] )." (".$record["upl_packets"]."pckts)";
		echo "</td>\n";
		echo "    </tr>\n";
	
	}
}
pg_free_result($result);


/* Closing connection */
pg_close($dbconn);



?>
