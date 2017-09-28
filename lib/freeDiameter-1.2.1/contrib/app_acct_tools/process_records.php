<?php
# This file may be called either from CLI or web PHP installation.
# The copyright is the same as the freeDiameter project. Licence is BSD.

# IMPORTANT: This script is highly experimental, PLEASE KEEP A COPY OF YOUR ACCOUNTING DATA 
# if this data has any importance.

/*-------------------------------------------------------*/

/* This script supports only one database where all tables are kept. 
  The Connection String used to access that database:
  Example: "host=localhost dbname=app_acct user=freediameter password=foo" */
$CONNSTR=""; 


/**** 1 : Incoming records (output of app_acct.fdx) ****/
/* The name of the table were the raw records are saved (from app_acct.conf) */
$INCOMING="incoming";

/* Note: For this script, this table MUST contain the following fields (with these names): 
 Session-Id, User-Name, Accounting-Record-Type, Accounting-Record-Number, Acct-Session-Id, 
 Accounting-{In,Out}put-{Octets,Packets},
 NAS-Identifier, Called-Station-Id, Calling-Station-Id, recorded_on.
 */


/**** 2 : Processed records (output of this script, input for display_results.php) ****/
/* The name of the table */
$PROCESSED="processed";

/* See process_database.sql for command to create this database */

/**** 3 : Orphan records (optional) ****/
/* The script can move records belonging to an unterminated session that has not received any new
  record for more than $ORPHAN_DELAY (based on recorded_on field) into an $ORPHANED_TABLE table, so that
  these records are not re-processed everytime the script runs. 
  If $ORPHANED_TABLE is empty, this feature is disabled.  */
$ORPHANED_TABLE="orphans";
$ORPHAN_DELAY = "2 days";


/**** 4 : Archived records (optional) ****/
/* When data has been processed successfully, the records can be saved into an archive table before being deleted. */
/* This table also must have the same structure as $INCOMING */
$ARCHIVES_TABLE="archived";

/*-------------------------------------------------------*/

/* Connect to the database */
$dbconn = pg_connect($CONNSTR)
    or die('Could not connect: ' . pg_last_error() . "\n");

/* Handle orphans first */
if ($ORPHANED_TABLE) {
	$orphans_sql = 'SELECT * FROM "'.$INCOMING.'" ';
	$orphans_sql.= 'WHERE "Acct-Session-Id" IN (';
	$orphans_sql.= '   SELECT data.asid ';
	$orphans_sql.= '   FROM (';
	$orphans_sql.= '       SELECT "Acct-Session-Id" as asid, MAX("recorded_on") as latest, bool_or("Accounting-Record-Type" = 2) as got_start, bool_or("Accounting-Record-Type" = 4) as got_stop  ';
	$orphans_sql.= '       FROM "'.$INCOMING.'"';
	$orphans_sql.= '       GROUP BY "Acct-Session-Id") ';
	$orphans_sql.= '   as data';
	$orphans_sql.= '   WHERE data.latest < current_timestamp - interval \''. $ORPHAN_DELAY .'\' AND NOT ( got_start AND got_stop )';
	$orphans_sql.= ');';
	
	/* Execute, move the orphaned records to the appropriate table. */
	$result = pg_query($dbconn, $orphans_sql) or die('Query failed: ' . pg_last_error() . "\n");
	if (pg_num_rows($result) > 0) {
		$i = 0;
		while ($orphan = pg_fetch_array($result, null, PGSQL_ASSOC)) {
			$i++;
			
			// this doesn't work: pg_insert( $dbconn, $ORPHANED_TABLE, $orphan ) or die('Insert failed: ' . pg_last_error() . "\n");
			$sql  = "INSERT INTO \"$ORPHANED_TABLE\" ";
			$sql .= '("';
			$sql .= join('", "', array_keys($orphan));
			$sql .= '") VALUES (';
			for($c = 0; $c < count($orphan); $c++)
			  $sql .= ($c ? ', ' : '').'$'.($c+1);
			$sql .= ')';
			pg_query_params($dbconn, $sql, array_values($orphan)) or die('Insert failed: ' . pg_last_error() . "\n");
						
			// This also doesn't work: pg_delete( $dbconn, $INCOMING, $orphan ) or die('Removing orphan failed: ' . pg_last_error() . "\n");
			$sql  = "DELETE FROM \"$INCOMING\" WHERE ";
			$sql .= '"Acct-Session-Id" = $1 AND "recorded_on" = $2';
			pg_query_params($dbconn, $sql, array($orphan["Acct-Session-Id"], $orphan["recorded_on"])) or die('Removing orphan failed: ' . pg_last_error() . "\n");
		}
		echo $i." orphans have been moved to '".$ORPHANED_TABLE."'\n";
	}
	pg_free_result($result);
}

/* Delete duplicate records (which might have been received by different servers on different time, but are identical otherwise */
$duplicate_sql  = ' SELECT * FROM (SELECT count(*) as cnt, min("recorded_on") as first, "Session-Id", "Acct-Session-Id", "Accounting-Record-Type", "Accounting-Record-Number"';
$duplicate_sql .= ' FROM "'.$INCOMING.'" GROUP BY "Session-Id", "Acct-Session-Id", "Accounting-Record-Type", "Accounting-Record-Number") as qry WHERE qry.cnt > 1';
$result = pg_query($dbconn, $orphans_sql) or die('Query failed: ' . pg_last_error() . "\n");
if (pg_num_rows($result) > 0) {
	/* We have some duplicates to delete */
	$i=0;
	while ($dup = pg_fetch_array($result, null, PGSQL_ASSOC)) {
		$i++;
		$sql  = "DELETE FROM \"$INCOMING\" WHERE ";
		$sql .= '"Session-Id" = $1 AND "Acct-Session-Id" = $2 AND "Accounting-Record-Type" = $3 AND "Accounting-Record-Number" = $4 AND "recorded_on" <> $5';
		$params = array($dup["Session-Id"], $dup["Acct-Session-Id"], $dup["Accounting-Record-Type"], $dup["Accounting-Record-Number"], $dup["first"]);
		pg_query_params($dbconn, $sql, $params) or die('Removing duplicates failed: ' . pg_last_error() . "\n");
	}
	echo $i." duplicate records have been deleted from '".$INCOMING."'\n";
}
pg_free_result($result);


/* Now, find Acct-Session-Id of completed sessions */
$completed_sql  = ' SELECT "Session-Id", "Acct-Session-Id" FROM (';
$completed_sql .= '     SELECT "Session-Id", "Acct-Session-Id", bool_or("Accounting-Record-Type" = 2) as got_start, bool_or("Accounting-Record-Type" = 4) as got_stop ';
$completed_sql .= '     FROM "'.$INCOMING.'" GROUP BY "Session-Id", "Acct-Session-Id"';
$completed_sql .= '    ) as input';
$completed_sql .= ' WHERE got_start AND got_stop';

$sids = pg_query($dbconn, $completed_sql) or die('Query failed: ' . pg_last_error() . "\n");
if (pg_num_rows($sids) > 0) {
	while ($sids_line = pg_fetch_array($sids, null, PGSQL_ASSOC)) {
		$sid  = $sids_line["Session-Id"];
		$asid = $sids_line["Acct-Session-Id"];
		echo "Processing Acct-Session-Id '".$asid."', Session-Id '".$sid ."'... ";
		
		$data=array();
		
		/* We simply ignore the interim record(s) in this first version, since they contain only cumulative data. It could be used in later version to draw the session details for example. */
		
		$result = pg_query_params($dbconn, 'SELECT *, "recorded_on" - CAST(textcat(text("Acct-Session-Time"), text(\' seconds\')) as INTERVAL) as start_time FROM "'.$INCOMING.
						   '" WHERE "Session-Id" = $1 AND "Acct-Session-Id" = $2 AND "Accounting-Record-Type" = 4 ORDER BY "recorded_on"', 
						   array($sid, $asid)) or die('Query failed: ' . pg_last_error() . "\n");
		$record = pg_fetch_array($result, null, PGSQL_ASSOC) or die('Internal error, got_stop is true but no record was returned');
		
		$data[/* "user_name" */] = $record["User-Name"];
		$data[/* "user_device" */] = $record["Calling-Station-Id"];
		
		$nas_id= ($record["NAS-Identifier"] ?: $record["Origin-Host"]) ?: "<unidentified NAS>";
		$ip = $record["NAS-IP-Address"] ?: $record["NAS-IPv6-Address"];
		$nas_ip= $ip ? inet_ntop(pg_unescape_bytea($ip)) : "<unknown NAS IP>";
		$nas_csi = $record["Called-Station-Id"];
		$data[/* "nas_info" */] = $nas_id . " (".$nas_ip.")" . ($nas_csi ? " - Called Station: ".$nas_csi : "");
		
		/* Is it possible to infer the session start time from this record only? */
		if ($record["Acct-Session-Time"]) {
			/* Yes, let's go */
			$data[/* "sess_start" */] = $record["start_time"];
			$data[/* "sess_duration" */] = $record["Acct-Session-Time"]." seconds";
		} else {
			/* No the information is missing, let's compute the approx value with the START record timestamp */
			$res = pg_query_params($dbconn, 'SELECT t_start."recorded_on" as begining, t_end."recorded_on" - t_start."recorded_on" as duration'.
							' FROM (SELECT "recorded_on" FROM "'.$INCOMING.'"  WHERE "Session-Id" = $1 AND "Acct-Session-Id" = $2 AND "Accounting-Record-Type" = 4 ORDER BY "recorded_on" LIMIT 1) as t_end, '.
							'      (SELECT "recorded_on" FROM "'.$INCOMING.'"  WHERE "Session-Id" = $1 AND "Acct-Session-Id" = $2 AND "Accounting-Record-Type" = 2 ORDER BY "Accounting-Record-Number", "recorded_on" LIMIT 1) as t_start',
							array($sid, $asid)) or die('Query failed: ' . pg_last_error() . "\n");
			$vals = pg_fetch_array($result, null, PGSQL_ASSOC) or die('Internal error, unable to compute session time');
			$data[/* "sess_start" */] = $vals["begining"];
			$data[/* "sess_duration" */] = $vals["duration"];
			pg_free_result($res);
		}
		
		$data[/* "downl_bytes" */] = $record["Accounting-Output-Octets"];
		$data[/* "downl_packets" */] = $record["Accounting-Output-Packets"];
		$data[/* "upl_bytes" */] = $record["Accounting-Input-Octets"];
		$data[/* "upl_packets" */] = $record["Accounting-Input-Packets"];
		pg_free_result($result);
		
		$result = pg_query_params($dbconn, 
			'INSERT INTO "'.$PROCESSED.'" (user_name, user_device, nas_info, sess_start, sess_duration, downl_bytes, downl_packets, upl_bytes, upl_packets) '.
			'VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9)', $data) or die('Query failed: ' . pg_last_error() . "\n");
		pg_free_result($result);
		echo "Data stored into '$PROCESSED'... ";
		
		/* Now that we have processed it, move these records to the $ARCHIVES_TABLE table. */
		if ($ARCHIVES_TABLE) {
			$result = pg_query_params($dbconn, 'SELECT * FROM "'.$INCOMING.'" WHERE "Session-Id" = $1 AND "Acct-Session-Id" = $2', array($sid, $asid)) or die('Query failed: ' . pg_last_error() . "\n");
			$i = 0;
			while ($rec = pg_fetch_array($result, null, PGSQL_ASSOC)) {
				$i++;

				$sql  = "INSERT INTO \"$ARCHIVES_TABLE\" ";
				$sql .= '("';
				$sql .= join('", "', array_keys($rec));
				$sql .= '") VALUES (';
				for($c = 0; $c < count($rec); $c++)
				  $sql .= ($c ? ', ' : '').'$'.($c+1);
				$sql .= ')';
				pg_query_params($dbconn, $sql, array_values($rec)) or die('Insert failed: ' . pg_last_error() . "\n");
			}
			echo $i." records archived into '".$ARCHIVES_TABLE."'";
			pg_free_result($result);
		}
		echo "\n";
		$result = pg_query_params($dbconn, 'DELETE FROM "'.$INCOMING.'" WHERE "Session-Id" = $1 AND "Acct-Session-Id" = $2', array($sid, $asid)) or die('Query failed: ' . pg_last_error() . "\n");
		pg_free_result($result);
	}
}
pg_free_result($sids);

echo "Operation completed with success!\n";

/* Closing connection */
pg_close($dbconn);


?>
