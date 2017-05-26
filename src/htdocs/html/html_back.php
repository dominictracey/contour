<?php

class HTML
{
	var $mTitle;
	var $mClass;
	var $mAction;
	var $mBody;

	function HTML()
	{
		$this->mTitle = "";
		$this->mClass = "";
		$this->mAction = "";
		$this->mBody = "";
	}

	function HTML($title, $class, $action, $body)
	{
		$this->mTitle = $title;
		$this->mClass = $class;
		$this->mAction = $action;
		$this->mBody = $body;
	}

	function SetTitle($title)
	{
		$this->mTitle = $title;
	}

	function SetClass($class)
	{
		$this->mClass = $class;
	}

	function SetAction($action)
	{
		$this->mAction = $action;
	}

	function SetBody($body)
	{
		$this->mBody = $body;
	}

	function Get()
	{
		return "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 3.2//EN\">
<html>
	<head>
		<title>".$this->mTitle."</title>
	</head>
	<body>
		<img src=\"../graphix/".$this->mClass."_".$this->mAction.".png\"><br>
		$this->mBody
	</body>
</html>";
	} 

}

?>
