set xdata time
set timefmt "%Y%m%d"

set datafile separator ','


#set term postscript enhanced color size 900,700
#set output "out.eps"

set terminal wxt size 900,700

set multiplot layout 6, 2 title "Daily Death Patterns and Winter Climates\n "

set label "Top 6 Deadliest Cold-Winter States" at screen 0.125, 0.95 tc rgb "#0000FF"

set label "Top 6 Deadliest Warm-Winter States" at screen 0.65, 0.95 tc rgb "#FF0000"

set yrange [0:400]
set ytics 100


plot "nyCleaned.txt" using 1:4 with lines lc rgb "#0000FF" title "New York"

plot "ca.txt" using 1:4 with lines lc rgb "#FF0000" title "California"


plot "njCleaned.txt" using 1:4 with lines lc rgb "#0000FF" title "New Jersey"

plot "tx.txt" using 1:4 with lines lc rgb "#FF0000" title "Texas"


plot "ma.txt" using 1:4 with lines lc rgb "#0000FF" title "Massachusetts"

plot "fl.txt" using 1:4 with lines lc rgb "#FF0000" title "Florida"


plot "il.txt" using 1:4 with lines lc rgb "#0000FF" title "Illinois"

plot "la.txt" using 1:4 with lines lc rgb "#FF0000" title "Louisiana"



plot "pa.txt" using 1:4 with lines lc rgb "#0000FF" title "Pennsylvania"

plot "az.txt" using 1:4 with lines lc rgb "#FF0000" title "Arizona"


plot "mi.txt" using 1:4 with lines lc rgb "#0000FF" title "Michigan"

plot "ga.txt" using 1:4 with lines lc rgb "#FF0000" title "Georgia"


unset multiplot

pause -1
