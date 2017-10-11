\version "2.6.3"

\header {
  title = "Last Dance in an '80s Teen Movie"
  composer = "Jason Rohrer (1977-)"
  tagline = ""
  copyright = "October 2006 (Placed in the public domain)"
}


harmonyChords = \chordmode{ c1 | g | a4:m f2. | f4 g a:m g }

harmonyLine = \relative c {
     <c e g>4
     <c e g>
     <c e g>
     <c e g>
     |
     <g' d b>
     <g d b>
     <g d b>
     <g d b>
     |
     <a, c e>
     <a c f>
     <a c f>
     <a c f>
     |
     <f a c>
     <g b d>
     <a c e>
     <g b d>
     }


melody = \relative c' {
     c1
     |
     c8 c c c c4 d
     |
     b c2 c8 d
     |
     e4 d c d
     |

     e1
     |
     d4 d d e
     |
     b c2 c8 d
     |
     e4 d c d
     |

     g2 g8( a) e4 
     |
     e2 g8 a e d 
     |
     d8 e g g g e d e
     | 
     g4 e4 d4 e4
     |

     c8 c c c c4 d
     |
     b8 b b4 b8 b c4
     |
     a8 b c2 c8 d8
     |
     e4 d c d


     e1
     |
     d8 d d d d4 e
     |
     b c2 c8 d
     |
     e8 e d4 c d
     |

     c1
     |
     c8 c c c c4 d
     |
     b c2 c8 d
     |
     e4 d c d
     |

     c1
     }


voiceWords = \lyricmode{
  I hard -- ly re -- cog -- nize your face now.
  Can you tell me your name?

  We must have been to -- geth -- er
  some -- time long a -- go, but

  I have grown old.
  Mem -- or -- ies are sink -- ing 
  like De -- cem -- ber sun be -- hind the hills.  Still, 

  no -- thing in the world can
  pull my gaze down a -- way 
  from your eyes. I am
  cap -- tured by them.

  I ne -- ver dreamed I'd lose my 
  own child in the cur -- rents of life, but

  I bare -- ly re -- cog -- nize your face, dear.
  Can you tell me your name
  
  please?
}


<<
  \context ChordNames { 
	
	  \harmonyChords
	  \harmonyChords
	  \harmonyChords
	  \harmonyChords
	  \harmonyChords
	  \harmonyChords
	  c1
	}

  \context GrandStaff <<

	

	\context Staff = upper {
	  \context Voice = vocals
	  \relative c {
		\time 4/4 
		\clef treble
		\melody
		|
		\bar "|."
	  }
	}
	\lyricsto "vocals" \new Lyrics 
	  \with {
		\override LyricText #'font-size = #'-1
	  } 
	  \voiceWords
	
	
	\context Staff = lower { 
	  \relative c {
		\time 4/4
		\clef bass
		\stemDown
		\harmonyLine
		\harmonyLine
		\harmonyLine
		\harmonyLine
		\harmonyLine
		\harmonyLine
		<c e g>1       
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
    leftmargin = 0.5\in
	linewidth = 7.5\in
	topmargin = 0\in
	bottommargin = 0.125\in
	footsep = 0\in
    }
  




