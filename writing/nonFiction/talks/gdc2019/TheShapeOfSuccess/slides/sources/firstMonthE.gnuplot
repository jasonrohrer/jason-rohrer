set output "firstMonthE.eps"

call 'firstMonthSetup.gnuplot'


plot "<(sed -n '1,32p' tcdFirstMonth.dat)" using 1:3 title "The Castle Doctrine" with linespoints pointtype 5 linecolor 5 lt 1 lw 3, "<(sed -n '1,5p' oholFirstMonth.dat)" using 1:3 title "One Hour One Life" with linespoints pointtype 7 linecolor 4 lt 1 lw 3,  "<(sed -n '3,32p' tcdFirstMonth.dat)" using 1:($3/5.3) title "Expected Curve" with linespoints pointtype 6 linecolor 2 lt 2 lw 2

