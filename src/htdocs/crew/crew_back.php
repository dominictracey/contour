<?php

/*
** crew_back.php - 
**   This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Thu Nov  8 22:28:35 EST 2001
*/

include ("../html/html_back.php");


class Crew
{
	var $crewid;
	var $givenname;
	var $surname;
	var $ssn;
	
	function Crew()
	{
		$this->crewid = 0;	
		$this->givenname = "";
		$this->surname = "";
		$this->ssn = "";
	}

	function SetID($crewid)
	{
		// connect to database
		$config = new Config;
		$conn = pg_Connect($config->GetDBConnectionString());   

		$result = pg_exec($conn,"select * from crew where crewid=$crewid;");
		if (!$result)
		{
			echo "Problem with query";
			pg_close($conn);
			return false;
		} else
		{
			if (pg_numrows($result) == 0)
			{
				echo "Crew ID not found";
				pg_close($conn);
				return false;
			}
			$data = pg_fetch_object($result, $row);
			$this->crewid = $data->crewid;	
			$this->givenname = $data->given_name;
			$this->surname = $data->surname;
			$this->ssn = $data->taxid;
		}

		pg_close($conn);
		return true;

	}

	function ShowHTML()
	{

		if ($this->crewid ==0)
		{
			$button_text = "Add";
			$title = "Add Crew";
			$action = "add";
		}
		else
		{
			$button_text = "Update";
			$title = "Edit Crew";
			$action = "edit";
		}

		$html = new HTML($title, "crew", $action, "<form method=\"post\" action=\"crew.php\">
			<input type=\"hidden\" name=\"action\" value=\"post_cb\">
			<input type=\"hidden\" name=\"crewid\" value=\"".$this->crewid."\">
			<table>
				<tr>
					<td>First Name:</td>
					<td><input type=\"text\" name=\"givenname\" value=\"".$this->givenname."\"></td>
				</tr>
				<tr>
					<td>Last Name:</td>
					<td><input type=\"text\" name=\"surname\" value=\"".$this->surname."\"></td>
				</tr>
				<tr>
					<td>Social Security #:</td>
					<td><input type=\"text\" name=\"ssn\" value=\"".$this->ssn."\"></td>
				</tr>
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
		$this->crewid = $HTTP_POST_VARS['crewid'];
		$this->givenname = $HTTP_POST_VARS['givenname'];
		$this->surname = $HTTP_POST_VARS['surname'];
		$this->ssn = $HTTP_POST_VARS['ssn'];
		
		// connect to database
		$config = new Config;
		$conn = pg_Connect($config->GetDBConnectionString());   

		// insert or update?
		if ($HTTP_POST_VARS["crewid"] == 0)
		{
			$sql_string = "select crew_add('$this->givenname',
				'$this->surname', '$this->ssn')";
		} else
		{
			$sql_string = "select crew_edit($this->crewid,'$this->givenname',
				'$this->surname', '$this->ssn',true)";
		}

		$result = pg_exec($conn,$sql_string);
		if (!$result)
			echo "Problem with query";
		else
		{
			if ($HTTP_POST_VARS["crewid"] == 0)
				header("Location: crew.php?action=show_all&message=Crew Member Added");
			else
				header("Location: crew.php?action=show_all&message=Crew Information Edited");
			echo "
	<form><input onClick=\"window.close()\" value=\"Done\" type=\"submit\"/></form>
	</body>
</html>";

		}
	}


	function ShowSynopsis()
	{
		return "<h2>$this->givenname $this->surname</h2>
[<a href=\"crew.php?action=edit&id=$this->crewid\">edit</a>] [<a href=\"crew.php?action=delete&id=$this->crewid\">delete</a>]";
	}

	function Deactivate($id)
	{
		$config = new Config;
		$conn = pg_Connect($config->GetDBConnectionString());   

		$result = pg_exec($conn,"update crew set active=false where crewid=$id;");
		header("Location: crew.php?action=show_all&message=Crew Deleted");

	}
}

class CrewMembers
{
	function CrewMembers()
	{

	}

	function ShowAll($message)
	{
		$html = "<font color=\"#ff0000\" size=\"+2\">".$message."</font><p>
	<a href=\"crew.php?action=add\"><b>[Add New Crew Member]</b></a>";
                // connect to database
                $config = new Config;
                $conn = pg_Connect($config->GetDBConnectionString());

                $result = pg_exec($conn,"select crewid from crew where active=true;");  
                if (!$result)
                {
                        echo "Problem with query";
                        pg_close($conn);
                        return false;
                } else
                {
                        $num = pg_numrows($result);
                        if ($num == 0)
                        {
                                echo "No crew found";
                                pg_close($conn);
                                return false;
                        }
                        for ($row = 0; $row < $num && $data = pg_fetch_object($result, $row); $row++)
                        {
				$crew = new Crew;
				$crew->SetID($data->crewid);
				$html = $html.$crew->ShowSynopsis();
			}
		}
		$out = new HTML("Crew Members", "crew", "all", $html);
		echo $out->Get();
	}
}

?>
