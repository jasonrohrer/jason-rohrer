<?php

$action = lt_requestFilter( "action", "/[A-Z_]+/i" );

$arrive = "";
$travel = "";
$buffer = "15";

$resultHTML = "";

if( $action == "compute" ) {
   
   $arrive = lt_requestFilter( "arrive", "/[0-9]?[0-9]:[0-9][0-9]\s*(am|pm)?/i", "" );
   $travel = lt_requestFilter( "travel", "/([0-9]?[0-9]:)?[0-9][0-9]/i", "" );
   $buffer = lt_requestFilter( "buffer", "/[0-9]+/i", "15" );
   
   
   if( $arrive != "" && $travel != "" ) {
       $DateTime = new DateTime( $arrive );

       $travelHours = "";
       $travelMinutes = $travel;

       if( strstr( $travel, ":" ) ) {
           $parts = preg_split( "/:/", $travel );

           $travelHours = $parts[0];
           $travelMinutes = $parts[1];
           }

       if( $travelHours != "" ) {
           $DateTime->modify( "-$travelHours hours" );
           }
       if( $travelMinutes != "" ) {
           $DateTime->modify( "-$travelMinutes minutes" );
           }
       
       $DateTime->modify( "-$buffer minutes" );

       
       $leaveTime = $DateTime->format( "g:i a" );

       $DateTime->modify( "-10 minutes" );

       $readyTime = $DateTime->format( "g:i a" );
       
       $resultHTML = "Leave at:<br>$leaveTime<br><br><br>".
           "Get ready to leave at:<br>$readyTime";
       }
   
   }

?>

<html>

<head>
<title>Leave Timer</title>

</head>

<body bgcolor=white text=black link=blue alink=blue vlink=blue>

<center>
<form action="index.php" method="POST">
<INPUT TYPE="hidden" NAME="action" VALUE="compute">

<table border=0>

<tr>
<td>Arrive by:</td>
<td><INPUT TYPE="text" MAXLENGTH=10 SIZE=8 NAME="arrive"
           VALUE="<?php echo $arrive;?>"></td>
</tr>

<tr>
<td>Travel time:</td>
<td><INPUT TYPE="text" MAXLENGTH=10 SIZE=8 NAME="travel"
           VALUE="<?php echo $travel;?>"></td>
</tr>

<tr>
<td>Buffer:</td>
<td><INPUT TYPE="text" MAXLENGTH=10 SIZE=8 NAME="buffer"
           VALUE="<?php echo $buffer;?>"> minutes</td>
</tr>

<tr>
<td></td>
<td> <INPUT TYPE="Submit" VALUE="Compute"></td>
</tr>
        
</table>
</form>

<br>
<br>
<?php echo $resultHTML;?>

</center>

</body>


</html>





<?php





/**
 * Filters a $_REQUEST variable using a regex match.
 *
 * Returns "" (or specified default value) if there is no EXACT match.
 * Entire variable value must match regex, with no extra characters before
 * or after part matched by regex.
 */
function lt_requestFilter( $inRequestVariable, $inRegex, $inDefault = "" ) {
    if( ! isset( $_REQUEST[ $inRequestVariable ] ) ) {
        return $inDefault;
        }

    return lt_filter( $_REQUEST[ $inRequestVariable ], $inRegex, $inDefault );
    }


/**
 * Filters a value  using a regex match.
 *
 * Returns "" (or specified default value) if there is no EXACT match.
 * Entire value must match regex, with no extra characters before
 * or after part matched by regex.
 */
function lt_filter( $inValue, $inRegex, $inDefault = "" ) {
    
    $numMatches = preg_match( $inRegex,
                              $inValue, $matches );

    if( $numMatches != 1 ) {
        return $inDefault;
        }

    if( $matches[0] == $inValue ) {
        return $matches[0];
        }
    else {
        return $inDefault;
        }
    }


?>