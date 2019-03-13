set output "secondMonthE.eps"

call 'secondMonthSetup.gnuplot'

set arrow from 15,7400 to 19.5,8300 head

set label "Christmas Day" at 15,7400 right font ",20"



plot "<(sed -n '1,32p' tcdSecondMonth.dat)" using 1:3 title "The Castle Doctrine" with linespoints pointtype 5 linecolor 5 lt 1 lw 3, "<(sed -n '1,32p' oholSecondMonth.dat)" using 1:3 title "One Hour One Life" with linespoints pointtype 7 linecolor 4 lt 1 lw 3,  "<(sed -n '3,32p' tcdSecondMonth.dat)" using 1:($3*3.389) title "Expected Curve" with linespoints pointtype 6 linecolor 2 lt 2 lw 2

