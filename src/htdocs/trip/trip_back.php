<?php

/*
** trip_back.php -
**   This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Sat Feb 16 11:01:46 EST 2002
*/

include_once ("../html/html_back.php");
include_once ("../effort/tow_back.php");


class Trip
{
	var $tripid;
	var $vesselid;
	var $mTows;
	var $depart;
	var $return;
	
	function Trip()
	{
		$this->tripid = 0;	
		$this->vesselid = 0;	
	}

	function SetID($tripid)
	{
		// connect to database
		$config = new Config;
		$conn = pg_Connect($config->GetDBConnectionString());   

		$result = pg_exec($conn,"select * from trip where tripid=$tripid;");
		if (!$result)
		{
			echo "Problem with tripid query";
			pg_close($conn);
			return false;
		} else
		{
			if (pg_numrows($result) == 0)
			{
				echo "Trip ID not found";
				pg_close($conn);
				return false;
			}
			$data = pg_fetch_object($result, $row);
			$this->tripid = $data->tripid;	
			$this->vesselid = $data->vesselid;
			$this->depart = $data->depart;
			$this->return = $data->return;
		}

		pg_close($conn);
		return true;

	}

	function GetTows()
	{
	
		// connect to database
		$config = new Config;
		$conn = pg_Connect($config->GetDBConnectionString());   

		//now the tows
		// @REX note that we are grabbing draws in here as well -- when we go live we will need to
		//	put a "not in the effort_draw table" clause in here.
		$sql = "select * from effort,effort_trip where effort.effortid=effort_trip.effortid and effort_trip.tripid=$this->tripid";
		//echo $sql;
		$result = pg_exec($conn, $sql); 
		if (!$result)
		{
			pg_close($conn);
			return "Problem with tows-in-trip query";
		} else
		{
			$num = pg_numrows($result);
                       	for ($row = 0; $row < $num && $data = pg_fetch_object($result, $row); $row++)
                       	{
				$tow = new Tow();
				$tow->SetID($data->effortid);
				$retval = $retval.$tow->ShowSynopsis();
			}
			
			
		}
		//echo $retval;
		return $retval;
	}

	/*
	** ShowHTML - Show a form for either starting or editing a trip's info. Currently, there is
	**	no information needed other than the vesselid.
	**	@REX vesselid hardcoded to the Prowler!
	*/
	function ShowHTML()
	{

		if ($this->tripid ==0)
		{
			$button_text = "Start";
			$title = "Add Trip";
			$action = "add";
		}
		else
		{
			$button_text = "Update";
			$title = "Edit Trip";
			$action = "edit";
		}

		$html = new HTML($title, "trip", $action, "<form method=\"post\" action=\"trip.php\">
			<input type=\"hidden\" name=\"action\" value=\"post_cb\">
			<input type=\"hidden\" name=\"tripid\" value=\"".$this->tripid."\">
			<input type=\"hidden\" name=\"vesselid\" value=\"1\">
			<table>
				<tr>
					<td>&nbsp;</td>
					<td><input type=\"submit\" value=\"".$button_text."\"></td>
				</tr>
			</table>
		</form>
");	
		echo $html->Get();
	}

	function HandlePost()
	{
		global $HTTP_POST_VARS;
		$this->tripid = $HTTP_POST_VARS['tripid'];
		$this->vesselid= $HTTP_POST_VARS['vesselid'];
		
		// connect to database
		$config = new Config;
		$conn = pg_Connect($config->GetDBConnectionString());   

		// insert or update?
		if ($HTTP_POST_VARS["tripid"] == 0)
		{
			$sql_string = "select trip_begin('$this->vesselid')";
		} else
		{
			$sql_string = "select trip_end($this->tripid)";
		}

		$result = pg_exec($conn,$sql_string);
		if (!$result)
			echo "Problem with query";
		else
		{
			if ($HTTP_POST_VARS["tripid"] == 0)
				header("Location: trip.php?action=show_all&message=Trip Started");
			else
				header("Location: trip.php?action=show_all&message=Trip Edited");
		}
	}

	function GetHeaders()
	{
		$tow = new Tow();
		return $tow->GetHeaders();
	}

	function ShowTows()
	{
		$html = "
	<!-- tow table -->
	<table border=\"1\">".$this->GetHeaders().$this->GetTows()."
	</table>
	<!-- end tow table -->";
		
		$out = new HTML("Trip Sheet","trip","sheet",$html);
		echo $out->Get();
	}

	function ShowSynopsis()
	{
		return "<h2>$this->givenname $this->surname</h2>
[<a href=\"trip.php?action=edit&id=$this->tripid\">edit</a>] [<a href=\"trip.php?action=delete&id=$this->tripid\">delete</a>]";
	}

	function Deactivate($id)
	{
		$config = new Config;
		$conn = pg_Connect($config->GetDBConnectionString());   

		$result = pg_exec($conn,"update trip set active=false where tripid=$id;");
		header("Location: trip.php?action=show_all&message=Trip Deleted");

	}
}

?>
