set output "witnessSubnauticaC.eps"

call 'firstMonthSetup.gnuplot'

set xrange [0:26]
set yrange [0:3800]


plot "witnessNumbered.dat" using 1:2 title "The Witness" with linespoints pointtype 5 linecolor 5 lt 1 lw 3, "subnauticaNumbered.dat" using 1:2 title "Subnautica" with linespoints pointtype 7 linecolor 4 lt 1 lw 3