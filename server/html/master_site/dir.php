<?php

$arrFiles = array();

$handle = opendir('/');

if ($handle) {

    while (($entry = readdir($handle)) !== FALSE) {

        $arrFiles[] = $entry;
        print "$entry";
        echo '';

    }

}

closedir($handle);

?>
