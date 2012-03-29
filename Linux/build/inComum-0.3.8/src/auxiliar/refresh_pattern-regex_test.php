#!/usr/bin/php -q
<?php
while(true){
	$argv = explode(' ',trim(fgets(STDIN)));
	$argv = $argv[0];

	if(preg_match("/^http:\/\/.{0,2}\.video[0-9]\.blip\.tv.*\.(flv|mp4|wmv|rm|ram|mov|avi|m4v|mp3)/", $argv)){
		echo "ok.\n";
	}else{
		echo "not ok.\n";
	}
}
/*
 * This file is intended to help you to test quickly your refresh pattern
 * lines. Simply run it and insert some URL against above pattern
 *
 * You should not install it together with inComum
 *
 * */

?>