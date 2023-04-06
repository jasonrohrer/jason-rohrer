<?php

$start = ev_requestFilter( "start",
                           "/[\-]?[0-9]+\.[0-9]+,\s?[\-]?[0-9]+\.[0-9]+/", "" );
$end = ev_requestFilter( "end",
                         "/[\-]?[0-9]+\.[0-9]+,\s?[\-]?[0-9]+\.[0-9]+/", "" );

$distMiles = ev_requestFilter( "dist",
                               "/[0-9]+[.]?[0-9]?/", "0" );


$numPoints = ev_requestFilter( "num_points", "/[0-9]+/", "0" );


if( $start != "" && $end != "" && $distMiles != 0 && $numPoints != 0 ) {
    // generate SVG

    $startParts = preg_split( "/,/", $start );
    $endParts = preg_split( "/,/", $end );

    $startLat = trim( $startParts[0] );
    $startLon = trim( $startParts[1] );

    $endLat = trim( $endParts[0] );
    $endLon = trim( $endParts[1] );

    $latDelta = $endLat - $startLat;
    $lonDelta = $endLon - $startLon;

    $distMeters = $distMiles * 5280 * 0.3048;

    
    $points = array();
    
    for( $i=0; $i<$numPoints; $i++ ) {
        $curLat = $startLat + $latDelta * $i / ($numPoints - 1);
        $curLon = $startLon + $lonDelta * $i / ($numPoints - 1);

        $points[] = "$curLat,$curLon";
        }

    $pointList =  join( "|", $points );

    // 5 minutes
    $ctx = stream_context_create(
        array( 'http'=>
               array(
                   'timeout' => 300,  // 5 minutes
                     )
               ));

    // our own private open-elevation instance running on Linode
    // the public API servers are unreliable
    $result =
        file_get_contents(
            //"http://45.33.85.207/api/v1/lookup?locations=$pointList",
            "https://api.opentopodata.org/v1/ned10m?locations=$pointList",
        false, $ctx );

    //echo $result;
    
    
    $resultObj = json_decode( $result );

    //var_dump( $resultObj );
    
    $elev = array();
    foreach( $resultObj->results as $r ) {
        //var_dump( $r );
        $elev[] = $r->elevation;
        //echo $r->elevation;
        //echo "\n";
        }
    //exit();
    

    // 2000 meters is just tall enough to include Mt. Washington
    $maxElev = 2000;


    // target height in mm for all images
    $targetImageHeight = 20;
    
    $scaleFactor = $targetImageHeight / $maxElev;
    
    
    // square plot, regardless of hike distance
    //$plotWidth = 2000;
    $plotWidth = $distMeters * $scaleFactor;

    $plotHeight = $maxElev * $scaleFactor;
    
    header( 'Content-Type: image/svg+xml' );

    echo '<?xml version="1.0" encoding="utf-8"?>
<svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" width="';
    // room on left and right for gap around plot
    echo $plotWidth + 20;
    echo '" height="';
    echo $plotHeight;
    echo '">';
    echo "\n";
    echo '<rect x="0" width="';
    echo $plotWidth + 20;
    echo '" height="';
    echo $plotHeight;
    echo '" fill="#FFEEEE"/>';
    echo "\n";
    
    
    $jump = $plotWidth / ( count( $elev ) - 1 );

    // line segments
    echo '<path d="M 10 ';
    echo $scaleFactor * ( $maxElev - $elev[0] );
    echo "";
    
    for( $i = 1; $i< count( $elev ); $i++ ) {
        
    
        echo ' L ';
        echo 10 + ($i) * $jump;
        echo ' ';
        echo $scaleFactor * ( $maxElev - $elev[$i] );
        echo "";
        }
    
    
    echo '" stroke="black" fill="transparent"/>
</svg>';
    
    }
else {
    echo "Start = $start, end =$end, dist=$dis, numPoints = $numPoints<br>";
    ?>
    
    <FORM ACTION="getElevationSVG.php" METHOD="get">
        Start Coords: <INPUT TYPE="text" MAXLENGTH=40 SIZE=40 NAME="start"
             VALUE=""><br>
        End Coords: <INPUT TYPE="text" MAXLENGTH=40 SIZE=40 NAME="end"
             VALUE=""><br>
        Dist Miles: <INPUT TYPE="text" MAXLENGTH=40 SIZE=40 NAME="dist"
             VALUE=""><br>
        Num Points: <INPUT TYPE="text" MAXLENGTH=40 SIZE=40 NAME="num_points"
             VALUE="30"><br>
        <INPUT TYPE="Submit" VALUE="Generate">
    </form>
        
<?php
    }





/**
 * Filters a $_REQUEST variable using a regex match.
 *
 * Returns "" (or specified default value) if there is no match.
 */
function ev_requestFilter( $inRequestVariable, $inRegex, $inDefault = "" ) {
    if( ! isset( $_REQUEST[ $inRequestVariable ] ) ) {
        return $inDefault;
        }

    return ev_filter( $_REQUEST[ $inRequestVariable ], $inRegex, $inDefault );
    }


/**
 * Filters a value  using a regex match.
 *
 * Returns "" (or specified default value) if there is no match.
 */
function ev_filter( $inValue, $inRegex, $inDefault = "" ) {
    
    $numMatches = preg_match( $inRegex,
                              $inValue, $matches );

    if( $numMatches != 1 ) {
        return $inDefault;
        }
        
    return $matches[0];
    }

?>