
<?php
$blockFlag = trim( file_get_contents( "/home/jcr13/public_html/block.txt" ) );

if( $blockFlag == 1 ) {
    echo "Disabled";
    die();
}

file_put_contents( "/home/jcr13/public_html/flag.txt", "1" );

?>



<img src=pics/backDoor.jpg><br><br><br>
<img src=pics/frontDoor.jpg><br><br><br>
<img src=pics/frontYard.jpg><br><br><br>
<img src=pics/rearDrive.jpg><br><br><br>
<img src=pics/bulkhead.jpg><br><br><br>
<img src=pics/frontDrive.jpg><br><br><br>
<img src=pics/garage.jpg><br><br><br>
<img src=pics/sideDrive.jpg><br><br><br>
