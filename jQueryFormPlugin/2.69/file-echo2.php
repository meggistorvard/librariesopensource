<?php
header('Access-Control-Allow-Origin: *');
header('Access-Control-Allow-Methods: POST, GET, OPTIONS');

foreach($_FILES as $file) {
    $n = $file['name'];
    $s = $file['size'];
    if (is_array($n)) {
        $c = count($n);
        for ($i=0; $i &lt; $c; $i++) {
            echo "&lt;br>uploaded: " . $n[$i] . " (" . $s[$i] . " bytes)";
        }
    }
    else {
        echo "&lt;br>uploaded: $n ($s bytes)";
    }
}