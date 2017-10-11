\version "2.6.3"

\header {
  title = "Frost"
  composer = "Jason Rohrer (1977-)"
  tagline = ""
  subsubtitle = "October 2005 (Placed in the public domain)"
}


harmonyChords = \chordmode{ a1 | e | d | e4 d4 e2 }
harmonyCloseChords = \chordmode{ a1 | e | fis2:m a2 | fis1:m | e1:sus4 | e1:sus4 }

harmonyLine = \relative c' {
  <cis a>4 cis8 a cis4 a
  |
  <b gis>4 b8 gis b4 gis
  |
  <fis d'>4 fis8 d' fis,4 d'
  |
  e4 d d e
}

harmonyCloseLine = \relative c' {
  a4 cis a cis
  |
  gis4 b gis b
  |
  cis,4 a' cis,4 a'
  |
  fis4 a f a
  |
  e4
  #( set-octavation 1 )
  a' a2
  |
  #( set-octavation 0 )
  e,4 
  #( set-octavation 1 )
  a' a2
}
 
% done down to here.  Still working...
 

melody = \relative c'' {
  e1 | cis1 | b1 | r1
  |
  e1 | fis4 e4 cis2 | b1 | r1
  |
  e1 | cis2 e2 | b1 | r1
  |
  e1 | fis4 e4 cis2 | b1 | r1
}


% fixme:  want slur in here
melodyClose = \relative c'' {
  e1 | cis1 | fis2 e4( e4 ) | cis2.( cis4 ) | b1 | r1
}



voiceWordsFirst = \lyricmode{
  Hard, cruel cold.
  Crawl slow -- ly to bed.
  Dreams thaw and freeze.
  I can't feel my toes.

  Sun -- rise melts our __ first __ frost.
}

voiceWordsSecond = \lyricmode{
Last warm day.
Back -- yard is a swamp.
Leaves turn and fall.
Sun low in the sky.

% multiple sylables for each slur in this stanza
\set ignoreMelismata = ##t
Win -- ter makes us for -- get the war.
}


<<
  \context ChordNames { 
	
	  \repeat volta 2{ \harmonyChords
					   \harmonyChords
					   \harmonyChords
					   \harmonyChords
					   \harmonyCloseChords }
	}

  \context GrandStaff <<

	

	\context Staff = upper {
	  \context Voice = vocals
	    \relative c {
		\key a \major
		\time 4/4 
		\clef treble
		\repeat volta 2{ \melody \melodyClose }
	  }
	}
	\lyricsto "vocals" \new Lyrics 
	  \with {
		\override LyricText #'font-size = #'-1
	  } 
	  \voiceWordsFirst
	\lyricsto "vocals" \new Lyrics 
	  \with {
		\override LyricText #'font-size = #'-1
	  } 
	  \voiceWordsSecond
	
	
	\context Staff = lower { 
	  \relative c {
		\key a \major
		\time 4/4
		\clef bass
		\stemDown
		\repeat volta 2{ \harmonyLine
						 \harmonyLine
						 \harmonyLine
						 \harmonyLine
						 \harmonyCloseLine }
	  }
	}
  >>
>>
  
  
  \layout {
	\context { \GrandStaff \accepts "Lyrics" }
	\context { \Lyrics \consists "Bar_engraver" 
			   \override BarLine #'bar-size = #4 }
	\context { \Score
			   \override BarNumber #'extra-offset = #'(-1.0 . +1.0) }
	\context { \Score \override ChordName #'font-family = #'roman }
	\context { \Score \override ChordName #'font-size = #'-1 }
  }
    

  \paper { 
	#(set-paper-size "letter")
    leftmargin = 0.75\in
	linewidth = 7\in
	topmargin = 0.5\in
	bottommargin = 0.5\in
	betweensystempadding = #1
	raggedbottom=##f
	raggedlastbottom=##f

	%betweensystemspace = 0.1\in
    }
  




