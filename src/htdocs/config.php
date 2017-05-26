<?php

class Config
{
	var $dbHost;
	var $dbPort;
	var $dbName;
	var $dbUser;

	function Config()
	{
		$this->dbHost = "192.168.2.1";
		$this->dbPort = "5432";
		$this->dbName = "contour";
		$this->dbUser = "dominic";
	}

	// Put together a Postgres connection string
	function GetDBConnectionString()
	{
		return ( "host=".$this->dbHost." port=". $this->dbPort." dbname=". $this->dbName." user=". $this->dbUser );
	}
}

?>
