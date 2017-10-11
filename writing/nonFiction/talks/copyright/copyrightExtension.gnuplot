set terminal postscript eps color
set output "copyrightExtension.eps"

set size .6,.6

set yrange [0: 120]
#set ytics 100

set xrange [1770: 2030]
#set xtics 1

set nokey

#set xdata time
#set timefmt "%m/%d/%Y"

#set mxtics 2

#set format x "%b %1d"
#set format x "%b"

#set nomxtics

set title "US Copyright Extensions"
set xlabel "Year"
set ylabel "Years of Protection"
plot "copyrightExtension.data" using 1:2 title "test" with linespoints pt 6, "mickeyAge.data" using 1:2 title "test" with lines lt 3 

pause -1 "Hit any key to continue"