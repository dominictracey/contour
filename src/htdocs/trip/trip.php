<?php

/*
** trip.php - 
**   This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Sat Feb 16 11:01:46 EST 2002
*/

        include ("../config.php");
        include ("./trip_back.php");

        $trip = new Trip;

        if ($HTTP_GET_VARS["action"] == "add")
                $trip->ShowHTML();
        else if ($HTTP_GET_VARS["action"] == "edit")
        {
                $trip->SetID($HTTP_GET_VARS["id"]);
                $trip->ShowHTML();
	}
         else if ($HTTP_GET_VARS["action"] == "show")
        {
                $trip->SetID($HTTP_GET_VARS["id"]);
                $trip->ShowTows();
        }
        else if ($HTTP_GET_VARS["action"] == "delete")
        {
                $trip->Deactivate($HTTP_GET_VARS["id"]);
        }
        else if ($HTTP_POST_VARS["action"] == "post_cb")
        {
                $trip->HandlePost();
        } else
        {
                echo "Bad action";
        }

?>
