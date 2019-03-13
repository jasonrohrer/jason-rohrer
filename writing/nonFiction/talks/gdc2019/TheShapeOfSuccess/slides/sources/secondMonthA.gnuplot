set output "secondMonthA.eps"

call 'secondMonthSetup.gnuplot'


plot "<(sed -n '1,32p' tcdSecondMonth.dat)" using 1:3 title "The Castle Doctrine" with linespoints pointtype 5 linecolor 5 lt 1 lw 3

