set output "firstMonthH.eps"

call 'firstMonthSetup.gnuplot'


plot "<(sed -n '1,32p' tcdFirstMonth.dat)" using 1:3 title "The Castle Doctrine" with linespoints pointtype 5 linecolor 5 lt 1 lw 3, "<(sed -n '1,32p' oholFirstMonth.dat)" using 1:3 title "One Hour One Life" with linespoints pointtype 7 linecolor 4 lt 1 lw 3,  "<(sed -n '3,27p' tcdFirstMonth.dat)" using ($1+4):($3*1.185) title "Expected Curve" with linespoints pointtype 6 linecolor 2 lt 2 lw 2

