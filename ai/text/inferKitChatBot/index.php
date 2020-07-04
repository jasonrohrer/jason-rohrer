<?php


include_once( "settings.php" );



$action = cs_requestFilter( "action", "/[A-Z_]+/i" );




if( $action == "show_log" ) {
    cs_showLog();
    }
else if( $action == "add_chat" ) {
    cs_addChat();
    }
else if( $action == "get_machine_response" ) {
    cs_getMachineResponse();
    }
else {
    cs_showChat( "", false, false, false );
    }



function cs_firstLineOnly( $inString ) {
    $gennedLineParts = array_filter( explode( "\n", $inString ) );

    return $gennedLineParts[0];
    }



function cs_addChat() {
    $said = cs_requestFilter( "said", "/[ A-Z0-9,.?\"'!$%*()+=&#@:;\-_]+/i" );
    $chatSoFar = cs_requestFilter( "chat_so_far", "/[ <>=\/A-Z0-9,.?\"'!$%*()+=&#@:;\-_\n]+/i" );

    cs_showChat( "$chatSoFar<br><br><b><font color=blue>Human:</font></b> $said", true, false, false );
    }




function cs_getMachineResponse( $askingForMore ) {
    $chatSoFar = cs_requestFilter( "chat_so_far", "/[ <>=\/A-Z0-9,.?\"'!$%*()+=&#@:;\n\-_]+/i" );

    // replace <br> with \n
    $chatPlain = str_replace( "<br>", "\n", $chatSoFar );

    // strip all other HTML tags
    $chatPlain = strip_tags( $chatPlain );

    $askingForMore = cs_requestFilter( "asking_for_more", "/[01]+/i", 0 );
    
    if( ! $askingForMore ) {
        // human just spoke, give computer a prompt for its speech
        $chatPlain =  $chatPlain . "\n\nComputer:";
        }
    
    //$chatPlain = implode("\n", array_filter(explode("\n", $chatPlain)));

    //$chatPlain =  str_replace( "\n", "", $chatPlain );

    //$chatPlain = "Test";

    // leave room for introduction
    
    $chatPlain = substr( $chatPlain, -700 );

    // human's first line has \n\n in front
    $chatPlain = "What follows is a conversation between a human and the world's most sophisticated artificial intelligence.  As you can see, the computer displays a shocking amount of cleverness and wit, as the discussion quickly becomes deep and philosophical." . $chatPlain;
    
    
    
    $jsonArray =
        array('prompt' => array( "text" => $chatPlain ), "length" => 100 );
    /*
    $postBody = "{\n".
        "\"prompt\": {\n".
        "\"text\": \"$chatPlain\" },\n".
        "\"length\": 100\n".
        "}";
    */
    $postBody = json_encode( $jsonArray );
    
    $url = 'https://api.inferkit.com/v1/models/standard/generate';

    global $api_key;
    
    $options = array(
        'http' => array(
            'header'  =>
            "Content-type: application/json\r\n".
            "Authorization: Bearer $api_key\r\n".
            "Content-Length: " . strlen($postBody) . "\r\n",
            'method'  => 'POST',
            'content' => $postBody ) );
    $context  = stream_context_create( $options );
    $result = file_get_contents( $url, false, $context );

    $len = strlen( $chatPlain );

    // debug printout
    //echo "<pre>chat plain:\n$chatPlain\n($len long)\nURL $url\npost body=\n$postBody\nresult = $result</pre>";

    if( $result === FALSE ) {

        cs_showChat( "$chatSoFar", false, true, false );
        
        }
    else {
        
        $a = json_decode( $result, true );
        $textGen = $a['data']['text'];
        
        
        $gennedChatLines = preg_split('/Computer:|Human:|Humans:|Machine:/', $textGen );


        $gennedLine = "";

        $computerHasBeenCutOff = false;
        
        if( count( $gennedChatLines ) > 1 ) {
            $gennedLine = trim( $gennedChatLines[0] );

            // make sure first line doesn't contain multiple lines
            $gennedLine = cs_firstLineOnly( $gennedLine );
            }
        else {
            // only one line, without Computer or Human tags?

            // take it raw
            $gennedLine = trim( $textGen );

            // only consider first line, if there's more than one
            $gennedLine = cs_firstLineOnly( $gennedLine );

            
            // watch out for it being cut-off...

            // does it end in proper ending punctiuation?

            $lastI = strlen( $gennedLine ) - 1;

            $lastChar = $gennedLine[ $lastI ];

            if( $lastChar != '.' &&
                $lastChar != '!' &&
                $lastChar != '?' &&
                $lastChar != '"' ) {

                // cut off!
                $computerHasBeenCutOff = true;
                }
            }

        
        // replace \n with <br> if computer has given us more than one line
        $gennedLine = str_replace( "\n", "<br>", $gennedLine );

        
        if( $computerHasBeenCutOff ) {
            // wait for more by making another request
            // don't let human type just yet until computer is done
            // with longer response.

            if( $askingForMore ) {
                cs_showChat( "$chatSoFar $gennedLine",
                             true, false, true );
                }
            else {
                cs_showChat(
                    "$chatSoFar<br><br>".
                    "<b><font color=red>Computer:</font></b> $gennedLine",
                    true, false, true );
                }
            }
        else {
            // show this complete computer response, and let human type more

            if( $askingForMore ) {
                cs_showChat( "$chatSoFar $gennedLine",
                             false, false, false );
                }
            else {
                cs_showChat(
                    "$chatSoFar<br><br>".
                    "<b><font color=red>Computer:</font></b> $gennedLine",
                    false, false, false );
                }
            }
        }
    }





function cs_showChat( $chatSoFar, $getMore, $tryReload, $noComputerPrompt ) {
    $chatSoFarOrig = $chatSoFar;

    $askingForMore = 0;

    if( $noComputerPrompt ) {
        $askingForMore = 1;
        }
    

    if( $getMore || $tryReload ) {
        
        $time = 0;

        $computerWaitString = "[...]";
        
        if( $tryReload ) {
            $time = 5;
            $computerWaitString = "[...FAILED, retrying...]";
            }

        // old meta refresh way, which packs everything into a url
        // that eventually overflows.
        // instead we do a javascript form post below

        //$chatSoFarEncoded = urlencode( $chatSoFar );
        //echo "<meta http-equiv=refresh content='$time; URL=index.php?action=get_machine_response&chat_so_far=$chatSoFarEncoded'>";

        if( $noComputerPrompt ) {
            // Computer: prompt already included in chat,
            // because we're waiting for more
            $chatSoFar = "$chatSoFar $computerWaitString";
            }
        else {
            // computer hasn't said anything yet
            // show Computer: prompt with wait string
            $chatSoFar = "$chatSoFar<br><br><b><font color=red>Computer:</font></b> $computerWaitString";
            }
        }
    
    
    echo "<center><table border=1 width=640><tr><td>";

    echo "<div id=chatBox style='height:500px;overflow:auto;background-color:whitesmoke;padding:20px;'>";
    

    echo "$chatSoFar";

    echo "</div>";
    
    
    echo "</td></tr>";
    

    echo "<tr><td>";


    

    $disabled = "";

    if( $getMore ) {
        $disabled = "disabled";
        }

    $chatSoFarEncoded = htmlspecialchars( $chatSoFar );
    
?>
    <FORM ACTION="index.php" METHOD="post">
         <INPUT TYPE="hidden" NAME="action" VALUE="add_chat">
         <INPUT TYPE="hidden" NAME="chat_so_far" VALUE="<?php echo $chatSoFarEncoded; ?>">
         Human: <INPUT TYPE="text" MAXLENGTH=200 SIZE=40 NAME="said" autofocus="autofocus" <?php echo $disabled; ?> >
         <INPUT TYPE="Submit" VALUE="Send" <?php echo $disabled; ?> >
        </FORM>
<?php
    
    
    echo "</td></tr></table></center>";

?>
    <script>
        
    
<?php


    if( $getMore || $tryReload ) {
        $slashChat = addslashes( $chatSoFarOrig );
        
?>
        //document.body.innerHTML = document.body.innerHTML + "bla bla A";
        
        var myForm = document.createElement('form');
        myForm.setAttribute('action', 'index.php');
        myForm.setAttribute('method', 'post');
        myForm.setAttribute('hidden', 'true');

        var myInput = document.createElement('input');
        myInput.setAttribute('type', 'hidden');
        myInput.setAttribute('name', 'action');
        myInput.setAttribute('value', 'get_machine_response');
        myForm.appendChild(myInput);

        var myInputB = document.createElement('input');
        myInputB.setAttribute('type', 'hidden');
        myInputB.setAttribute('name', 'asking_for_more');
        myInputB.setAttribute('value', '<?php echo $askingForMore; ?>');
        myForm.appendChild(myInputB);

        var myInputC = document.createElement('input');
        myInputC.setAttribute('type', 'hidden');
        myInputC.setAttribute('name', 'chat_so_far');
        myInputC.setAttribute('value', '<?php echo $slashChat; ?>');
        myForm.appendChild(myInputC);
        
        document.body.appendChild(myForm);
        myForm.submit();
        
        //document.body.innerHTML = document.body.innerHTML + "bla blaB";


<?php
              
        }

?>
    var objDiv = document.getElementById("chatBox");
    objDiv.scrollTop = objDiv.scrollHeight;
    </script>
<?php
    
    }





/**
 * Filters a $_REQUEST variable using a regex match.
 *
 * Returns "" (or specified default value) if there is no match.
 */
function cs_requestFilter( $inRequestVariable, $inRegex, $inDefault = "" ) {
    if( ! isset( $_REQUEST[ $inRequestVariable ] ) ) {
        return $inDefault;
        }

    return cs_filter( $_REQUEST[ $inRequestVariable ], $inRegex, $inDefault );
    }


/**
 * Filters a value  using a regex match.
 *
 * Returns "" (or specified default value) if there is no match.
 */
function cs_filter( $inValue, $inRegex, $inDefault = "" ) {
    
    $numMatches = preg_match( $inRegex,
                              $inValue, $matches );

    if( $numMatches != 1 ) {
        return $inDefault;
        }
        
    return $matches[0];
    }


?>