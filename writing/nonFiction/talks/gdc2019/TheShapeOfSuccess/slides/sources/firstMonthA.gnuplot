set output "firstMonthA.eps"

call 'firstMonthSetup.gnuplot'


plot "<(sed -n '1,32p' tcdFirstMonth.dat)" using 1:3 title "The Castle Doctrine" with linespoints pointtype 5 linecolor 5 lt 1 lw 3

