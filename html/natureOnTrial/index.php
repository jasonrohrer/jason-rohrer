<?php
include_once( "seedBlogs.php" );
include_once( "header.php" );
?>

</CENTER>

<?php
seedBlog(
    // name of this seed blog in the database
    "static_blocks",
    // 1 = show intro text below headlines
    // 0 = show only headlines
    1,
    // 1 = show author for each post
    // 0 = hide author
    0,
    // 1 = show creation date for each post
    // 0 = hide dates
    0,
    // 2 = allow custom order tweaking with up/down widgets
    // 1 = order by creation date (newest first)
    // 0 = order by expiration date (oldest first)
    2,
    // show an unlimited number of static blocks
    -1,
    // skip none of them (start with first post)
    0,
    // hide the archive link
    0,
    // hide the submission link from public
    0 );
?>

<BR>
<BR>

<H2>Updates:</H2>
<?php
seedBlogFormatted(
    // name of this seed blog in the database
    "updates",
    // 1 = show intro text below headlines
    // 0 = show only headlines
    1,
    // 1 = show author for each post
    // 0 = hide author
    1,
    // 1 = show creation date for each post
    // 0 = hide dates
    1,
    // 2 = allow custom order tweaking with up/down widgets
    // 1 = order by creation date (newest first)
    // 0 = order by expiration date (oldest first)
    1,
    // show at most 8 posts
    8,
    // skip none of them (start with first post)
    0,
    // show the archive link
    1,
    // show the submission link from public
    1,
    // opening tags for each story block
    "<TABLE WIDTH=100% CELLSPACING=0 CELLPADDING=1>
     <TR><TD BGCOLOR=#000000>
     <TABLE WIDTH=100% CELLSPACING=0 CELLPADDING=6>
     <TR><TD BGCOLOR=#FFFFFF>",
    // closing tags for each story block
    "</TD></TR></TABLE></TD></TR></TABLE>",
    // opening tags for headlines
    "<FONT SIZE=4>",
    // closing tags for headlines
    "</FONT><HR>",
    // opening tags for each text block
    "",
    // closing tags for each text block
    "",
    // story separator
    "<BR><BR>" );
?>

<BR>

<?php
// show an RSS feed button here
seedBlogRSSButton(
    // the name of the blog in the database
    "updates",
    // title of the RSS channel
    "Nature On Trial Updates",
    // description of the channel
    "Updates about Jason Rohrer's fight for natural landscaping rights.",
    // include at most 10 features in the feed
    10,
    // show authors
    1,
    // show post dates
    1 );
?>

<CENTER>

<?php include_once( "footer.php" ); ?>


