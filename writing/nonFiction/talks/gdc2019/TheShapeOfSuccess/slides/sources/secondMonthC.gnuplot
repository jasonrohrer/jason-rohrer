set output "secondMonthC.eps"

call 'secondMonthSetup.gnuplot'


plot "<(sed -n '1,32p' tcdSecondMonth.dat)" using 1:3 title "The Castle Doctrine" with linespoints pointtype 5 linecolor 5 lt 1 lw 3, "<(sed -n '1,3p' oholSecondMonth.dat)" using 1:3 title "One Hour One Life" with linespoints pointtype 7 linecolor 4 lt 1 lw 3,  "<(sed -n '3,32p' tcdSecondMonth.dat)" using 1:($3*3.389) title "Expected Curve" with linespoints pointtype 6 linecolor 2 lt 2 lw 2

