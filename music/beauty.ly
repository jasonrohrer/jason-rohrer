\version "2.6.3"

\header {
  title = "Beauty"
  composer = "Jason Rohrer (1977-)"
  tagline = ""
  copyright = "May 2007 (Placed in the public domain)"
}


harmonyChords = \chordmode{ d1 | fis:m | g | a }

harmonyLine = \relative c {
     <d fis a>4
	 <d fis a>
	 <d fis a>
	 <d fis a>
     |
     <cis fis cis'>
     <cis fis cis'>
     <cis fis cis'>
     <cis fis cis'>
     |
	 <d g d'>
	 <d g d'>
	 <d g d'>
	 <d g d'>
     |
	 <e a e'>
	 <e a e'>
	 <e a e'>
	 <e a e'>
     }


melodyA = \relative c'' {
  a4. fis8 fis4 e4
  |
  a4. e8 g4 fis4
  |
  a4. d8 e4 b4
  |
  cis4. d8 e8 fis8 g,4
}


melodyB = \relative c' {
  d2 d8 e8 e4
  |
  fis2 a,2
  |
  d4 e b e
  |
  g8 fis e d   g fis cis e
}


voiceWordsFirst = \lyricmode{
  Last night I heard yet a -- noth -- er song a --
  bout an eigh -- teen year old beau -- ty

  queen.  All you oth -- er girls must feel like you
  are com -- plete -- ly   in -- vi -- si -- ble.

  Ug -- ly girl this is a song for you.
  You're ig -- nored by all oth -- er songs.
}

voiceWordsSecond = \lyricmode{
  We know that beau -- ty is on -- ly skin deep, 
  but your skin is all that we can see.

  Though we would love to see much deep -- er.
  X -- ray eyes are just a rip -- off.

  Ug -- ly girl we feel your pain 'cause
  we will all be ug -- ly too some -- day.
}


endingWords = \lyricmode{

  You know that beau -- ty is on -- ly skin deep,
  but my skin is all that you can see.

  Though I wish you could see much deep -- er.
  Then you'd see the ug -- li -- ness in 

  % 17 blank words in here to push this last word to
  % the third alternative ending
  % a hack!
  " "  " "  " "  " "
  " "  " "  " "  " "
  " "  " "  " "  " "
  " "  " "  " "  " "
  " "
  me.
}


<<
  \context ChordNames { 
	
	  \repeat volta 3{ \harmonyChords
					   \harmonyChords }
	  \alternative{ {\harmonyChords} {d1} }
	}

  \context GrandStaff <<

	

	\context Staff = upper {
	  \context Voice = vocals
	    \relative c {
		\key d \major
		\time 4/4 
		\clef treble
		\repeat volta 3{ \melodyB \melodyB}
		\alternative{ {\melodyA} {d'1} }
		|
		\bar "|."
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
	
	\lyricsto "vocals" \new Lyrics 
	  \with {
		\override LyricText #'font-size = #'-1
	  } 
	  \endingWords

	
	\context Staff = lower { 
	  \relative c {
		\key d \major
		\time 4/4
		\clef bass
		\stemDown
		\repeat volta 3{ \harmonyLine
						 \harmonyLine }

		\alternative{ { \harmonyLine } { <d fis a>1 } }       
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
	topmargin = 0.5\in
	bottommargin = 0.5\in
    }
  




