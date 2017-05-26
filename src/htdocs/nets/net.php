<?php
/*
** net.php - 
**   This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Thu Nov  8 22:26:22 EST 2001
*/
	include ("../config.php");
	include ("./net_back.php");

	$net = new Net;

	if ($HTTP_GET_VARS["action"] == "add")
		$net->ShowHTML();
	else if ($HTTP_GET_VARS["action"] == "edit")
	{
		$net->SetID($HTTP_GET_VARS["id"]);
		$net->ShowHTML();
	}
	else if ($HTTP_GET_VARS["action"] == "delete")
	{
		$net->Deactivate($HTTP_GET_VARS["id"]);
	} 
	else if ($HTTP_GET_VARS["action"] == "show_all")
	{
		$nets = new Nets;
		$nets->ShowAll($HTTP_GET_VARS["message"]);
	} 
	else if ($HTTP_POST_VARS["action"] == "post_cb")
	{	
		$net->HandlePost();
	} else
	{
		echo "Bad action";
	}
?>
