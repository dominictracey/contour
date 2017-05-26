<?php
/*
** crew.php - 
**   This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Thu Nov  8 22:25:58 EST 2001
*/
	include ("../config.php");
	include ("./crew_back.php");

	$crew = new Crew;

	if ($HTTP_GET_VARS["action"] == "add")
		$crew->ShowHTML();
	else if ($HTTP_GET_VARS["action"] == "edit")
	{
		$crew->SetID($HTTP_GET_VARS["id"]);
		$crew->ShowHTML();
	}
	else if ($HTTP_GET_VARS["action"] == "delete")
	{
		$crew->Deactivate($HTTP_GET_VARS["id"]);
	} 
	else if ($HTTP_GET_VARS["action"] == "show_all")
	{
		$crews = new CrewMembers;
		$crews->ShowAll($HTTP_GET_VARS["message"]);
	} else if ($HTTP_POST_VARS["action"] == "post_cb")
	{	
		$crew->HandlePost();
	} else
	{
		echo "Bad action";
	}
?>
