<?php

include_once ("../html/html_back.php");


class Net
{
	var $netid;
	var $name;
	var $description;
	var $footrope_length;
	var $headrope_length;
	var $legs;
	var $meshes_across_belly;
	var $meshes_down_belly;
	var $frame_type;
	var $frame_size_inches;
	var $date_built;
	var $builder;
	
	function Net()
	{
		$this->netid = 0;	
		$this->name = "";
		$this->description = "";
		$this->footrope_length = 0;
		$this->headrope_length = 0;
		$this->legs = 0;
		$this->meshes_across_belly = 0;
		$this->meshes_down_belly = 0;
		$this->frame_type = 0;
		$this->frame_size_inches = 0;
		$this->date_built = 0;
		$this->builder = 0;
	}

	function SetID($netid)
	{
		// connect to database
		$config = new Config;
		$conn = pg_Connect($config->GetDBConnectionString());   

		$result = pg_exec($conn,"select * from nets where netid=$netid;");
		if (!$result)
		{
			echo "Problem with query";
			pg_close($conn);
			return false;
		} else
		{
			if (pg_numrows($result) == 0)
			{
				echo "Net ID not found";
				pg_close($conn);
				return false;
			}
			$data = pg_fetch_object($result, $row);
			$this->netid = $data->netid;	
			$this->name = $data->name;
			$this->description = $data->description;
			$this->footrope_length = $data->footrope_length;
			$this->headrope_length = $data->headrope_length;
			$this->legs = $data->legs;
			$this->meshes_across_belly = $data->meshes_across_belly;
			$this->meshes_down_belly = $data->meshes_down_belly;
			$this->frame_type = $data->frame_type;
			$this->frame_size_inches = $data->frame_size_inches;
			$this->date_built = $data->date_built;
			$this->builder = $data->builder;
		}

		pg_close($conn);
		return true;

	}

	function GetFrameTypes($selected)
	{
		// connect to database
		$config = new Config;
		$conn = pg_Connect($config->GetDBConnectionString());   

		$result = pg_exec($conn,"select * from net_frame_type;");
		if (!$result)
		{
			echo "Problem with query";
			pg_close($conn);
			return "";
		} else
		{
			$num = pg_numrows($result);
			for ($row = 0; $row < $num && $data = pg_fetch_object($result, $row); $row++)
			{
				$retval = $retval."<option value=\"$data->net_frame_type_id\"";
				if ($data->net_frame_type_id == $selected)
					$retval = $retval." selected";
				$retval = $retval." >$data->short_name\n";
			}	
		}
		pg_close($conn);
		return $retval;
	}

	function ShowHTML()
	{
		if ($this->legs)
			$legs_checked = " CHECKED ";

		$frame_types = $this->GetFrameTypes($this->frame_type);

		if ($this->netid ==0)
		{
			$button_text = "Add";
			$graphic = "add";
		}
		else
		{
			$button_text = "Update";
			$graphic = "edit";
		}

		$html = new HTML("Add Net", "net", $graphic, "<form method=\"post\" action=\"net.php\">
			<input type=\"hidden\" name=\"action\" value=\"post_cb\">
			<input type=\"hidden\" name=\"netid\" value=\"".$this->netid."\">
			<table>
				<tr>
					<td>Name:</td>
					<td><input type=\"text\" name=\"name\" value=\"".$this->name."\"></td>
				</tr>
				<tr>
					<td>Description:</td>
					<td><textarea name=\"description\">".$this->description."</textarea></td>
				</tr>
				<tr>
					<td>Footrope length:</td>
					<td><input type=\"text\" name=\"footrope_length\" value=\"".$this->footrope_length."\"></td>
				</tr>
				<tr>
					<td>Headrope length:</td>
					<td><input type=\"text\" name=\"headrope_length\" value=\"".$this->headrope_length."\"></td>
				</tr>
				<tr>
					<td>Legs:</td>
					<td><input type=\"checkbox\" name=\"legs\" ".$legs_checked."></td>
				</tr>
				<tr>
					<td>Meshes across belly:</td>
					<td><input type=\"text\" name=\"meshes_across_belly\" value=\"".$this->meshes_across_belly."\"></td>
				</tr>
				<tr>
					<td>Meshes down belly:</td>
					<td><input type=\"text\" name=\"meshes_down_belly\" value=\"".$this->meshes_down_belly."\"></td>
				</tr>
				<tr>
					<td>Frame type:</td>
					<td><select name=\"frame_type\">".$frame_types."</select></td>
				</tr>
				<tr>
					<td>Frame Size (inches):</td>
					<td><input type=\"text\" name=\"frame_size_inches\" value=\"".$this->frame_size_inches."\"></td>
				</tr>
				<tr>
					<td>Date built:</td>
					<td><input type=\"text\" name=\"date_built\" value=\"".$this->date_built."\"></td>
				</tr>
				<tr>
					<td>Builder:</td>
					<td><input type=\"text\" name=\"builder\" value=\"".$this->builder."\"></td>
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
		$this->netid = $HTTP_POST_VARS['netid'];
		$this->name = $HTTP_POST_VARS['name'];
		$this->description = $HTTP_POST_VARS["description"];
		$this->footrope_length = $HTTP_POST_VARS["footrope_length"];
		$this->headrope_length =  $HTTP_POST_VARS["headrope_length"]; 
		if ($HTTP_POST_VARS["legs"] == "on")
			$this->legs = "true"; 
		else
			$this->legs = "false"; 
		$this->meshes_across_belly =  $HTTP_POST_VARS["meshes_across_belly"]; 
		$this->meshes_down_belly =  $HTTP_POST_VARS["meshes_down_belly"]; 
		$this->frame_type =  $HTTP_POST_VARS["frame_type"]; 
		$this->frame_size_inches =  $HTTP_POST_VARS["frame_size_inches"]; 
		$this->date_built =  $HTTP_POST_VARS["date_built"]; 
		$this->builder =  $HTTP_POST_VARS["builder"]; 

		// connect to database
		$config = new Config;
		$conn = pg_Connect($config->GetDBConnectionString());   

		// insert or update?
		if ($HTTP_POST_VARS["netid"] == 0)
		{
			$sql_string = "select net_add('$this->name',
				'$this->description',
				$this->footrope_length,
				$this->headrope_length,
				$this->legs,
				$this->meshes_across_belly,
				$this->meshes_down_belly,
				$this->frame_type,
				$this->frame_size_inches,
				date('$this->date_built'),
				'$this->builder')"; 
		} else
		{
			$sql_string = "select net_edit($this->netid,
				'$this->name',
				'$this->description',
				$this->footrope_length,
				$this->headrope_length,
				$this->legs ,
				$this->meshes_across_belly ,
				$this->meshes_down_belly ,
				$this->frame_type ,
				$this->frame_size_inches ,
				date('$this->date_built') ,
				'$this->builder',
				true)"; 
		}

		$result = pg_exec($conn,$sql_string);
		if (!$result)
			echo "Problem with query";
		else
		{
			if ($HTTP_POST_VARS["netid"] == 0)
			{
				//echo "Successfully added";
				header("Location: net.php?action=show_all&message=Net Added");
			}
			else
			{	
				//echo "Successfully edited";
				header("Location: net.php?action=show_all&message=Net Edited");
			}
		}
	}


	function ShowSynopsis()
	{
		return "<h1>$this->name</h1> [<a href=\"net.php?action=edit&id=$this->netid\">edit</a>] [<a href=\"net.php?action=delete&id=$this->netid\">delete</a>]";
	}

	function Deactivate($id)
	{
		$config = new Config;
		$conn = pg_Connect($config->GetDBConnectionString());   

		$result = pg_exec($conn,"update nets set active=false where netid=$id;");
		header("Location: net.php?action=show_all&message=Net Deleted");

	}
}

class Nets
{
	function Nets()
	{

	}

	function ShowAll($message)
	{
		$html = "<font color=\"#ff0000\" size=\"+2\">$message</font><p>
<a href=\"net.php?action=add\"><b>[Add New Net]</b></a>";
                // connect to database
                $config = new Config;
                $conn = pg_Connect($config->GetDBConnectionString());

                $result = pg_exec($conn,"select netid from nets where active=true;");   
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
                                echo "No nets found";
                                pg_close($conn);
                                return false;
                        }
                        for ($row = 0; $row < $num && $data = pg_fetch_object($result, $row); $row++)
                        {
                                $net = new Net;
                                $net->SetID($data->netid);
                                $html = $html.$net->ShowSynopsis();
                        }
                }

		$html = new HTML("Nets", "net", "all", $html); 
		echo $html->Get();
	}
}

?>
