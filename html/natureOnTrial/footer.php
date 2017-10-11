</CENTER>
</TD></TR>
</TABLE>

</TD>
<TD BGCOLOR=#DDDDDD WIDTH=198 VALIGN=TOP>

<TABLE WIDTH=100% HEIGHT=100% CELLPADDING=5 CELLSPACING=0>
<TR><TD BGCOLOR=#DDDDDD VALIGN=TOP>


<?php
seedBlogs_showSearchBox(
    // 10 characters wide
    10,
    // show the Search button
    true );
?>


<BR>

<!-- Hard-code internal links -->
[<A HREF="index.php">Home</A>] [<A HREF="login.php">Account</A>]<BR><BR>


<B>Trial Documents:</B><BR>

<?php
// use a seedBlog to manage external links
seedBlog(
    // name of this seed blog in the database
    "documents",
    // 1 = show intro text below headlines
    // 0 = show only headlines
    0,
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
    // show an unlimited number of links
    -1,
    // skip none of them (start with first post)
    0,
    // hide the archive link
    0,
    // hide the submission link from public
    0 );
?>

<BR>

<B>External Links:</B><BR>

<?php
// use a seedBlog to manage external links
seedBlog(
    // name of this seed blog in the database
    "external_links",
    // 1 = show intro text below headlines
    // 0 = show only headlines
    0,
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
    // show an unlimited number of links
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
<BR>

<CENTER>
<A HREF="http://hcsoftware.sf.net/seedBlogs">
<IMG SRC="images/madeWithButton_dddddd.png" BORDER=0 WIDTH=96 HEIGHT=28></A>
</CENTER>

<!-- End of sidebar table -->
</TD></TR>
</TABLE>


<!-- End of two-column content table -->
</TD></TR>
</TABLE>


<!-- End of black border table -->
</TD></TR>
</TABLE>


<!-- Creative Commons Public Domain -->
<TABLE BORDER=0>
<TR><TD VALIGN=MIDDLE>
<a rel="license" href="http://creativecommons.org/licenses/publicdomain/"><img alt="Public Domain Dedication" border="0"
src="http://creativecommons.org/images/public/norights-a.gif" VALIGN=MIDDLE /></a>
</TD><TD VALIGN=MIDDLE>
All content on this site is placed in the 
<a rel="license" href="http://creativecommons.org/licenses/publicdomain/">Public Domain</a>.
</TD></TR>
</TABLE>
<!-- /Creative Commons Public Domain -->

</CENTER>



</BODY>
</HTML>

