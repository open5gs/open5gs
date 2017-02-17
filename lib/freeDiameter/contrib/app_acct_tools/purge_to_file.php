<?php
# This file may be called either from CLI or web PHP installation.

# The copyright is the same as the freeDiameter project. Licence is BSD.

/*-------------------------------------------------------*/

/* -- These parameters must match the app_acct.conf file -- */

/* The Connection String used to access the database.
  Example: "host=localhost dbname=app_acct user=freediameter password=foo" */
$CONNSTR=""; 

/* The name of the table were the records are saved by app_acct.fdx */
$TABLE="incoming";


/* -- These parameters are specific to this script -- */

/* Name of the dump file to create. It can be relative or absolute. 
   The script fails if this file already exists.
   Use for example "data-".date("Ymdhis").".sql" to avoid collisions. */
$DUMPFILE="incoming_data.sql";

/* Does the script also echoes an HTML TABLE of the data processed? */
$DISPLAY_HTML=FALSE; // it would be better to detect if we are called through Web or CLI...
/* In addition, issue a full HTML page (including headers) ? */
$HTML_HEADERS=TRUE;

/*-------------------------------------------------------*/

/* Connect to the database */
$dbconn = pg_connect($CONNSTR)
    or die('Could not connect: ' . pg_last_error() . "\n");
    
/* Check if the file exists */
$file = fopen($DUMPFILE, "xb")
    or die("The file '$DUMPFILE' already exists or cannot be created, aborting.\n");

/* First, query the table format */
$format_sql =   "SELECT ".
		"   a.attname AS field,".
		"   t.typname AS type,".
/*		"   a.attlen AS length,". */
		"   a.atttypmod AS lengthvar,".
		"   a.attnotnull AS notnull".
		" FROM".
		"   pg_class c,".
		"   pg_attribute a,".
		"   pg_type t".
		" WHERE".
		"   c.relname = '" . $TABLE . "'".
		"   AND a.attnum > 0".
		"   AND a.attrelid = c.oid".
		"   AND a.atttypid = t.oid".
		"   ORDER BY a.attnum";
$result = pg_query($dbconn, $format_sql) or die('Query failed: ' . pg_last_error() . "\n");

/* app_acct.fdx only uses a few different fields types: */
$conv_types = array(
    "timestamptz" => "timestamp with time zone", /* recorded on */
    "bytea" => "bytea",   /* octet string */
    "int4" => "integer",  /* unsigned32, integer32, float32 */
    "int8" => "bigint"    /* unsigned64, integer64, float64 */
    );
    
$fields_types=array();

fwrite($file, " -- Data purged on ".date(DATE_RFC822)."\n\n");
fwrite($file, " -- Format of the table it was extracted from:\n");
fwrite($file, " -- CREATE TABLE \"".$TABLE."\" (\n");
$i = 0;
while ($field = pg_fetch_array($result, null, PGSQL_ASSOC)) {
	if ($i++)
		fwrite($file, ",\n");
	fwrite($file, " --    \"".$field["field"]."\" ");
	if (array_key_exists($field["type"], $conv_types))
		$fields_types[$field["field"]] = $conv_types[$field["type"]];
	else
		$fields_types[$field["field"]] = $field["type"];
	fwrite($file, $fields_types[$field["field"]]);
	if ($field["lengthvar"] != "-1")
		fwrite($file, "(".$field["lengthvar"].")");
	if ($field["notnull"] == "t")
		fwrite($file, " NOT NULL");
}
fwrite($file, "\n -- );\n\n");
pg_free_result($result);

/* Now, the data */
$result = pg_query($dbconn, "SELECT * FROM \"".$TABLE."\"") or die('Query failed: ' . pg_last_error() . "\n");
if (pg_num_rows($result) > 0) {
	fwrite($file, "INSERT INTO \"".$TABLE."\"\n (\n");
	$i = pg_num_fields($result);
	if ($DISPLAY_HTML && $HTML_HEADERS) echo "<HTML>\n<HEAD><TITLE>Purge</TITLE></HEAD>\n<BODY>\n";
	if ($DISPLAY_HTML) echo "<TABLE>\n  <TR>\n";
	for ($j = 0; $j < $i; $j++) {
	      fwrite($file, ($j ? ", " : "") . "\"" . pg_escape_string(pg_field_name($result, $j)). "\"");
	      if ($DISPLAY_HTML) echo "    <TD>".htmlentities(pg_field_name($result, $j))."</TD>\n";
	}
	fwrite($file, "\n )\n VALUES \n");
	if ($DISPLAY_HTML) echo "  </TR>\n";
	$i = 0;
	while ($line = pg_fetch_array($result, null, PGSQL_ASSOC)) {
		if ($i++)
			fwrite($file, ",\n");
		fwrite($file, " ( ");
		
		if ($DISPLAY_HTML) echo "  <TR>\n";
		$j = 0;
		$sql = "";
		foreach ($line as $f => $v) {
			if (!is_null($v))
				switch ($fields_types[$f]) {
					case "bytea":
						$v = "E'".pg_escape_bytea(pg_unescape_bytea($v))."'";
						break;
					case "timestamp with time zone":
						$v = "E'".pg_escape_string($v)."'";
						break;
				}
			else
				$v = "NULL";
			
	      		if ($DISPLAY_HTML) echo "    <TD>".htmlentities(print_r($line[$f], TRUE))."</TD>\n";

			fwrite($file, ($j ? ", " : "") . $v);
			$sql .= ($j ? " AND " : "") . "\"".pg_escape_string($f)."\" ";
			if (is_null($line[$f]))
				$sql .= " IS NULL";
			else 
				$sql .= " = " . $v;
			$j++;
		}
		fwrite($file, ")");
		if ($DISPLAY_HTML) echo "  </TR>\n";
		$res = pg_query( "DELETE FROM \"".$TABLE."\" WHERE ".$sql) or die('DELETE query failed: ' . pg_last_error() . "\n");
		pg_free_result($res);
	}
	fwrite($file, "\n;\n");
	if ($DISPLAY_HTML) echo "</TABLE>\n";
	if ($DISPLAY_HTML && $HTML_HEADERS) echo "</BODY>\n</HTML>\n";
        else echo $i." records have been successfully written to '".$DUMPFILE."' and removed from database.\n";
} else {
	if ($DISPLAY_HTML) echo "<p><em>No new record in the database</em></p>\n";
        else echo "No new record in the database, the generated file is empty.\n";
}
pg_free_result($result);

/* Closing connection */
pg_close($dbconn);

/* Closing the file */
fclose($file);


?>
