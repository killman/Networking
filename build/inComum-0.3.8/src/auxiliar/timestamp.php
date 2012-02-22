#!/usr/bin/php
<?
if($_SERVER['argv'][1]) echo date('d-m-Y H:i:s',$_SERVER['argv'][1])."\n";
while(true){
	echo date('d-m-Y H:i:s',trim(fgets(STDIN)))."\n";
}
?>
