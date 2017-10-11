
        <!-- (end body) -->
        </TD></TR></TABLE>
    </TD>
    <TD BGCOLOR=#D7EBCB VALIGN=TOP>
        <TABLE WIDTH=100% CELLPADDING=10><TR><TD>
        <!-- (sidebar) -->


        <?php seedBlogs_showSearchBox( 10, true); ?>

		

        <?php
        // insert a static_menu seedBlog here

        seedBlogFormatted(
            // name of this seed blog in the database
            "static_menu",
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
            // show an unlimited number of menu items
            -1,
            // skip none of them (start with first post)
            0,
            // hide the archive link
            0,
            // hide the submission link from public
            0,
            // opening tags for each story block
            "",
            // closing tags for each story block
            "",
            // opening tags for headlines
            "<TABLE CELLSPACING=3 CELLPADDING=0>
             <TR><TD VALIGN=TOP>[",
            // closing tags for headlines
            "]</TD></TR></TABLE>",
            // opening tags for text block
            "",
            // closing tags for text block
            "",
            // story separator
            "" );
        ?>
        

        
        <BR>
        <FONT COLOR=#2F4323><B>Projects:</B></FONT>
        <BR>
        <?php
        // insert a projects seedBlog here

        seedBlogFormatted(
            // name of this seed blog in the database
            "projects",
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
            // show an unlimited number of projects
            -1,
            // skip none of them (start with first post)
            0,
            // hide the archive link
            0,
            // hide the submission link from public
            0,
            // opening tags for each story block
            "",
            // closing tags for each story block
            "",
            // opening tags for headlines
            "<TABLE CELLSPACING=3 CELLPADDING=0>
             <TR><TD NOWRAP VALIGN=TOP>--</TD><TD VALIGN=TOP>",
            // closing tags for headlines
            "</TD></TR></TABLE>",
            // opening tags for text block
            "",
            // closing tags for text block
            "",
            // story separator
            "" );
        ?>

        <BR>
        <FONT COLOR=#2F4323><B>Reading List:</B></FONT>
        <BR>
        <?php
        // insert a book list seedBlog here

        seedBlogFormatted(
            // name of this seed blog in the database
            "reading_list",
            // 1 = show intro text below headlines
            // 0 = show only headlines
            0,
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
            // show at most 7 books
            7,
            // skip none of them (start with first post)
            0,
            // hide the archive link
            0,
            // hide the submission link from public
            0,
            // opening tags for each story block
            "",
            // closing tags for each story block
            "",
            // opening tags for headlines
            "<TABLE CELLSPACING=3 CELLPADDING=0>
             <TR><TD NOWRAP VALIGN=TOP>--</TD><TD VALIGN=TOP>",
            // closing tags for headlines
            "</TD></TR></TABLE>",
            // opening tags for text block
            "",
            // closing tags for text block
            "",
            // story separator
            "" );
        ?>


        <BR>
        <FONT COLOR=#2F4323><B>Older Posts:</B></FONT>
        <BR>
        <?php
        // insert a seedBlog of older features posts here

        seedBlogFormatted(
            // name of this seed blog in the database
            "features",
            // 1 = show intro text below headlines
            // 0 = show only headlines
            0,
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
            // show 10 old posts
            10,
            // skip the first 6 (those on front page)
            6,
            // hide the archive link
            0,
            // hide the submission link from public
            0,
            // opening tags for each story block
            "",
            // closing tags for each story block
            "",
            // opening tags for headlines
            "<TABLE CELLSPACING=3 CELLPADDING=0>
             <TR><TD NOWRAP VALIGN=TOP>--</TD><TD VALIGN=TOP>",
            // closing tags for headlines
            "</TD></TR></TABLE>",
            // opening tags for text block
            "",
            // closing tags for text block
            "",
            // story separator
            "" );
        ?>

        <BR>
        <BR>
        
        <!-- See clean RSS link below
        <?php
        // RSS 2.0 button for features
        seedBlogRSSButton(
            // name of this seed blog in the database
            "features",
            // title of this RSS feed
            "interchangeable parts",
            // description of this RSS feed
            "Jason Rohrer's journal of inventions and ideas",
            // include at most 0 features in the feed
            $inMaxNumber = 10,
            // show authors for each post in the feed
            0,
            // show dates for each post in the feed
            1 );
        ?>
        -->

        <table BORDER=0 CELLSPACING=0 CELLPADDING=1><tr><td BGCOLOR=#898E79><table BORDER=0 CELLSPACING=0 CELLPADDING=1><tr><td BGCOLOR=#FFFFFF><table BORDER=0 CELLSPACING=0 CELLPADDING=2><tr><td BGCOLOR=#FF6600><a HREF="http://northcountrynotes.org/jason-rohrer/worklog/rss.xml"><font COLOR=#FFFFFF><b>RSS 2.0</b></font></a></td></tr></table></td></tr></table></td></tr></table>

        

        <BR>
        <A HREF="http://hcsoftware.sf.net/seedBlogs/"><IMG
        BORDER=0 SRC="madeWithButton_d8eccb.png" WIDTH=96 HEIGHT=28></A>


        <!-- (end sidebar) -->        
        </TD></TR></TABLE>
    </TD>
    <TD BGCOLOR=#688D77></TD>
</TR>
<!-- Black stripe at bottom -->
<TR><TD BGCOLOR=#56707F></TD>
    <TD COLSPAN=2 BGCOLOR=#000000></TD>
    <TD BGCOLOR=#2F4323></TD>
    <TD BGCOLOR=#688D77></TD>
</TR>        
<TR>
    <TD BGCOLOR=#56707F></TD>
    <TD COLSPAN=2 BGCOLOR=#56707F></TD>
    <TD BGCOLOR=#688D77 ALIGN=CENTER><BR><!--
        <a href="http://sourceforge.net"><img
           src="http://sourceforge.net/sflogo.php?group_id=17004"
           width="88" height="31" border="0" alt="SourceForgeLogo"></a>-->
        <BR><BR>
    </TD>
    <TD BGCOLOR=#688D77></TD>
</TR>
</TABLE>
</CENTER>
        
</BODY>

</HTML>