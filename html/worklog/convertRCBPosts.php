<?php



$user_id = "jcr13";
$blog_name = "features";

$directoryPath = "rcbfiles";


$databaseServer = "localhost";
$databaseUsername = "jcr13_jcrsbuser";
$databasePassword = "novapear";
$databaseName = "jcr13_jasonrohrerseedblogs";
$tableNamePrefix = "";
    
    
mysql_connect( $databaseServer, $databaseUsername, $databasePassword )
     or fatalError( "Could not connect to database server: " .
                    mysql_error() );
    
mysql_select_db( $databaseName )
     or fatalError( "Could not select $databaseName database: " .
                    mysql_error() );


//$fileList = scandir( $directoryPath );

// php 4 alternative to scandir
$dirHandle  = opendir( $directoryPath );
while( false !== ( $filename = readdir( $dirHandle ) ) ) {
    if( preg_match( "/\.txt/", $filename ) ) {
        $fileList[] = $filename;
        }
    }


foreach( $fileList as $i => $fileName ) {
    $fileNameParts = preg_split( "/\./", $fileName );

    $timestamp = $fileNameParts[0];

    // $fileContents = file_get_contents( $directoryPath . "/" . $fileName );

    // older php equivalent of file_get_contents
    $fileContents = implode( "", file( $directoryPath . "/" . $fileName ) );

    // escape all quotes to avoid breaking SQL query
    $fileContents = addslashes( $fileContents );
    
    $fileParts = preg_split( "/\n/", $fileContents );

    // first line is subject
    $subject_line = $fileParts[0];

    // strip off subject
    array_shift( $fileParts );

    $intro_text = implode( "\n", $fileParts );

    // body is empty for imported posts
    $body_text = "";

    
    $post_id = $user_id . "_" . $timestamp . "_0";

    $allow_comments = 1;
    $expiration_date = "NULL";
    $postApproved = 1;

    // An example SQL timestamp:   "2005-01-19 17:22:50"

    $sqlTimestamp = date( "Y-m-d H:i:s", $timestamp );
    
    $query = "INSERT INTO $tableNamePrefix"."posts VALUES ( " .
        "'$post_id', '$blog_name', '$user_id', '$sqlTimestamp', " .
        "'$sqlTimestamp', $expiration_date, '$allow_comments',".
        "'$postApproved', ".
        "\"0\", '$subject_line', " .
        "'$intro_text', '$body_text' );";

    echo "Query = $query<BR><BR><HR>";

    mysql_query( $query )
        or fatalError( "Database query failed:<BR>$query<BR><BR>" .
                       mysql_error() );
    }

mysql_close();




/**
 * Displays the error page and dies.
 *
 * @param $message the error message to display on the error page.
 */
function fatalError( $message ) {
    //global $errorMessage;

    // set the variable that is displayed inside error.php
    //$errorMessage = $message;
    
    //include_once( "error.php" );

    // for now, just print error message
    echo( "<B>Fatal error:</B>  $message<BR>" );
    die();
    }