set output "witnessSubnauticaA.eps"

call 'firstMonthSetup.gnuplot'

set xrange [0:26]
set yrange [0:1400]


plot "witnessNumbered.dat" using 1:2 title "The Witness" with linespoints pointtype 5 linecolor 5 lt 1 lw 3