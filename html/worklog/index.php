<?php
include_once( "seedBlogs.php" );
include( "header.php" );
?>


<TABLE WIDTH=100% BORDER=0 CELLSPACING=0 CELLPADDING=0><TR><TD>
<?php

// insert a Features seedBlog here

seedBlog(
    // name of this seed blog in the database
    "features",
    // 1 = show intro text below headlines
    // 0 = show only headlines
    1,
    // 1 = show author for each post
    // 0 = hide author
    0,
    // 1 = show creation date for each post
    // 0 = hide dates
    1,
    // 2 = allow custom order tweaking with up/down widgets
    // 1 = order by creation date (newest first)
    // 0 = order by expiration date (oldest first)
    1,
    // show at most 6 features
    6,
    // skip none of them (start with first post)
    0,
    // show the archive link
    1,
    // hide the submission link from public
    0 );


?>

</TD></TR></TABLE>




<?php
include( "footer.php" );
?>