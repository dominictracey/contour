<?php

/*
** history.php - 
**   This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Sat Feb 16 11:01:14 EST 2002
*/


include "../html/html_back.php";
include "../config.php";

// connect to database
$config = new Config;
$conn = pg_Connect($config->GetDBConnectionString());   

$result = pg_exec($conn, "select * from trip");

$html = "<a href=\"trip.php?action=add\">New Trip</a><p>
<table>";
if (!$result)
{
	echo "Problem with query";
	pg_close($conn);
	return false;
} 
else
{
	$num = pg_numrows($result);
	if ($num == 0)
	{
		$html = $html."No trips found";
	}
	for ($row = 0; $row < $num && $data = pg_fetch_object($result, $row); $row++)
	{
		$html = $html."
	<tr>
		<td><a href=\"trip.php?action=show&id=$data->tripid\">$data->tripid</a></td><td>$data->depart</td>
	</tr>";
	}
}

$html = $html."
</table>";

$out = new HTML("history", "trip", "hail", $html); 
echo $out->Get();
pg_close($conn);

?>

